<#
.SYNOPSIS
    Discover which XML elements/subelements are allowed inside a B&R Automation Studio
    technology-package configuration file (e.g. the files under Physical/<cfg>/<cpu>/mapp*).

.DESCRIPTION
    Automation Studio technology packages (mappMotion, mapp6D, mappServices, mappControl,
    mappVision, ...) ship an "Editors" folder containing the META markup that dictates which
    nodes a deployed configuration file may contain. This script re-reads that live markup on
    EVERY call (no caching, no hardcoded node lists) so that markup changes introduced by a
    new mapp version are reflected automatically.

    Resolution chain (all read live from the AS installation):
      1. <AS>\AS\TechnologyPackages\<tp>\<version>\ApplicationSettings\TP.cfg
         -> Element[@Type='TechnologyPackage']/Group[@ID='Files']/Property[@ID^='FileExt']/@Value
         = the authoritative list of valid file endings for that package + version.
      2. <AS>\AS\TechnologyPackages\<tp>\<version>\Editors\<Name>.xml
         = top-level editor markup; the file ending (without leading dot) maps case-insensitively
         to the editor file's base name (.axis -> Axis.xml, .purevaxcfg -> PureVAxCfg.xml,
         .mpalarmxcore -> mpalarmxcore.xml, .layout6d -> Layout6D.xml).
      3. The markup is assembled live: <Include File="..."/> fragments (resolved relative to the
         Editors root) are spliced in, <Template> definitions are expanded into nodes that
         reference them via Template="...", and nodes are filtered by the requested version using
         the FromVersion/ToVersion(+1..5) attributes.

    The deployed configuration file wraps these nodes in <Element ID="..." Type="<ending w/o dot>">.

.PARAMETER TechnologyPackage
    Technology package name, e.g. mappMotion, mapp6D, mappServices, mappControl, mappVision.
    Matched case-insensitively; a lenient fallback also matches loose names (e.g. "6D" -> mapp6D,
    "mappControls" -> mappControl).

.PARAMETER TPVersion
    Technology package version, e.g. 6.6.1. When omitted, the highest installed version is used.

.PARAMETER FileEnding
    The configuration file ending, with or without the leading dot (e.g. ".axis" or "axis").
    Required for Action=GetConfigElements. Validated live against TP.cfg.

.PARAMETER ParentElement
    Which part of the configuration to return.
      - "Root" (default) or empty: the whole configuration (all top-level allowed nodes).
      - A "/"-delimited path of node IDs: returns that element and its applicable subelements.
        Because IDs are not globally unique (dynamic "[%i]" groups, repeated IDs under different
        Selector branches), a full path is required to disambiguate. When the current node on the
        path is a Selector, the next path segment must be one of its Selection IDs.
        Example: BaseType/RotaryPeriodic  or  PureVAxis

.PARAMETER Action
    GetConfigElements (default) : return allowed elements for a file ending.
    ListFileEndings             : return all valid file endings for the package + version,
                                  each mapped to its editor markup file.

.PARAMETER ProjectPath
    Optional path to the AS project directory. Only used to bias AS-installation selection toward
    the project's version when several installations contain the requested package.

