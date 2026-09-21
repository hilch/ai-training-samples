<#
.SYNOPSIS
    Shared B&R property-grid engine — XML assembly, template expansion, version filtering,
    element-model conversion, and ParentElement drill-down navigation.

.DESCRIPTION
    Dot-source this file from as-tp-config-api and as-physical-hw-api to share the identical
    property-grid tree-walk logic. Every function is generic (operates on LocalName, carries
    no TP or hardware assumptions).

    Call sequence for callers:
        1. Build the assembled root:
               $root = Get-AssembledPropertiesRoot `
                   -EntryNode  <XmlElement>          # the <Properties> or <Configuration> node to process
                   -IncludeRoot <string>              # base directory for <Include File="..."/> resolution
                   -Version    <string>              # version string for FromVersion/ToVersion filtering
                   [-ExtraTemplateFiles <string[]>]  # additional ArCfgTemplates*.xml to preload
        2. (Optional) Navigate to a scope:
               $scope = Find-ScopeNode -Root $root -ParentElement "FunctionModel/Standard"
        3. Emit JSON-ready models:
               $pg = Get-PropertyGridElements -Root $root -ScopeNode $scope -ParentElement "..." `
                   -MaxDepth 1 -MaxElements 500
               # Returns @{ elements; emittedCount; truncated; hint }

    Usage:
        . "$PSScriptRoot/as-propertygrid.ps1"

    Prerequisites: Compare-Versions must already be available (provided by as-shared.ps1).
#>

#region XML helpers

function Read-PgXmlFile {
    <#
    .SYNOPSIS
        Loads an XML file without resolving external entities/schemas (no network access).
    #>
    param([string]$Path)
    $doc = New-Object System.Xml.XmlDocument
    $doc.XmlResolver = $null
    $doc.Load($Path)
    return $doc
}

function Get-PgAttr {
    <#
    .SYNOPSIS
        Returns an attribute value or $null when missing/empty.
    #>
    param([System.Xml.XmlElement]$El, [string]$Name)
    $v = $El.GetAttribute($Name)
    if ([string]::IsNullOrEmpty($v)) { return $null }
    return $v
}

function Get-PgLocalized {
    <#
    .SYNOPSIS
        Returns a localized attribute, preferring -en, then bare, then -de.
    #>
    param([System.Xml.XmlElement]$El, [string]$Base)
    foreach ($suffix in @("-en", "", "-de")) {
        $v = $El.GetAttribute("$Base$suffix")
        if (-not [string]::IsNullOrEmpty($v)) { return $v }
    }
    return $null
}

#endregion

#region Version filtering

function Test-PgNodeHasVersionAttrs {
    param([System.Xml.XmlElement]$El)
    foreach ($suffix in @("", "1", "2", "3", "4", "5")) {
        if ($El.HasAttribute("FromVersion$suffix") -or $El.HasAttribute("ToVersion$suffix")) { return $true }
    }
    return $false
}

function Test-PgVersionApplies {
    <#
    .SYNOPSIS
        True when $Version falls inside at least one FromVersion/ToVersion(+1..5) range,
        or when the node carries no version attributes at all.

        $VersionMap (optional) supplies a per-dimension version keyed by the numeric suffix
        ("1".."5") and "" for the bare FromVersion/ToVersion. A FromVersion<N> is then compared
        against $VersionMap["<N>"] instead of the scalar $Version. Suffixes absent from the map
        fall back to $Version. An empty map reproduces the original scalar behaviour.
    #>
    param([System.Xml.XmlElement]$El, [string]$Version, [hashtable]$VersionMap = @{})

    $hasAny = $false
    foreach ($suffix in @("", "1", "2", "3", "4", "5")) {
        $from = $El.GetAttribute("FromVersion$suffix")
        $to   = $El.GetAttribute("ToVersion$suffix")
        if ([string]::IsNullOrEmpty($from) -and [string]::IsNullOrEmpty($to)) { continue }
        $hasAny = $true

        $ver = if ($VersionMap -and $VersionMap.ContainsKey($suffix) -and $VersionMap[$suffix]) { $VersionMap[$suffix] } else { $Version }

        $okFrom = [string]::IsNullOrEmpty($from) -or ((Compare-Versions $ver $from) -ge 0)
        $okTo   = [string]::IsNullOrEmpty($to)   -or ((Compare-Versions $ver $to)   -le 0)
        if ($okFrom -and $okTo) { return $true }
    }
    if (-not $hasAny) { return $true }
    return $false
}

#endregion

#region Markup assembly

# Names of <Include> references that could not be resolved from disk during the last
# Get-AssembledPropertiesRoot call. These are AS-internal fragments (Macro= references and a
# handful of File= groups such as ArCfgGroupOPC/Communication) that Automation Studio expands
# inside its own engine and does not ship as standalone XML. Reset per assembly.
$script:PgUnresolvedIncludes = [System.Collections.Generic.List[string]]::new()