.PARAMETER MaxDepth
    How many levels below the requested scope to expand (Action=GetConfigElements only).
    Depth is relative to the requested scope:
      - Root call: the top-level nodes are 1 level below the configuration root.
      - ParentElement call: the requested node is the scope; its children are 1 level below it.
    Default 1 keeps responses small (a list of the scope's contents). Nodes deeper than MaxDepth
    are collapsed to a stub carrying "hasChildren":true and "childCount"/"selectionCount"; drill in
    by calling again with a more specific -ParentElement path. Use -1 for unlimited depth.

.PARAMETER MaxElements
    Safety budget on the total number of element nodes emitted in one call (Action=GetConfigElements
    only). When the budget is reached, remaining containers are collapsed to stubs and the result
    carries "truncated":true, "emittedCount", and a "hint". Default 500. Use 0 (or negative) for no cap.

.EXAMPLE
    .\use-as-tp-config-api.ps1 -TechnologyPackage mappMotion -TPVersion 6.6.1 -FileEnding .axis

.EXAMPLE
    .\use-as-tp-config-api.ps1 -TechnologyPackage mappMotion -TPVersion 6.6.1 -FileEnding .purevaxcfg -ParentElement PureVAxis

.EXAMPLE
    .\use-as-tp-config-api.ps1 -TechnologyPackage mappServices -Action ListFileEndings
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$TechnologyPackage,

    [Parameter()]
    [string]$TPVersion = "",

    [Parameter()]
    [string]$FileEnding = "",

    [Parameter()]
    [string]$ParentElement = "Root",

    [Parameter()]
    [ValidateSet("GetConfigElements", "ListFileEndings")]
    [string]$Action = "GetConfigElements",

    [Parameter()]
    [string]$ProjectPath = ".",

    [Parameter()]
    [int]$MaxDepth = 1,

    [Parameter()]
    [int]$MaxElements = 500
)

# Dot-source shared utilities (Get-InstalledASVersions, Get-ASContentPath, Compare-Versions, output helpers)
. "$PSScriptRoot/../../as-shared/scripts/as-shared.ps1"
# Dot-source shared property-grid engine (assembly, templates, version filter, drill-down, model)
. "$PSScriptRoot/../../as-shared/scripts/as-propertygrid.ps1"

#region Compatibility aliases Ã¢â‚¬â€ thin wrappers so orchestrators call the Pg-prefixed shared functions
# without needing any code changes below this point.

function Read-XmlFile   { param([string]$Path)                        Read-PgXmlFile   @PSBoundParameters }
function Get-Attr       { param([System.Xml.XmlElement]$El,[string]$Name) Get-PgAttr   @PSBoundParameters }
function Get-Localized  { param([System.Xml.XmlElement]$El,[string]$Base) Get-PgLocalized @PSBoundParameters }

function Find-ScopeNode { param([System.Xml.XmlElement]$Root,[string]$ParentElement) Find-PgScopeNode @PSBoundParameters }

function ConvertTo-ElementModel { param([System.Xml.XmlElement]$El,[int]$CurrentDepth=1) ConvertTo-PgElementModel @PSBoundParameters }

# Depth/budget globals Ã¢â‚¬â€ redirect to the shared Pg-prefixed state
# (script: variables in as-propertygrid.ps1 are visible here because dot-sourcing runs in caller scope)
Set-Variable -Name DepthLimit    -Scope script -Value 1
Set-Variable -Name ElementBudget -Scope script -Value 0
Set-Variable -Name EmittedCount  -Scope script -Value 0
Set-Variable -Name Truncated     -Scope script -Value $false
$script:ContainerKinds = $script:PgContainerKinds

function Get-AssembledMarkup {
    param([string]$EntryFile, [string]$EditorsRoot, [string]$Version)
    $doc  = Read-PgXmlFile $EntryFile
    $root = $doc.DocumentElement
    return Get-AssembledPropertiesRoot -EntryNode $root -IncludeRoot $EditorsRoot `
        -Version $Version -EntryFilePath $EntryFile
}

#endregion

#region XML helpers (STUB Ã¢â‚¬â€ replaced by shared engine; kept for reference)

function Get-Attr-UNUSED {
    <#
    .SYNOPSIS
        Returns an attribute value or $null when missing/empty.
    #>
    param([System.Xml.XmlElement]$El, [string]$Name)
    $v = $El.GetAttribute($Name)
    if ([string]::IsNullOrEmpty($v)) { return $null }
    return $v
}

#endregion Ã¢â‚¬â€ XML helpers stub

#region Version filtering Ã¢â‚¬â€ REMOVED (now in as-propertygrid.ps1) Ã¢â‚¬â€ stub kept for reference

function Test-NodeHasVersionAttrs { param([System.Xml.XmlElement]$El) Test-PgNodeHasVersionAttrs @PSBoundParameters }
function Test-VersionApplies     { param([System.Xml.XmlElement]$El,[string]$Version) Test-PgVersionApplies @PSBoundParameters }

#endregion

#region Markup assembly / Element model / Navigation â€” REMOVED (now in as-propertygrid.ps1)
# Compat alias stubs are defined in the Compatibility aliases region above.
#endregion

#region TP / installation resolution

function ConvertTo-NormalizedTPName {
    param([string]$Name)
    $s = $Name.ToLower()
    $s = $s -replace '^mapp', ''
    $s = $s -replace '[^a-z0-9]', ''
    $s = $s -replace 's$', ''
    return $s
}

function Resolve-TPFolder {
    <#
    .SYNOPSIS
        Resolves a technology-package folder under $TpRoot: exact (case-insensitive) match first,
        then a single lenient normalized match (e.g. "6D" -> mapp6D, "mappControls" -> mappControl).
    #>
    param([string]$TpRoot, [string]$Name)
    $dirs = @(Get-ChildItem -Path $TpRoot -Directory -ErrorAction SilentlyContinue)
    if ($dirs.Count -eq 0) { return $null }

    $exact = $dirs | Where-Object { $_.Name -ieq $Name } | Select-Object -First 1
    if ($exact) { return $exact }

    $target = ConvertTo-NormalizedTPName $Name
    $fuzzy = @($dirs | Where-Object { (ConvertTo-NormalizedTPName $_.Name) -eq $target })
    if ($fuzzy.Count -eq 1) { return $fuzzy[0] }
    return $null
}

function Get-VersionDirs {
    param([string]$TpDir)
    return @(Get-ChildItem -Path $TpDir -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -match '^\d+(\.\d+)*$' })
}

function Resolve-TPInstallation {
    <#
    .SYNOPSIS
        Finds the AS installation + version folder for the requested package.
        Returns @{ ASPath; TPName; Version; VersionPath; DisplayName; InternalName }.
    #>
    param([string]$TechnologyPackage, [string]$TPVersion)

    $installs = @(Get-InstalledASVersions)
    if ($installs.Count -eq 0) { throw "No B&R Automation Studio installations found in the Windows registry." }

    $tpDir = $null
    $asPath = $null
    foreach ($inst in $installs) {
        $tpRoot = Join-Path (Get-ASContentPath $inst.Path) "TechnologyPackages"
        if (-not (Test-Path $tpRoot)) { continue }
        $candidate = Resolve-TPFolder -TpRoot $tpRoot -Name $TechnologyPackage
        if ($candidate) {
            if ($TPVersion) {
                $verPath = Join-Path $candidate.FullName $TPVersion
                if (Test-Path $verPath) { $tpDir = $candidate; $asPath = $inst.Path; break }
                else { continue }  # this install has the TP but not the requested version
            }
            else {
                $tpDir = $candidate; $asPath = $inst.Path; break
            }
        }
    }

    if (-not $tpDir) {
        # Build a helpful list of what is available from the newest installation that has any TPs.
        foreach ($inst in $installs) {
            $tpRoot = Join-Path (Get-ASContentPath $inst.Path) "TechnologyPackages"
            if (-not (Test-Path $tpRoot)) { continue }
            $avail = (Get-ChildItem -Path $tpRoot -Directory -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Name) -join ', '
            if ($TPVersion) {
                throw "Technology package '$TechnologyPackage' version '$TPVersion' not found. Available packages: $avail"
            }
            throw "Technology package '$TechnologyPackage' not found. Available packages: $avail"
        }
        throw "Technology package '$TechnologyPackage' not found and no TechnologyPackages folder exists in any installation."
    }

    $versionDirs = Get-VersionDirs -TpDir $tpDir.FullName
    if ($versionDirs.Count -eq 0) { throw "No installed versions found for technology package '$($tpDir.Name)'." }

    if ($TPVersion) {
        $selected = $versionDirs | Where-Object { $_.Name -eq $TPVersion } | Select-Object -First 1
        if (-not $selected) {
            $vlist = ($versionDirs | Select-Object -ExpandProperty Name) -join ', '
            throw "Version '$TPVersion' not found for '$($tpDir.Name)'. Available versions: $vlist"
        }
    }
    else {
        $selected = $versionDirs | Sort-Object { try { [version]($_.Name) } catch { [version]"0.0" } } -Descending | Select-Object -First 1
    }

    # Read identity from TP.cfg (best effort).
    $displayName = $tpDir.Name
    $internalName = $tpDir.Name
    $tpCfg = Join-Path $selected.FullName "ApplicationSettings\TP.cfg"
    if (Test-Path $tpCfg) {
        try {
            $cfg = Read-XmlFile $tpCfg
            $tpElem = $cfg.SelectSingleNode("/Configuration/Element[@Type='TechnologyPackage']")
            if ($tpElem) {
                $dn = $tpElem.SelectSingleNode("Property[@ID='DisplayName']")
                if ($dn) { $v = Get-Localized $dn 'Value'; if ($v) { $displayName = $v } }
                $inm = $tpElem.SelectSingleNode("Property[@ID='InternalName']")
                if ($inm) { $v = $inm.GetAttribute('Value'); if ($v) { $internalName = $v } }
            }
        }
        catch { }
    }

    return @{
        ASPath       = $asPath
        TPName       = $tpDir.Name
        Version      = $selected.Name
        VersionPath  = $selected.FullName
        DisplayName  = $displayName
        InternalName = $internalName
    }
}

function Get-FileEndings {
    <#
    .SYNOPSIS
        Reads the valid file endings from TP.cfg (Element[@Type='TechnologyPackage']/Group[@ID='Files']).
        Selects by ID prefix 'FileExt' (numbering is not always contiguous).
    #>
    param([string]$VersionPath)
    $tpCfg = Join-Path $VersionPath "ApplicationSettings\TP.cfg"
    if (-not (Test-Path $tpCfg)) { throw "TP.cfg not found at: $tpCfg" }

    $cfg = Read-XmlFile $tpCfg
    $filesGroup = $cfg.SelectSingleNode("/Configuration/Element[@Type='TechnologyPackage']/Group[@ID='Files']")
    if (-not $filesGroup) { return @() }

    $endings = New-Object System.Collections.Generic.List[string]
    foreach ($p in $filesGroup.ChildNodes) {
        if ($p.NodeType -ne [System.Xml.XmlNodeType]::Element -or $p.LocalName -ne 'Property') { continue }
        if ($p.GetAttribute('ID') -notlike 'FileExt*') { continue }
        $v = $p.GetAttribute('Value')
        if (-not [string]::IsNullOrEmpty($v)) { $endings.Add($v) }
    }
    return $endings
}

function Get-EditorFileMap {
    <#
    .SYNOPSIS
        Maps lowercase editor base names to the actual top-level Editors\*.xml file names.
    #>
    param([string]$EditorsDir)
    $map = @{}
    if (-not (Test-Path $EditorsDir)) { return $map }
    foreach ($f in Get-ChildItem -Path $EditorsDir -File -Filter *.xml -ErrorAction SilentlyContinue) {
        $map[$f.BaseName.ToLower()] = $f.Name
    }
    return $map
}

#endregion

#region Actions

function Invoke-ListFileEndings {
    param($Resolved)
    $editorsDir = Join-Path $Resolved.VersionPath "Editors"
    $endings = Get-FileEndings -VersionPath $Resolved.VersionPath

    $list = New-Object System.Collections.Generic.List[object]
    foreach ($e in ($endings | Sort-Object)) {
        $key = $e.TrimStart('.').ToLower()
        $entry = $e
        $list.Add($entry)
    }

    $out = [ordered]@{
        technologyPackage = $Resolved.InternalName
        displayName       = $Resolved.DisplayName
        version           = $Resolved.Version
        fileEndings       = $list
    }
    return $out
}

function Invoke-GetConfigElements {
    param($Resolved, [string]$FileEnding, [string]$ParentElement)

    if ([string]::IsNullOrWhiteSpace($FileEnding)) {
        throw "FileEnding is required for Action=GetConfigElements."
    }

    $ending = "." + ($FileEnding.TrimStart('.').ToLower())
    $editorsDir = Join-Path $Resolved.VersionPath "Editors"

    # Validate the ending against the live TP.cfg list.
    $validEndings = Get-FileEndings -VersionPath $Resolved.VersionPath
    $match = $validEndings | Where-Object { $_ -ieq $ending } | Select-Object -First 1
    if (-not $match) {
        $vlist = (($validEndings | Sort-Object) -join ', ')
        throw "File ending '$ending' is not valid for $($Resolved.InternalName) $($Resolved.Version). Valid endings: $vlist"
    }
    $ending = $match  # canonical casing from TP.cfg

    # Map ending -> top-level editor markup file.
    $editorMap = Get-EditorFileMap -EditorsDir $editorsDir
    $key = $ending.TrimStart('.').ToLower()
    if (-not $editorMap.ContainsKey($key)) {
        throw "No editor markup file found in '$editorsDir' for ending '$ending'."
    }
    $editorFile = $editorMap[$key]
    $entryPath = Join-Path $editorsDir $editorFile

    # Assemble the live markup (includes + templates + version filtering) on every call.
    $root = Get-AssembledMarkup -EntryFile $entryPath -EditorsRoot $editorsDir -Version $Resolved.Version

    # Resolve the requested scope.
    $scope = Find-ScopeNode -Root $root -ParentElement $ParentElement

    $pg = Get-PropertyGridElements -Root $root -ScopeNode $scope -ParentElement $ParentElement `
        -MaxDepth $MaxDepth -MaxElements $MaxElements

    $out = [ordered]@{
        technologyPackage = $Resolved.InternalName
        displayName       = $Resolved.DisplayName
        version           = $Resolved.Version
        fileEnding        = $ending
        parentElement     = $(if ([string]::IsNullOrWhiteSpace($ParentElement)) { 'Root' } else { $ParentElement })
        maxDepth          = $MaxDepth
        xmlAuthoring      = Get-XmlAuthoringGuide -Profile 'tp-config' -TypeName ($ending.TrimStart('.'))
        emittedCount      = $pg.emittedCount
        elements          = $pg.elements
    }
    if ($pg.truncated) {
        $out.truncated = $true
        $out.hint = $pg.hint
    }
    return $out
}