function Resolve-PgIncludePath {
    <#
    .SYNOPSIS
        Resolves an <Include File=.. [Directory=..]/> path relative to IncludeRoot.
        Returns $null if the file cannot be resolved safely within IncludeRoot.
        Silently drops Macro= references. {VersionN} tokens in Directory are substituted from
        $VersionTokenMap (key = the numeric N, e.g. '3'); a directory that still contains an
        unresolved {Version...} token after substitution is dropped (best-effort).
    #>
    param([string]$IncludeRoot, [string]$Directory, [string]$File, [hashtable]$VersionTokenMap = @{})

    # Skip macro-only references.
    if ([string]::IsNullOrEmpty($File))      { return $null }

    # Substitute {VersionN} tokens (e.g. "mappMotion/{Version3}/Editors") when a map is supplied.
    if (-not [string]::IsNullOrEmpty($Directory) -and $Directory -match '\{Version') {
        if ($VersionTokenMap -and $VersionTokenMap.Count -gt 0) {
            foreach ($m in [regex]::Matches($Directory, '\{Version(\d+)\}')) {
                $n = $m.Groups[1].Value
                if ($VersionTokenMap.ContainsKey($n) -and $VersionTokenMap[$n]) {
                    $Directory = $Directory -replace [regex]::Escape($m.Value), $VersionTokenMap[$n]
                }
            }
        }
        # Any token still unresolved => cannot resolve safely.
        if ($Directory -match '\{Version') { return $null }
    }

    $parts = @()
    if (-not [string]::IsNullOrEmpty($Directory)) { $parts += ($Directory -split '[\\/]') }
    $parts += ($File -split '[\\/]')

    $p = $IncludeRoot
    foreach ($seg in $parts) {
        if ([string]::IsNullOrEmpty($seg)) { continue }
        $p = Join-Path $p $seg
    }

    # Add .xml extension if missing.
    if ([System.IO.Path]::GetExtension($p) -eq '') { $p = "$p.xml" }

    try { $full = [System.IO.Path]::GetFullPath($p) } catch { return $null }

    # Safety: must remain inside IncludeRoot.
    $rootFull = [System.IO.Path]::GetFullPath($IncludeRoot).TrimEnd([char[]]@('\', '/')) + [System.IO.Path]::DirectorySeparatorChar
    if (-not $full.StartsWith($rootFull, [System.StringComparison]::OrdinalIgnoreCase)) { return $null }
    if (-not (Test-Path $full -PathType Leaf)) { return $null }
    return $full
}

function Expand-PgIncludes {
    <#
    .SYNOPSIS
        Recursively replaces every <Include> element under $Node with the element children of
        the referenced file's document root. Resolved relative to $IncludeRoot; version-filtered.
        A path stack guards against cyclic includes.
        Macro= references are first looked up in $MacroMap (in-document <Macro> definitions from
        the same .hwx file); only if absent there are they treated as AS-internal (unresolvable).
    #>
    param(
        [System.Xml.XmlElement]$Node,
        [string]$IncludeRoot,
        [string]$Version,
        [System.Collections.Generic.HashSet[string]]$Stack,
        [hashtable]$MacroMap = @{},   # ID -> XmlElement map of <Macro> nodes in the source document
        [hashtable]$VersionMap = @{},      # per-dimension version for FromVersion<N>/ToVersion<N> filtering
        [hashtable]$VersionTokenMap = @{}  # N -> version for {VersionN} substitution in Include Directory
    )

    $doc = $Node.OwnerDocument
    foreach ($child in @($Node.ChildNodes)) {
        if ($child.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }

        if ($child.LocalName -eq 'Include') {
            # Drop includes that do not apply to the requested version.
            if (-not (Test-PgVersionApplies $child $Version $VersionMap)) { [void]$Node.RemoveChild($child); continue }

            $macro = $child.GetAttribute('Macro')
            if (-not [string]::IsNullOrEmpty($macro)) {
                if ($MacroMap.ContainsKey($macro)) {
                    # In-document macro: splice its children in place of the <Include>.
                    $macroNode = $MacroMap[$macro]
                    foreach ($imp in @($macroNode.ChildNodes)) {
                        if ($imp.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
                        $imported = $doc.ImportNode($imp, $true)
                        [void]$Node.InsertBefore($imported, $child)
                        # Expand any includes within the imported macro content.
                        Expand-PgIncludes -Node $imported -IncludeRoot $IncludeRoot -Version $Version -Stack $Stack -MacroMap $MacroMap -VersionMap $VersionMap -VersionTokenMap $VersionTokenMap
                    }
                    [void]$Node.RemoveChild($child)
                }
                else {
                    # AS-internal macro reference (not on disk). Track and drop.
                    [void]$script:PgUnresolvedIncludes.Add("Macro:$macro")
                    [void]$Node.RemoveChild($child)
                }
                continue
            }

            $file = $child.GetAttribute('File')
            $dir  = $child.GetAttribute('Directory')
            $target = Resolve-PgIncludePath -IncludeRoot $IncludeRoot -Directory $dir -File $file -VersionTokenMap $VersionTokenMap

            if (-not $target) {
                if (-not [string]::IsNullOrEmpty($file)) {
                    # File reference that does not exist on disk (AS-internal group). Track and drop.
                    [void]$script:PgUnresolvedIncludes.Add("File:$file")
                }
                [void]$Node.RemoveChild($child)
                continue
            }
            if ($Stack.Contains($target)) {
                # Cyclic include — silently skip (already expanded higher in the stack).
                [void]$Node.RemoveChild($child)
                continue
            }

            $incDoc  = Read-PgXmlFile $target
            $incRoot = $incDoc.DocumentElement
            [void]$Stack.Add($target)
            Expand-PgIncludes -Node $incRoot -IncludeRoot $IncludeRoot -Version $Version -Stack $Stack -MacroMap $MacroMap -VersionMap $VersionMap -VersionTokenMap $VersionTokenMap
            [void]$Stack.Remove($target)

            foreach ($imp in @($incRoot.ChildNodes)) {
                if ($imp.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
                $node2 = $doc.ImportNode($imp, $true)
                [void]$Node.InsertBefore($node2, $child)
            }
            [void]$Node.RemoveChild($child)
        }
        else {
            Expand-PgIncludes -Node $child -IncludeRoot $IncludeRoot -Version $Version -Stack $Stack -MacroMap $MacroMap -VersionMap $VersionMap -VersionTokenMap $VersionTokenMap
        }
    }
}

function Expand-PgTemplates {
    <#
    .SYNOPSIS
        Expands every node that carries a Template="ID" attribute by merging the referenced
        <Template> definition's attributes (as defaults) and prepending its child elements.
        Optionally accepts a pre-seeded $TemplateMap (hashtable ID->cloned-XmlElement) for
        globally-defined templates (e.g. from AS/Hardware/ArCfgTemplates*.xml).
    #>
    param(
        [System.Xml.XmlDocument]$Doc,
        [hashtable]$TemplateMap = @{}
    )

    # Collect template definitions from the document itself; clone and remove.
    $map = @{}
    # Start with pre-seeded global templates (lower priority than inline).
    foreach ($k in $TemplateMap.Keys) { $map[$k] = $TemplateMap[$k] }

    foreach ($t in @($Doc.SelectNodes("//Template"))) {
        $id = $t.GetAttribute('ID')
        if (-not [string]::IsNullOrEmpty($id) -and -not $map.ContainsKey($id)) {
            $map[$id] = $t.CloneNode($true)
        }
    }
    foreach ($t in @($Doc.SelectNodes("//Template"))) { [void]$t.ParentNode.RemoveChild($t) }
    if ($map.Count -eq 0) { return }

    # Iteratively expand Template= references (clones may reference other templates).
    $maxIterations = 200
    for ($i = 0; $i -lt $maxIterations; $i++) {
        $refs = @($Doc.SelectNodes("//*[@Template]"))
        if ($refs.Count -eq 0) { break }

        foreach ($n in $refs) {
            $tid = $n.GetAttribute('Template')
            $n.RemoveAttribute('Template')
            if (-not $map.ContainsKey($tid)) {
                Write-Host "  [warn] Template not found: '$tid'" -ForegroundColor DarkYellow
                continue
            }
            $tpl = $map[$tid]

            # Merge template attributes as defaults (instance attributes win; skip ID/TemplateType).
            foreach ($attr in $tpl.Attributes) {
                if ($attr.Name -eq 'ID' -or $attr.Name -eq 'TemplateType') { continue }
                if (-not $n.HasAttribute($attr.Name)) { [void]$n.SetAttribute($attr.Name, $attr.Value) }
            }
            # Prepend template child elements before existing children.
            # Use ImportNode so template nodes from external documents are adopted correctly.
            $targetDoc = $n.OwnerDocument
            $refChild = $n.FirstChild
            foreach ($c in $tpl.ChildNodes) {
                if ($c.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
                $clone = $c.CloneNode($true)
                if ($clone.OwnerDocument -ne $targetDoc) {
                    $clone = $targetDoc.ImportNode($clone, $true)
                }
                [void]$n.InsertBefore($clone, $refChild)
            }
        }
    }
}

function Remove-PgOutOfVersionNodes {
    <#
    .SYNOPSIS
        Removes any element (and its subtree) whose version attributes exclude $Version.
    #>
    param([System.Xml.XmlElement]$Node, [string]$Version, [hashtable]$VersionMap = @{})

    foreach ($child in @($Node.ChildNodes)) {
        if ($child.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
        if ((Test-PgNodeHasVersionAttrs $child) -and (-not (Test-PgVersionApplies $child $Version $VersionMap))) {
            [void]$Node.RemoveChild($child)
            continue
        }
        Remove-PgOutOfVersionNodes -Node $child -Version $Version -VersionMap $VersionMap
    }
}

function New-PgGlobalTemplateMap {
    <#
    .SYNOPSIS
        Builds a hashtable of <Template ID -> cloned-XmlElement> by loading all ArCfgTemplates*.xml
        (or any other supplied template files) from the given include root.
        Used to pre-seed Expand-PgTemplates for hardware modules whose Properties blocks reference
        Template="OnOffText" etc. without an explicit <Include> for those definitions.
    #>
    param(
        [string]$IncludeRoot,
        [string[]]$ExtraTemplateFiles = @()
    )

    $map = @{}
    $files = @()

    # Search the IncludeRoot itself (top-level only) for ArCfgTemplates*.xml template fragments.
    if (Test-Path $IncludeRoot -PathType Container) {
        $files += @(Get-ChildItem -Path $IncludeRoot -Filter 'ArCfgTemplates*.xml' -File -ErrorAction SilentlyContinue | Select-Object -ExpandProperty FullName)
    }
    $files += $ExtraTemplateFiles | Where-Object { Test-Path $_ -PathType Leaf }

    foreach ($f in ($files | Select-Object -Unique)) {
        try {
            $doc = Read-PgXmlFile $f
            foreach ($t in $doc.SelectNodes("//Template")) {
                $id = $t.GetAttribute('ID')
                if (-not [string]::IsNullOrEmpty($id) -and -not $map.ContainsKey($id)) {
                    $map[$id] = $t.CloneNode($true)
                }
            }
        }
        catch { Write-Host "  [warn] Could not load template file '$f': $_" -ForegroundColor DarkYellow }
    }

    return $map
}

function Get-AssembledPropertiesRoot {
    <#
    .SYNOPSIS
        Takes an already-loaded entry node ($EntryNode, e.g. a <Properties> or <Configuration>
        element) and returns it fully assembled: includes spliced, templates expanded (with
        optional pre-seeded global template map), and out-of-version nodes removed.

        For the tp-config path, $EntryNode is loaded from a .xml file via Read-PgXmlFile.
        For the hw-api path, $EntryNode is an in-memory element cloned from the .hwx.
    #>
    param(
        [System.Xml.XmlElement]$EntryNode,
        [string]$IncludeRoot,
        [string]$Version,
        [hashtable]$TemplateMap = @{},
        [string]$EntryFilePath = "",   # supplied only when EntryNode came from a file (for cycle guard)
        [hashtable]$MacroMap   = @{},   # in-document <Macro ID="..."> definitions (e.g. from .hwx panel modules)
        [hashtable]$VersionMap = @{},      # per-dimension version for FromVersion<N>/ToVersion<N> filtering
        [hashtable]$VersionTokenMap = @{}  # N -> version for {VersionN} substitution in Include Directory
    )

    $stack = New-Object 'System.Collections.Generic.HashSet[string]'
    if (-not [string]::IsNullOrEmpty($EntryFilePath)) {
        [void]$stack.Add(([System.IO.Path]::GetFullPath($EntryFilePath)))
    }

    # Reset the unresolved-include tracker for this assembly.
    $script:PgUnresolvedIncludes = [System.Collections.Generic.List[string]]::new()

    Expand-PgIncludes -Node $EntryNode -IncludeRoot $IncludeRoot -Version $Version -Stack $stack -MacroMap $MacroMap -VersionMap $VersionMap -VersionTokenMap $VersionTokenMap
    Expand-PgTemplates -Doc $EntryNode.OwnerDocument -TemplateMap $TemplateMap
    Remove-PgOutOfVersionNodes -Node $EntryNode -Version $Version -VersionMap $VersionMap
    return $EntryNode
}

function Get-PgUnresolvedIncludes {
    <#
    .SYNOPSIS
        Returns the unique list of <Include> references that could not be resolved from disk
        during the most recent Get-AssembledPropertiesRoot call. These are AS-internal fragments
        (Macro= references and a few File= groups like ArCfgGroupOPC/Communication/OPCUA) that
        Automation Studio expands inside its own engine and does not ship as standalone XML.
    #>
    return @($script:PgUnresolvedIncludes | Select-Object -Unique)
}

#endregion

#region Element model (-> JSON)

# Container element kinds. 'Property' is the tp-config leaf vocabulary; 'Parameter' is the
# hardware-module (.hwx / ArCfg) leaf vocabulary. Both are included so the same walker handles
# config editor markup and hardware Properties blocks. (tp-config editor files contain no
# <Parameter> nodes, so including it here does not change tp-config output.)
$script:PgContainerKinds = @('Group', 'Property', 'Parameter', 'Selector', 'Struct')

# Traversal budget / depth state — reset by Get-PropertyGridElements before each call.
$script:PgDepthLimit   = 1
$script:PgElementBudget = 0
$script:PgEmittedCount  = 0
$script:PgTruncated     = $false

function Measure-PgContainerChildren {
    param([System.Xml.XmlElement]$El)
    $n = 0
    foreach ($c in $El.ChildNodes) {
        if ($c.NodeType -eq [System.Xml.XmlNodeType]::Element -and $script:PgContainerKinds -contains $c.LocalName) { $n++ }
    }
    return $n
}

function Measure-PgSelections {
    param([System.Xml.XmlElement]$El)
    $n = 0
    foreach ($c in $El.ChildNodes) {
        if ($c.NodeType -eq [System.Xml.XmlNodeType]::Element -and $c.LocalName -eq 'Selection') { $n++ }
    }
    return $n
}

function Get-PgFlagAttributes {
    <#
    .SYNOPSIS
        Returns an ordered hashtable of boolean flag attributes (camelCased).
        Captures Is* attributes, plus Visible, CanEdit, Validated, ArConfigSkip.
    #>
    param([System.Xml.XmlElement]$El)
    $flags = [ordered]@{}

    # Is* boolean attributes
    foreach ($attr in $El.Attributes) {
        if ($attr.Name -match '^Is[A-Z]' -and $attr.Value -match '^(?i:true|false)$') {
            $key = $attr.Name.Substring(0, 1).ToLower() + $attr.Name.Substring(1)
            $flags[$key] = [bool]($attr.Value -match '^(?i:true)$')
        }
    }

    # Additional editor-hint booleans
    foreach ($pair in @(
            @('Visible',      'visible'),
            @('CanEdit',      'canEdit'),
            @('Validated',    'validated'),
            @('ArAsHwAc',     'arAsHwAc'),
            @('ArConfigSkip', 'arConfigSkip'))) {
        $raw = $El.GetAttribute($pair[0])
        if (-not [string]::IsNullOrEmpty($raw) -and $raw -match '^(?i:true|false|on|off|0|1)$') {
            $flags[$pair[1]] = $raw -match '^(?i:true|on|1)$'
        }
    }

    if ($flags.Count -gt 0) { return $flags }
    return $null
}

function Get-PgChildModels {
    param([System.Xml.XmlElement]$El, [int]$ChildDepth)
    $children = New-Object System.Collections.Generic.List[object]
    foreach ($c in $El.ChildNodes) {
        if ($c.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
        if ($script:PgContainerKinds -contains $c.LocalName) {
            $children.Add((ConvertTo-PgElementModel $c $ChildDepth))
        }
    }
    return $children
}

function Get-PgLocationPath {
    <#
    .SYNOPSIS
        Computes the AS "Location" path for a node that lives inside a sub-config namespace
        (an ancestor <Group ArAsHwAc="true">, e.g. ACOPOS DriveConfiguration). The path is the
        "/"-joined chain of ancestor Group/Selector/Struct IDs from the namespace root down to
        and including the node's immediate parent. <Selection> ancestors are skipped (a Selector
        contributes a path segment, its chosen Selection does not). Returns $null when the node
        has no ArAsHwAc ancestor (ordinary grid nodes), so 'location' is emitted ONLY for nodes
        that must be authored with a Location attribute in Hardware.hw.

        Example: <Property ID="Position"> under .../Homing/<Selector Mode>/<Selection AbsoluteSwitch>
        yields "DriveConfiguration/Channel[1]/RealAxis/Homing/Mode".
    #>
    param([System.Xml.XmlElement]$El)

    $segments  = New-Object System.Collections.Generic.List[string]
    $foundRoot = $false
    $cur = $El.ParentNode
    while ($cur -and $cur.NodeType -eq [System.Xml.XmlNodeType]::Element) {
        if (@('Group', 'Selector', 'Struct') -contains $cur.LocalName) {
            $cid = $cur.GetAttribute('ID')
            if (-not [string]::IsNullOrEmpty($cid)) { $segments.Insert(0, $cid) }
        }
        if (($cur.GetAttribute('ArAsHwAc')) -match '^(?i:true)$') { $foundRoot = $true; break }
        $cur = $cur.ParentNode
    }
    if (-not $foundRoot -or $segments.Count -eq 0) { return $null }
    return ($segments -join '/')
}

function ConvertTo-PgElementModel {
    <#
    .SYNOPSIS
        Recursively converts an assembled property-grid element into an ordered hashtable.
        Mirrors the tp-config output shape exactly: kind, id, name, description, data fields,
        children/selections (or stubs with hasChildren/childCount/selectionCount).
    #>
    param([System.Xml.XmlElement]$El, [int]$CurrentDepth = 1)

    $script:PgEmittedCount++
    $canDescend = ($script:PgDepthLimit -lt 0) -or ($CurrentDepth -lt $script:PgDepthLimit)
    $budgetLeft = ($script:PgElementBudget -le 0) -or ($script:PgEmittedCount -lt $script:PgElementBudget)

    $m = [ordered]@{}
    $m.kind = $El.LocalName
    $id   = Get-PgAttr $El 'ID'
    if ($id) {
        $m.id = $id
        # Dynamic/array nodes carry a "[%I]" placeholder in their ID. Emit a concrete,
        # 1-based example so the consumer writes Segment[1], Segment[2], ... and never the
        # literal "[%I]". The raw 'id' is kept verbatim so -ParentElement drill-down still works.
        if ($id -match '\[%I\]') { $m.idExample = ($id -replace '\[%I\]', '[1]') }
    }
    $name = Get-PgLocalized $El 'Name'; if ($name) { $m.name = $name }
    $desc = Get-PgLocalized $El 'Description'; if ($desc) { $m.description = $desc }

    # Location path: emitted only for nodes inside an ArAsHwAc sub-config namespace (e.g. ACOPOS
    # DriveConfiguration). Its presence tells the consumer this node MUST be authored with a
    # Location="..." attribute in Hardware.hw; nodes without it flatten without Location. A
    # <Selection> never carries its own location (it is not authored as a tag).
    if ($El.LocalName -ne 'Selection') {
        $loc = Get-PgLocationPath $El
        if ($loc) { $m.location = $loc }
    }

    # Cardinality / dynamic behavior.
    $dyn  = Get-PgAttr $El 'DynamicMode'; if ($dyn)  { $m.dynamicMode = $dyn }
    $minC = Get-PgAttr $El 'MinCount';    if ($minC) { $m.minCount = [int]$minC }
    $maxC = Get-PgAttr $El 'MaxCount';    if ($maxC) { $m.maxCount = [int]$maxC }

    switch ($El.LocalName) {
        'Parameter' {
            # Hardware modules use <Parameter> like tp-config uses <Property>; emit identically.
            $dt  = Get-PgAttr $El 'Type';  if (-not $dt)  { $dt  = Get-PgAttr $El 'DataType' }; if ($dt)  { $m.dataType = $dt }
            $val = Get-PgAttr $El 'Value'; if (-not $val) { $val = Get-PgAttr $El 'Value-en' }; if ($null -ne $val) { $m.value = $val }
            # Writability: a Parameter is read-only when the editor forbids editing (CanEdit=False)
            # or when AS computes the value itself (IsCalculated=True). 'editable' is the single
            # signal an agent needs - only author/override nodes where editable is true.
            $m.editable = -not (((Get-PgAttr $El 'CanEdit') -match '^(?i:false)$') -or ((Get-PgAttr $El 'IsCalculated') -match '^(?i:true)$'))
            $vr  = Get-PgAttr $El 'Range'; if (-not $vr)  { $vr  = Get-PgAttr $El 'ValueRange' }; if ($vr) { $m.valueRange = $vr }

            $cb = $El.SelectSingleNode('ComboBox')
            if ($cb) {
                $items = New-Object System.Collections.Generic.List[object]
                foreach ($it in $cb.ChildNodes) {
                    if ($it.NodeType -ne [System.Xml.XmlNodeType]::Element -or $it.LocalName -ne 'Item') { continue }
                    $im = [ordered]@{ id = $it.GetAttribute('ID') }
                    $iname = Get-PgLocalized $it 'Name'; if ($iname) { $im.name = $iname }
                    $items.Add($im)
                }
                # 'valueFromList' tells the consumer the Value attribute MUST be one of
                # valueList[].id (the id, never its display name).
                if ($items.Count -gt 0) { $m.valueList = $items; $m.valueFromList = $true }
            }
        }
        'Property' {
            $dt  = Get-PgAttr $El 'DataType'; if ($dt)   { $m.dataType = $dt }
            $val = Get-PgAttr $El 'Value';    if (-not $val) { $val = Get-PgAttr $El 'Value-en' }
            if ($null -ne $val) { $m.value = $val }
            # Writability: a Property is read-only when IsReadOnly=True. 'editable' is the single
            # signal an agent needs - only author/override nodes where editable is true.
            $m.editable = -not ((Get-PgAttr $El 'IsReadOnly') -match '^(?i:true)$')
            $vr  = Get-PgAttr $El 'ValueRange'; if ($vr) { $m.valueRange = $vr }
            $unit = Get-PgLocalized $El 'Unit'; if ($unit) { $m.unit = $unit }
            $nf  = Get-PgAttr $El 'NumberFormat'; if ($nf) { $m.numberFormat = $nf }
            $re  = Get-PgAttr $El 'RegularExpression'; if ($re) { $m.regularExpression = $re }

            foreach ($pair in @(
                    @('SelectionType', 'selectionType'),
                    @('SelectionSubType', 'selectionSubType'),
                    @('SelectionFileExtension', 'selectionFileExtension'),
                    @('SelectionFolderName', 'selectionFolderName'),
                    @('SelectionValue', 'selectionValue'))) {
                $av = Get-PgAttr $El $pair[0]; if ($av) { $m[$pair[1]] = $av }
            }
            # A ConfigurationElement property is a cross-reference: its Value must be the ID of an
            # existing object whose type matches 'selectionSubType', not a free literal.
            if ($m.Contains('selectionType') -and $m.selectionType -eq 'ConfigurationElement') {
                $m.valueIsReference = $true
            }

            $vl = $El.SelectSingleNode('ValueList')
            if ($vl) {
                $items = New-Object System.Collections.Generic.List[object]
                foreach ($it in $vl.ChildNodes) {
                    if ($it.NodeType -ne [System.Xml.XmlNodeType]::Element -or $it.LocalName -ne 'Item') { continue }
                    $im = [ordered]@{ id = $it.GetAttribute('ID') }
                    $iname = Get-PgLocalized $it 'Name'; if ($iname) { $im.name = $iname }
                    $idesc = Get-PgLocalized $it 'Description'; if ($idesc) { $im.description = $idesc }
                    $items.Add($im)
                }
                $m.valueList = $items
                $editable = (Get-PgAttr $vl 'IsEditable') -match '^(?i:true)$'
                if ($editable) { $m.valueListEditable = $true }
                # Enumerated, non-editable list => Value MUST be one of valueList[].id.
                if ($items.Count -gt 0 -and -not $editable) { $m.valueFromList = $true }
            }
        }
        'Selector' {
            $def = Get-PgAttr $El 'Value'; if ($def) { $m.default = $def }
            if ($canDescend -and $budgetLeft) {
                $selections = New-Object System.Collections.Generic.List[object]
                foreach ($s in $El.ChildNodes) {
                    if ($s.NodeType -ne [System.Xml.XmlNodeType]::Element -or $s.LocalName -ne 'Selection') { continue }
                    $selections.Add((ConvertTo-PgElementModel $s ($CurrentDepth + 1)))
                }
                $m.selections = $selections
            }
            else {
                $sc = Measure-PgSelections $El
                if ($sc -gt 0) {
                    $m.selectionCount = $sc
                    $m.hasChildren    = $true
                    if (-not $budgetLeft) { $script:PgTruncated = $true }
                }
            }
        }
        { $_ -in @('Group', 'Struct', 'Selection') } {
            if ($canDescend -and $budgetLeft) {
                $children = Get-PgChildModels $El ($CurrentDepth + 1)
                if ($children.Count -gt 0) { $m.children = $children }
            }
            else {
                $cc = Measure-PgContainerChildren $El
                if ($cc -gt 0) {
                    $m.childCount  = $cc
                    $m.hasChildren = $true
                    if (-not $budgetLeft) { $script:PgTruncated = $true }
                }
            }
        }
    }

    $flags = Get-PgFlagAttributes $El
    if ($flags) { $m.flags = $flags }

    return $m
}

#endregion

#region XML authoring guide

function Get-XmlAuthoringGuide {
    <#
    .SYNOPSIS
        Builds the embedded "xmlAuthoring" legend that travels with every GetConfigElements /
        GetModuleProperties response. It makes the JSON self-describing: it states the file/element
        wrapper the elements[] tree lives inside, and the exact JSON->XML rules (kind=tag, id=ID,
        value=Value, valueFromList, [%I] arrays, Selector/Selection, ConfigurationElement refs) so a
        consumer never has to consult an external mapping document.

    .PARAMETER Profile
        'tp-config' : mapp technology-package config files (<Configuration><Element ID Type>...).
        'hardware'  : a hardware module's config block in Hardware.hw (<Module ...>...).

    .PARAMETER TypeName
        tp-config only: the config Element Type (file ending without the dot, e.g. 'layout6d').

    .PARAMETER ModuleType / ModuleVersion
        hardware only: used to render a concrete <Module> wrapper.
    #>
    param(
        [ValidateSet('tp-config', 'hardware')] [string]$Profile,
        [string]$TypeName      = '',
        [string]$ModuleType    = '',
        [string]$ModuleVersion = ''
    )

    $g = [ordered]@{}
    if ($Profile -eq 'tp-config') {
        $tn = if ($TypeName) { $TypeName } else { '<type>' }
        $g.wrapper   = "<?xml version=`"1.0`" encoding=`"utf-8`"?>`n<?AutomationStudio FileVersion=`"4.9`"?>`n<Configuration>`n  <Element ID=`"<unique-id>`" Type=`"$tn`">`n    <!-- transcribe each elements[] node here -->`n  </Element>`n</Configuration>"
        $g.element   = "Each node's 'kind' is the XML tag and 'id' is its ID attribute: <{kind} ID=`"{id}`" ... />. Nest a Group/Struct node's children[] inside it. 'name' and 'description' are documentation only - never write them into the XML."
        $g.value     = "On a Property, put 'value' into a Value attribute. When 'valueFromList' is true, Value MUST be one of valueList[].id (the id, never its name)."
        $g.editable  = "Author/override ONLY nodes where 'editable' is true. editable=false means the value is read-only or computed by AS - do not emit the node or set its Value. Hidden nodes (flags.isInvisible=true) are advanced/internal; leave them at their default unless explicitly required."
        $g.arrays    = "An 'id' containing [%I] is an array template (see 'idExample'): emit one node per instance, replacing %I with a 1-based index - Segment[1], Segment[2], ... - honoring minCount/maxCount."
        $g.selector  = "A Selector becomes <Selector ID=`"{id}`" Value=`"<one selection id>`">; include ONLY the chosen selection's children inside it. A Selection is never its own tag - its id is just a possible Selector Value ('default' marks the default selection)."
        $g.reference = "When 'valueIsReference' is true (selectionType=ConfigurationElement), Value must be the ID of an existing object whose type matches 'selectionSubType', defined elsewhere in the project."
    }
    else {
        $mt  = if ($ModuleType)    { $ModuleType }    else { '<module-type>' }
        $ver = if ($ModuleVersion) { $ModuleVersion } else { '<version>' }
        $g.wrapper   = "<Module Name=`"<instance-name>`" Type=`"$mt`" Version=`"$ver`">`n  <!-- transcribe each elements[] node here (FLATTENED, see 'element') -->`n</Module>"
        $g.element   = "FLATTENED authoring - the 'kind' is NOT reused as the tag. Each editable leaf (kind 'Parameter' OR 'Property') is written as <Parameter ID=`"{id}`" Value=`"...`" /> directly under <Module>. 'Group'/'Struct'/'Selection' wrappers are NOT reproduced. 'name'/'description' are documentation only."
        $g.value     = "On a leaf, put 'value' into a Value attribute. When 'valueFromList' is true, Value MUST be one of valueList[].id (the id, never its name)."
        $g.editable  = "Author/override ONLY nodes where 'editable' is true. editable=false means read-only or computed by AS - do not emit the node or set its Value. Hidden nodes (flags.isInvisible=true or flags.visible=false) are advanced/internal; leave them at their default unless explicitly required."
        $g.arrays    = "An 'id' with a placeholder ([%I] -> see 'idExample', or %R / %../R) is an array template: replace the placeholder with a 1-based index in the written id (FileDevice1, FileDeviceName1, ...), honoring minCount/maxCount. A dynamic array-element Group is emitted as an empty <Group ID=`"FileDevice1`" /> marker; its members are flat <Parameter> siblings."
        $g.selector  = "A configured Selector becomes <Parameter ID=`"{id}`" Value=`"<one selection id>`" /> ('default' marks the default selection id); then emit that selection's editable children as their own flat <Parameter> entries. A Selection is never its own tag."
        $g.location  = "When a node carries a 'location' field, it lives in a Technology-Package sub-config namespace and MUST be authored with that exact path as a Location attribute: <Parameter ID=`"{id}`" Location=`"{location}`" Value=`"...`" /> (e.g. location=DriveConfiguration/Channel[1]/RealAxis/Homing/Mode). A Selector contributes a path segment, its chosen Selection does not. Nodes WITHOUT a 'location' are written without a Location attribute. The namespace root is the Group with flags.arAsHwAc=true; drill in with -ParentElement."
    }
    return $g
}

#endregion

#region Navigation

function Find-PgScopeNode {
    <#
    .SYNOPSIS
        Resolves $ParentElement to a node within the assembled tree. Returns the root element
        for Root/empty. Walks the "/"-delimited path of node IDs, descending into Selection
        when the current node is a Selector. Throws a descriptive error listing available IDs.
    #>
    param([System.Xml.XmlElement]$Root, [string]$ParentElement)

    if ([string]::IsNullOrWhiteSpace($ParentElement) -or $ParentElement -ieq 'Root' -or $ParentElement -ieq 'none') {
        return $Root
    }

    $segments  = $ParentElement -split '/' | Where-Object { $_ -ne '' }
    $current   = $Root
    $traversed = New-Object System.Collections.Generic.List[string]

    foreach ($seg in $segments) {
        $candidates = @()
        if ($current.LocalName -eq 'Selector') {
            $candidates = @($current.ChildNodes | Where-Object {
                    $_.NodeType -eq [System.Xml.XmlNodeType]::Element -and $_.LocalName -eq 'Selection'
                })
        }
        else {
            $candidates = @($current.ChildNodes | Where-Object {
                    $_.NodeType -eq [System.Xml.XmlNodeType]::Element -and
                    ($script:PgContainerKinds -contains $_.LocalName -or $_.LocalName -eq 'Selection')
                })
        }

        $next = $candidates | Where-Object { $_.GetAttribute('ID') -ieq $seg } | Select-Object -First 1
        if (-not $next) {
            $available = ($candidates | ForEach-Object { $_.GetAttribute('ID') } | Where-Object { $_ } | Select-Object -Unique) -join ', '
            $where = if ($traversed.Count -gt 0) { ($traversed -join '/') } else { 'Root' }
            throw "ParentElement segment '$seg' not found under '$where'. Available IDs: $available"
        }
        $traversed.Add($seg)
        $current = $next
    }
    return $current
}

#endregion

#region Main walk entry point

function Get-PropertyGridElements {
    <#
    .SYNOPSIS
        Walks the assembled property-grid tree rooted at $Root, navigates to $ScopeNode
        (already resolved by Find-PgScopeNode), and returns a hashtable:
          @{ elements; emittedCount; truncated; hint }
        compatible with the JSON output shape of both as-tp-config-api and as-physical-hw-api.

    .PARAMETER Root
        The assembled <Properties> or <Configuration> root element.

    .PARAMETER ScopeNode
        The element to treat as the scope (from Find-PgScopeNode). Pass $Root for the full tree.

    .PARAMETER ParentElement
        The raw -ParentElement string (used to build the hint message only).

    .PARAMETER MaxDepth
        Levels to expand below the scope. -1 = unlimited. Default 1.

    .PARAMETER MaxElements
        Total node budget; 0 = no cap. Default 500.
    #>
    param(
        [System.Xml.XmlElement]$Root,
        [System.Xml.XmlElement]$ScopeNode,
        [string]$ParentElement = 'Root',
        [int]$MaxDepth    = 1,
        [int]$MaxElements = 500
    )

    # Reset traversal state.
    $script:PgDepthLimit    = $MaxDepth
    $script:PgElementBudget = $MaxElements
    $script:PgEmittedCount  = 0
    $script:PgTruncated     = $false

    $elements = New-Object System.Collections.Generic.List[object]
    if ($ScopeNode -eq $Root) {
        # Top-level: each container child of Root is depth=1.
        foreach ($c in $Root.ChildNodes) {
            if ($c.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
            if ($script:PgContainerKinds -contains $c.LocalName) {
                $elements.Add((ConvertTo-PgElementModel $c 1))
            }
        }
    }
    else {
        # Scoped: the requested node itself is depth=0, its children are depth=1.
        $elements.Add((ConvertTo-PgElementModel $ScopeNode 0))
    }

    $out = @{
        elements     = $elements
        emittedCount = $script:PgEmittedCount
        truncated    = $script:PgTruncated
        hint         = $null
    }
    if ($script:PgTruncated) {
        $out.hint = "Output reached the MaxElements budget ($MaxElements). Drill into a specific node via -ParentElement <id-path>, or raise -MaxElements / -MaxDepth."
    }
    return $out
}

#endregion