#endregion

#region Output

function Write-Result {
    <#
    .SYNOPSIS
        Serializes $Result as compact JSON to stdout. Compact output drops the ~90% whitespace
        that indented JSON would add, keeping the agent/terminal payload small and token-friendly.
        Uses ConvertTo-AsJson so PowerShell 5.1's \uXXXX escaping of <, >, ', & is undone.
    #>
    param($Result, [int]$Depth = 64)
    Write-Output (ConvertTo-AsJson -InputObject $Result -Depth $Depth)
}

#endregion

#region Main

try {
    $resolved = Resolve-TPInstallation -TechnologyPackage $TechnologyPackage -TPVersion $TPVersion

    switch ($Action) {
        "ListFileEndings" { $result = Invoke-ListFileEndings -Resolved $resolved }
        default { $result = Invoke-GetConfigElements -Resolved $resolved -FileEnding $FileEnding -ParentElement $ParentElement }
    }

    Write-Result -Result $result -Depth 64
    exit 0
}
catch {
    $err = [ordered]@{
        error             = $_.Exception.Message
        technologyPackage = $TechnologyPackage
        version           = $TPVersion
        fileEnding        = $FileEnding
        action            = $Action
    }
    Write-Result -Result $err -Depth 5
    exit 1
}

#endregion
