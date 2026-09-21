<#
.SYNOPSIS
    Manage B&R Automation Studio Physical hardware configuration: add, search, and describe hardware modules.

.DESCRIPTION
    Actions:
    - AddHWModule             : Resolve a hardware module from the AS installation and insert it into Hardware.hw.
    - SearchHWModule          : Search for hardware modules by partial name in the AS installation.
    - SearchByClassification  : Search for hardware modules by a keyword matched against each module's <Classification> Value attributes and top-level Description-en.
    - GetHWModuleInfo         : Read the description and classification from a module's .hwx file.
    - GetIOMappingConfig      : List the permittable I/O channel mappings (ChannelName, DataType, Direction, Description) of a module from its .hwx file.
    - GetPossibleHWConnections: List a module's connectors and routing options (target module/connector, connection types) from its .hwx file.
    - GetModuleProperties          : Browse the configuration properties of a module (same vocabulary as as-tp-config-api: Parameter/Selector/Group/ComboBox).
    - GetPermittedHwClassifications: List all permitted classification Value strings from HardwareCatalogLayout.xml (use before SearchByClassification).

.PARAMETER ProjectPath
    Path to the Automation Studio project directory (containing .apj file).

.PARAMETER Action
    The action to perform: AddHWModule | SearchHWModule | SearchByClassification | GetHWModuleInfo | GetIOMappingConfig | GetPossibleHWConnections | GetModuleProperties | GetPermittedHwClassifications
    Default: AddHWModule

.PARAMETER HwModuleType
    Exact hardware module type name or search pattern.
    Required for AddHWModule, SearchHWModule, GetHWModuleInfo, GetIOMappingConfig, GetPossibleHWConnections, GetModuleProperties.

.PARAMETER ClassificationKeyword
    Keyword matched (case-insensitive substring) against each module's <Classification> Value attributes
    and the module Description-en attribute.
    Required for SearchByClassification.

.PARAMETER HwModuleVersion
    Optional hardware module version for AddHWModule / GetHWModuleInfo / GetIOMappingConfig / GetPossibleHWConnections / GetModuleProperties.
    Highest installed version is selected when omitted.

.PARAMETER HwFileName
    Name of the .hw file to register the hardware module in (optional, default "Hardware.hw").

.PARAMETER Target
    Optional I/O target generation (SG3 | SG4 | SGC) for GetIOMappingConfig and GetModuleProperties.
    When omitted, the target is derived from the active configuration's CPU; falls back to SG4.

.PARAMETER ResolveIncludes
    For GetIOMappingConfig: also resolve shared <Include> channel fragments
    (AS/Hardware/Channels/<File>.xml, e.g. X2X bus diagnostics). Off by default.

.EXAMPLE
    .\use-as-physical-hw-api.ps1 -ProjectPath . -Action AddHWModule -HwModuleType "80VD100PD.C000-01"

.EXAMPLE
    .\use-as-physical-hw-api.ps1 -ProjectPath . -Action AddHWModule -HwModuleType "80VD100PD.C000-01" -HwModuleVersion "2.5.1.0"

.EXAMPLE
    .\use-as-physical-hw-api.ps1 -ProjectPath . -Action SearchHWModule -HwModuleType "8EI"

.EXAMPLE
    .\use-as-physical-hw-api.ps1 -ProjectPath . -Action SearchByClassification -ClassificationKeyword "PPC3100"

.EXAMPLE
    .\use-as-physical-hw-api.ps1 -ProjectPath . -Action GetHWModuleInfo -HwModuleType "X20DI8371"

.EXAMPLE
    .\use-as-physical-hw-api.ps1 -ProjectPath . -Action GetIOMappingConfig -HwModuleType "X20DO2322"

.EXAMPLE
    .\use-as-physical-hw-api.ps1 -ProjectPath . -Action GetPossibleHWConnections -HwModuleType "X20DO2322"
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [ValidateScript({ Test-Path $_ -PathType Container })]
    [string]$ProjectPath,

    [Parameter()]
    [ValidateSet("AddHWModule", "SearchHWModule", "SearchByClassification", "GetHWModuleInfo", "GetIOMappingConfig", "GetPossibleHWConnections", "GetModuleProperties", "GetPermittedHwClassifications")]
    [string]$Action = "AddHWModule",

    [Parameter()]
    [string]$HwModuleType = "",

    [Parameter()]
    [string]$ClassificationKeyword = "",

    [Parameter()]
    [string]$HwModuleVersion = "",

    [Parameter()]
    [string]$HwFileName = "Hardware.hw",

    [Parameter()]
    [ValidateSet("", "SG3", "SG4", "SGC")]
    [string]$Target = "",

    [Parameter()]
    [switch]$ResolveIncludes,

    # GetModuleProperties drill-down parameters
    [Parameter()]
    [string]$ParentElement = "Root",

    [Parameter()]
    [int]$MaxDepth = 1,

    [Parameter()]
    [int]$MaxElements = 500
)

# Dot-source shared utilities
. "$PSScriptRoot/../../as-shared/scripts/as-shared.ps1"
# Dot-source shared property-grid engine
. "$PSScriptRoot/../../as-shared/scripts/as-propertygrid.ps1"

#region Hardware File Lookup

function Find-HwFile {
    <#
    .SYNOPSIS
        Finds the .hw file matching HwFileName in the active configuration folder.
    #>
    param(
        [string]$ProjectDir,
        [string]$ActiveConfig,
        [string]$HwFileName
    )

    $configDir = Join-Path (Join-Path $ProjectDir "Physical") $ActiveConfig
    if (-not (Test-Path $configDir)) { return $null }

    $hwFile = Join-Path $configDir $HwFileName
    if (Test-Path $hwFile) { return $hwFile }
    return $null
}

#endregion

#region Hardware Module Resolution

function Resolve-HardwareModuleVersion {
    <#
    .SYNOPSIS
        Resolves a hardware module to an installed version in AS content.
    .DESCRIPTION
        Searches AS/Hardware/Modules/<ModuleType>/ for version folders and returns
        the requested version, or the highest available version when not specified.
    #>
    param(
        [string]$ASContentPath,
        [string]$ModuleType,
        [string]$RequestedVersion = ""
    )

    $modulesRoot = Join-Path (Join-Path $ASContentPath "Hardware") "Modules"
    if (-not (Test-Path $modulesRoot)) { return $null }

    $modulePath = Join-Path $modulesRoot $ModuleType
    if (-not (Test-Path $modulePath)) { return $null }

    $versionDirs = Get-ChildItem -Path $modulePath -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -match '^[Vv]?\d+(\.\d+)*$' }

    if ($versionDirs.Count -eq 0) { return $null }

    if ($RequestedVersion) {
        foreach ($dir in $versionDirs) {
            if (($dir.Name -replace '^[Vv]', '') -eq ($RequestedVersion -replace '^[Vv]', '')) {
                return @{ Version = ($dir.Name -replace '^[Vv]', ''); VersionPath = $dir.FullName; ModulePath = $modulePath }
            }
        }
        return $null
    }

    $best = $versionDirs[0]
    foreach ($dir in $versionDirs) {
        $candidate = $dir.Name -replace '^[Vv]', ''
        $current   = $best.Name -replace '^[Vv]', ''
        if ((Compare-Versions $candidate $current) -gt 0) { $best = $dir }
    }

    return @{ Version = ($best.Name -replace '^[Vv]', ''); VersionPath = $best.FullName; ModulePath = $modulePath }
}

function Get-UniqueHardwareModuleName {
    <#
    .SYNOPSIS
        Returns a unique module Name value for Hardware.hw.
    .DESCRIPTION
        Uses the module type as base name. Appends _a, _b, ... if already present.
    #>
    param(
        [string]$HwContent,
        [string]$BaseName
    )

    $existing = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
    [regex]::Matches($HwContent, '<Module\s+[^>]*Name="([^"]+)"') | ForEach-Object {
        $existing.Add($_.Groups[1].Value) | Out-Null
    }

    if (-not $existing.Contains($BaseName)) { return $BaseName }

    for ($i = 0; $i -lt 26; $i++) {
        $suffix    = [char]([int][char]'a' + $i)
        $candidate = "$BaseName`_$suffix"
        if (-not $existing.Contains($candidate)) { return $candidate }
    }

    $counter = 1
    while ($true) {
        $candidate = "$BaseName`_$counter"
        if (-not $existing.Contains($candidate)) { return $candidate }
        $counter++
    }
}

function Register-HardwareModuleInHwFile {
    <#
    .SYNOPSIS
        Adds a <Module> entry to Hardware.hw under <Hardware>.
    .DESCRIPTION
        If a module with matching Type and Version already exists, no change is made.
        Returns @{ Added; Skipped; ModuleName; Reason }.
    #>
    param(
        [string]$HwFilePath,
        [string]$ModuleType,
        [string]$ModuleVersion
    )

    $content        = Get-Content $HwFilePath -Raw
    $escapedType    = [regex]::Escape($ModuleType)
    $escapedVersion = [regex]::Escape($ModuleVersion)
    $existingPattern = '<Module\s+(?=[^>]*Type="' + $escapedType + '")(?=[^>]*Version="' + $escapedVersion + '")[^>]*>'

    if ($content -match $existingPattern) {
        return @{ Added = $false; Skipped = $true; ModuleName = $null; Reason = "module type/version already present" }
    }

    $moduleName = Get-UniqueHardwareModuleName -HwContent $content -BaseName $ModuleType
    $entry      = '  <Module Name="{0}" Type="{1}" Version="{2}"></Module>' -f $moduleName, $ModuleType, $ModuleVersion

    if ($content -match '<Hardware[^>]*>([\s\S]*?)</Hardware>') {
        $insertPoint = $content.IndexOf('</Hardware>')
        if ($insertPoint -eq -1) {
            return @{ Added = $false; Skipped = $false; ModuleName = $null; Reason = "could not locate </Hardware>" }
        }
        $content = $content.Substring(0, $insertPoint) + "$entry`n" + $content.Substring($insertPoint)
    }
    elseif ($content -match '<Hardware[^>]*/>') {
        $content = $content -replace '<Hardware[^>]*/>', "<Hardware>`n$entry`n</Hardware>"
    }
    else {
        return @{ Added = $false; Skipped = $false; ModuleName = $null; Reason = "no <Hardware> element found" }
    }

    [System.IO.File]::WriteAllText($HwFilePath, $content, $script:Utf8NoBom)
    return @{ Added = $true; Skipped = $false; ModuleName = $moduleName; Reason = $null }
}

#endregion

#region AddHWModule Orchestrator

function Invoke-AddHWModule {
    param(
        [string]$ProjectPath,
        [string]$ASContentPath,
        [string]$HwModuleType,
        [string]$HwModuleVersion,
        [string]$HwFileName
    )

    $result = @{
        success       = $false
        moduleType    = $HwModuleType
        moduleVersion = $null
        moduleName    = $null
        hwFile        = $null
        skipped       = $false
        error         = $null
    }

    if (-not $HwModuleType) { $result.error = "HwModuleType parameter is required for AddHWModule action."; return $result }

    $activeConfig = Get-ActiveConfiguration -ProjectDir $ProjectPath
    if (-not $activeConfig) { $result.error = "Could not determine active configuration from LastUser.set."; return $result }
    Write-Host "Active configuration: $activeConfig" -ForegroundColor Cyan

    $hwFilePath = Find-HwFile -ProjectDir $ProjectPath -ActiveConfig $activeConfig -HwFileName $HwFileName
    if (-not $hwFilePath) { $result.error = "Could not find '$HwFileName' in Physical/$activeConfig/."; return $result }
    $result.hwFile = $hwFilePath
    Write-Host "HW file: $hwFilePath" -ForegroundColor Cyan

    $resolvedModule = Resolve-HardwareModuleVersion -ASContentPath $ASContentPath -ModuleType $HwModuleType -RequestedVersion $HwModuleVersion
    if (-not $resolvedModule) {
        $requestedInfo  = if ($HwModuleVersion) { " version '$HwModuleVersion'" } else { "" }
        $result.error   = "Module '$HwModuleType'$requestedInfo was not found in AS hardware modules at '$ASContentPath\Hardware\Modules'."
        return $result
    }
    $result.moduleVersion = $resolvedModule.Version
    Write-Host "Resolved module: $HwModuleType v$($resolvedModule.Version)" -ForegroundColor Gray

    $registerResult = Register-HardwareModuleInHwFile -HwFilePath $hwFilePath -ModuleType $HwModuleType -ModuleVersion $resolvedModule.Version
    if ($registerResult.Skipped) {
        $result.success = $true
        $result.skipped = $true
        return $result
    }

    if (-not $registerResult.Added) { $result.error = "Failed to update Hardware.hw: $($registerResult.Reason)"; return $result }

    $result.moduleName = $registerResult.ModuleName
    $result.success    = $true
    return $result
}

#endregion

#region SearchHWModule Orchestrator

function Invoke-SearchHWModule {
    param(
        [string]$ASContentPath,
        [string]$SearchPattern
    )

    $result = @{
        success  = $false
        basePath = $null
        matches  = @()
        error    = $null
    }

    $modulesRoot    = Join-Path (Join-Path $ASContentPath "Hardware") "Modules"
    $result.basePath = $modulesRoot

    if (-not (Test-Path $modulesRoot)) { $result.error = "Hardware Modules path not found: $modulesRoot"; return $result }

    $matchingDirs = Get-ChildItem -Path $modulesRoot -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -like "*$SearchPattern*" } |
        Sort-Object Name

    if ($matchingDirs.Count -eq 0) { $result.success = $true; return $result }

    if ($matchingDirs.Count -gt 500) {
        $result.error = "Search returned $($matchingDirs.Count) modules - too many results. Narrow your search pattern (e.g. use 'X20DO' instead of 'X20')."
        return $result
    }

    foreach ($dir in $matchingDirs) {
        $versionDirs = Get-ChildItem -Path $dir.FullName -Directory -ErrorAction SilentlyContinue |
            Where-Object { $_.Name -match '^[Vv]?\d+(\.\d+)*$' } |
            Sort-Object { try { [version]($_.Name -replace '^[Vv]', '') } catch { [version]'0.0.0.0' } }

        $versions = $versionDirs | ForEach-Object { $_.Name -replace '^[Vv]', '' }

        # Read Description-en from the highest-version .hwx file
        $description = $null
        if ($versionDirs.Count -gt 0) {
            $highestVersionDir = $versionDirs | Select-Object -Last 1
            $hwxFile = Join-Path $highestVersionDir.FullName ("$($dir.Name).hwx")
            if (Test-Path $hwxFile) {
                try {
                    [xml]$hwxXml = Get-Content $hwxFile -Raw -ErrorAction Stop
                    $moduleNode = $hwxXml.SelectSingleNode('//*[local-name()="Module"]')
                    if ($moduleNode) {
                        $description = $moduleNode.GetAttribute('Description-en')
                        if (-not $description) {
                            $descNode = $moduleNode.SelectSingleNode('.//*[@Description-en]')
                            if ($descNode) { $description = $descNode.GetAttribute('Description-en') }
                        }
                    }
                }
                catch { $description = $null }
            }
        }

        $result.matches += @{
            moduleName  = $dir.Name
            versions    = @($versions)
            description = $description
        }
    }

    $result.success = $true
    return $result
}

function ConvertTo-ClassificationQuery {
    <#
    .SYNOPSIS
        Parses a boolean classification search expression into an OR-of-AND structure.
    .DESCRIPTION
        Supported operators (case-insensitive substring matching on searchable module text):
          OR  : '|'  or the standalone word 'OR'   (match any group)
          AND : '&', '+' or the standalone word 'AND' (all terms in a group must match)
          NOT : leading '-', '!' or the word 'NOT' (term must NOT be present); also
                works infix, so "A NOT B", "A -B" and "A & -B" all mean "A AND NOT B".
        Precedence: NOT binds to a term, AND binds tighter than OR.
        Example: "X20 & Digital | Acopos6D | ACOPOS NOT P3"
        Returns an array of OR-groups; each group is an array of @{ Word; Negate }.
    #>
    param([string]$Expression)

    # Normalize infix negation into explicit AND-NOT so natural agent queries work:
    #   "A NOT B"  -> "A & NOT B"      (infix word NOT)
    #   "A -B"/"A !B" -> "A & -B"      (space-prefixed exclusion token, Google-style)
    $expr = [regex]::Replace($Expression, '(?i)\s+NOT\s+', ' & NOT ')
    $expr = [regex]::Replace($expr, '(?<=\S)\s+([-!])(?=\S)', ' & $1')

    $groups = @()
    # Split on OR (symbol '|' or standalone word 'OR')
    foreach ($orPart in [regex]::Split($expr, '(?i)\s*(?:\||\bOR\b)\s*')) {
        if ([string]::IsNullOrWhiteSpace($orPart)) { continue }

        $terms = @()
        # Split on AND (symbols '&'/'+' or standalone word 'AND')
        foreach ($andPart in [regex]::Split($orPart, '(?i)\s*(?:&|\+|\bAND\b)\s*')) {
            $t = $andPart.Trim()
            if ($t -eq '') { continue }

            $negate = $false
            $m = [regex]::Match($t, '(?i)^(?:NOT\s+|[-!]\s*)(.+)$')
            if ($m.Success) { $negate = $true; $t = $m.Groups[1].Value.Trim() }

            if ($t -ne '') { $terms += @{ Word = $t; Negate = $negate } }
        }

        if ($terms.Count -gt 0) { $groups += , @($terms) }
    }

    return , @($groups)
}

function Test-ContainsSearchTerm {
    <#
    .SYNOPSIS
        Returns true when haystack contains needle (case-insensitive),
        including a whitespace-insensitive fallback.
    .DESCRIPTION
        Handles common formatting variants such as "15.1A" vs "15.1 A"
        by retrying the comparison after removing all whitespace.
    #>
    param(
        [string]$Haystack,
        [string]$Needle
    )

    if ([string]::IsNullOrEmpty($Haystack) -or [string]::IsNullOrWhiteSpace($Needle)) {
        return $false
    }

    if ($Haystack.IndexOf($Needle, [System.StringComparison]::OrdinalIgnoreCase) -ge 0) {
        return $true
    }

    $normalizedHaystack = [regex]::Replace($Haystack, '\s+', '')
    $normalizedNeedle   = [regex]::Replace($Needle, '\s+', '')
    if ([string]::IsNullOrEmpty($normalizedNeedle)) {
        return $false
    }

    return ($normalizedHaystack.IndexOf($normalizedNeedle, [System.StringComparison]::OrdinalIgnoreCase) -ge 0)
}

function Invoke-SearchByClassification {
    <#
    .SYNOPSIS
        Searches all hardware modules whose classification metadata and/or
        Description-en match a boolean keyword expression (OR '|', AND '&', NOT '-').
    .DESCRIPTION
        Walks AS/Hardware/Modules/*, reads the highest installed .hwx per module,
        extracts Classification Value attributes and the top-level module
        Description-en, and evaluates the boolean keyword expression against both
        sources (case-insensitive substring). Returns matching module numbers with
        description and match details.
    #>
    param(
        [string]$ASContentPath,
        [string]$Keyword
    )

    $result = @{
        success    = $false
        basePath   = $null
        keyword    = $Keyword
        matchCount = 0
        matches    = @()
        error      = $null
    }

    $modulesRoot     = Join-Path (Join-Path $ASContentPath "Hardware") "Modules"
    $result.basePath = $modulesRoot

    if (-not (Test-Path $modulesRoot)) { $result.error = "Hardware Modules path not found: $modulesRoot"; return $result }

    # Parse the boolean keyword expression (OR '|', AND '&'/'+', NOT '-'/'!') into
    # an OR-of-AND structure. A module matches when ANY group matches, and a group
    # matches when ALL of its (possibly negated) terms match either classification
    # values or the module Description-en text.
    $queryGroups = ConvertTo-ClassificationQuery -Expression $Keyword
    if ($queryGroups.Count -eq 0) { $result.error = "No valid search term in keyword '$Keyword'."; return $result }

    $moduleDirs = Get-ChildItem -Path $modulesRoot -Directory -ErrorAction SilentlyContinue | Sort-Object Name

    $classificationRegex = [regex]'(?is)<Classification\b[^>]*>(.*?)</Classification>'
    $valueRegex          = [regex]'(?i)\bValue\s*=\s*"([^"]*)"'
    $moduleDescRegex     = [regex]'(?is)<Module\b[^>]*\bDescription-en\s*=\s*"([^"]*)"'


    # Progress feedback (host stream only - does not pollute the JSON output stream)
    $sw            = [System.Diagnostics.Stopwatch]::StartNew()
    $nextTick      = [TimeSpan]::FromSeconds(2)
    $progressShown = $false

    foreach ($dir in $moduleDirs) {
        if ($sw.Elapsed -ge $nextTick) {
            if (-not $progressShown) { Write-Host -NoNewline "Searching"; $progressShown = $true }
            Write-Host -NoNewline "."
            $nextTick = $nextTick.Add([TimeSpan]::FromSeconds(2))
        }

        # Pick highest-version folder (same logic as SearchHWModule) - the highest
        # hardware number always carries the newest and most complete metadata.
        $versionDirs = Get-ChildItem -Path $dir.FullName -Directory -ErrorAction SilentlyContinue |
            Where-Object { $_.Name -match '^[Vv]?\d+(\.\d+)*$' } |
            Sort-Object { try { [version]($_.Name -replace '^[Vv]', '') } catch { [version]'0.0.0.0' } }

        if ($versionDirs.Count -eq 0) { continue }

        $highestVersionDir = $versionDirs | Select-Object -Last 1
        $hwxFile           = Join-Path $highestVersionDir.FullName ("$($dir.Name).hwx")
        if (-not (Test-Path $hwxFile)) { continue }

        try { $content = Get-Content $hwxFile -Raw -ErrorAction Stop } catch { continue }

        $classMatch = $classificationRegex.Match($content)
        if (-not $classMatch.Success) { continue }

        # Top-level module Description-en.
        $description = $null
        $dm = $moduleDescRegex.Match($content)
        if ($dm.Success) { $description = $dm.Groups[1].Value }

        # Collect all Classification Values once.
        $values = @()
        foreach ($vm in $valueRegex.Matches($classMatch.Groups[1].Value)) {
            if ($vm.Groups[1].Value) { $values += $vm.Groups[1].Value }
        }

        # Evaluate the OR-of-AND query against classification values and Description-en.
        # Matching is whitespace-tolerant so tokens like "15.1A" and "15.1 A"
        # are treated as equivalent.
        $matchedValues = $null
        $matchedDescriptionTerms = @()
        foreach ($group in $queryGroups) {
            $groupOk                   = $true
            $groupMatched              = @()
            $groupMatchedDescription   = @()
            foreach ($term in $group) {
                $valueHits = @($values | Where-Object { Test-ContainsSearchTerm -Haystack $_ -Needle $term.Word })
                $descriptionHit = Test-ContainsSearchTerm -Haystack $description -Needle $term.Word
                $anyHit = ($valueHits.Count -gt 0 -or $descriptionHit)

                if ($term.Negate) {
                    if ($anyHit) { $groupOk = $false; break }
                }
                else {
                    if (-not $anyHit) { $groupOk = $false; break }
                    if ($valueHits.Count -gt 0) { $groupMatched += $valueHits }
                    if ($descriptionHit) { $groupMatchedDescription += $term.Word }
                }
            }
            if ($groupOk) {
                $matchedValues = @($groupMatched | Select-Object -Unique)
                $matchedDescriptionTerms = @($groupMatchedDescription | Select-Object -Unique)
                break
            }
        }

        if ($null -eq $matchedValues) { continue }

        $result.matches += @{
            moduleName              = $dir.Name
            description             = $description
            matchedValues           = @($matchedValues)
            matchedDescriptionTerms = @($matchedDescriptionTerms)
        }
    }

    if ($progressShown) { Write-Host "" }

    $result.matchCount = $result.matches.Count
    $result.success    = $true
    return $result
}

#endregion

#region HWX File Resolution

function Resolve-HwxFile {
    <#
    .SYNOPSIS
        Resolves a module's .hwx descriptor file from the AS installation.
    .DESCRIPTION
        Locates AS/Hardware/Modules/<ModuleType>/<Version>/<ModuleType>.hwx.
        When no version is supplied, the highest installed version is chosen.
        Returns @{ Version; HwxFile; Error }.
    #>
    param(
        [string]$ASContentPath,
        [string]$ModuleType,
        [string]$ModuleVersion = ""
    )

    $modulesRoot = Join-Path (Join-Path $ASContentPath "Hardware") "Modules"
    $modulePath  = Join-Path $modulesRoot $ModuleType

    if (-not (Test-Path $modulePath)) {
        return @{ Version = $null; HwxFile = $null; Error = "Module '$ModuleType' not found at: $modulePath" }
    }

    $resolvedVersion = $ModuleVersion
    if (-not $resolvedVersion) {
        $best = $null
        foreach ($dir in Get-ChildItem -Path $modulePath -Directory -ErrorAction SilentlyContinue |
            Where-Object { $_.Name -match '^[Vv]?\d+(\.\d+)*$' }) {
            $candidate = $dir.Name -replace '^[Vv]', ''
            if (-not $best -or (Compare-Versions $candidate $best) -gt 0) { $best = $candidate }
        }
        if (-not $best) { return @{ Version = $null; HwxFile = $null; Error = "No version folders found for module '$ModuleType'." } }
        $resolvedVersion = $best
    }

    $versionDir = Join-Path $modulePath $resolvedVersion
    if (-not (Test-Path $versionDir)) { $versionDir = Join-Path $modulePath "V$resolvedVersion" }
    if (-not (Test-Path $versionDir)) {
        return @{ Version = $resolvedVersion; HwxFile = $null; Error = "Version folder '$resolvedVersion' not found for module '$ModuleType'." }
    }

    $hwxFile = Join-Path $versionDir "$ModuleType.hwx"
    if (-not (Test-Path $hwxFile)) {
        return @{ Version = $resolvedVersion; HwxFile = $null; Error = "HWX file not found: $hwxFile" }
    }

    return @{ Version = $resolvedVersion; HwxFile = $hwxFile; Error = $null }
}

function Get-ProjectCpuTarget {
    <#
    .SYNOPSIS
        Determines the I/O mapping target generation (SG3/SG4/SGC) of the project's active CPU.
    .DESCRIPTION
        Resolves the active configuration's CPU module (from Cpu.pkg), loads its .hwx, and
        reads the I/O mapping target in priority order:
          1. <Editor Name="IoMapping" Target="..."/>
          2. first module-level <Channels Target="..."/>
          3. <Firmware><Target Name="..."/>
          4. default "SG4"
        Returns @{ Target; Source; Cpu; Error }.
    #>
    param(
        [string]$ProjectDir,
        [string]$ASContentPath
    )

    $result = @{ Target = "SG4"; Source = "default"; Cpu = $null; Error = $null }

    $activeConfig = Get-ActiveConfiguration -ProjectDir $ProjectDir
    if (-not $activeConfig) { $result.Error = "Could not determine active configuration."; return $result }

    $configDir = Join-Path (Join-Path $ProjectDir "Physical") $activeConfig
    if (-not (Test-Path $configDir)) { $result.Error = "Config directory not found: $configDir"; return $result }

    # Find the CPU package (Cpu.pkg) under a CPU folder in the active configuration.
    $cpuPkg = $null
    foreach ($cpuDir in Get-ChildItem -Path $configDir -Directory -ErrorAction SilentlyContinue) {
        $candidate = Join-Path $cpuDir.FullName "Cpu.pkg"
        if (Test-Path $candidate) { $cpuPkg = $candidate; break }
    }
    if (-not $cpuPkg) { $result.Error = "No Cpu.pkg found in $configDir."; return $result }

    try { [xml]$cpuXml = Get-Content $cpuPkg -Raw -ErrorAction Stop }
    catch { $result.Error = "Failed to parse Cpu.pkg: $_"; return $result }

    $cfgNode = $cpuXml.SelectSingleNode('//*[local-name()="Configuration"]')
    $cpuType = if ($cfgNode) { $cfgNode.GetAttribute('ModuleId') } else { $null }
    if (-not $cpuType) { $result.Error = "Could not read CPU ModuleId from Cpu.pkg."; return $result }
    $result.Cpu = $cpuType

    # Determine the CPU version from Hardware.hw when possible (else highest installed).
    $cpuVersion = ""
    $hwFile = Join-Path $configDir "Hardware.hw"
    if (Test-Path $hwFile) {
        $hwContent = Get-Content $hwFile -Raw
        $escaped = [regex]::Escape($cpuType)
        if ($hwContent -match ('<Module\s+[^>]*Type="' + $escaped + '"[^>]*Version="([^"]+)"')) {
            $cpuVersion = $matches[1]
        }
    }

    $resolved = Resolve-HwxFile -ASContentPath $ASContentPath -ModuleType $cpuType -ModuleVersion $cpuVersion
    if ($resolved.Error -or -not $resolved.HwxFile) { return $result }

    try { [xml]$hwxXml = Get-Content $resolved.HwxFile -Raw -ErrorAction Stop }
    catch { return $result }

    # 1. <Editor Name="IoMapping" Target="..."/>
    $ioEditor = $hwxXml.SelectSingleNode('//*[local-name()="Editor"][@Name="IoMapping"][@Target]')
    if ($ioEditor -and $ioEditor.GetAttribute('Target')) {
        $result.Target = $ioEditor.GetAttribute('Target'); $result.Source = "cpuIoMappingEditor"; return $result
    }

    # 2. First module-level <Channels Target="..."/>
    $chTarget = $hwxXml.SelectSingleNode('/*[local-name()="Module"]/*[local-name()="Channels"][@Target]')
    if ($chTarget -and $chTarget.GetAttribute('Target')) {
        $result.Target = $chTarget.GetAttribute('Target'); $result.Source = "cpuChannelsTarget"; return $result
    }

    # 3. <Firmware><Target Name="..."/>
    $fwTarget = $hwxXml.SelectSingleNode('//*[local-name()="Firmware"]/*[local-name()="Target"][@Name]')
    if ($fwTarget -and $fwTarget.GetAttribute('Name')) {
        $result.Target = $fwTarget.GetAttribute('Name'); $result.Source = "cpuFirmwareTarget"; return $result
    }

    return $result
}

#endregion

#region HWX Channel Extraction

function Get-ChannelEntry {
    <#
    .SYNOPSIS
        Builds an ordered channel info hashtable from a <Channel> element.
    #>
    param(
        [System.Xml.XmlElement]$Channel,
        [string]$ConnectorName
    )

    $desc = $Channel.GetAttribute('Description-en')
    if (-not $desc) { $desc = $Channel.GetAttribute('Description-de') }

    $dirParam  = $Channel.SelectSingleNode('./*[local-name()="Parameter"][@ID="Direction"]')
    $direction = if ($dirParam) { $dirParam.GetAttribute('Value') } else { $null }
    $direction = switch ($direction) { 'IN' { 'In' } 'OUT' { 'Out' } default { $direction } }

    $typeParam = $Channel.SelectSingleNode('./*[local-name()="Parameter"][@ID="Type"]')
    $dataType  = if ($typeParam) { $typeParam.GetAttribute('Value') } else { $null }

    $entry = [ordered]@{
        channelName = $Channel.GetAttribute('ID')
        dataType    = $dataType
        direction   = $direction
        description = $desc
    }

    $filterNode = $Channel.SelectSingleNode('./*[local-name()="Filter"]')
    if ($filterNode -and $filterNode.GetAttribute('Value')) { $entry.filter = $filterNode.GetAttribute('Value') }

    if ($ConnectorName) { $entry.connector = $ConnectorName }

    return $entry
}

function Add-ChannelToMap {
    <#
    .SYNOPSIS
        Adds a channel to the de-duplication map, preferring the variant without ToVersion/Filter.
    #>
    param(
        [System.Xml.XmlElement]$Channel,
        [string]$ConnectorName,
        [System.Collections.Specialized.OrderedDictionary]$Map
    )

    $id = $Channel.GetAttribute('ID')
    if (-not $id) { return }

    $score = 0
    if (-not [string]::IsNullOrEmpty($Channel.GetAttribute('ToVersion'))) { $score += 2 }
    if ($null -ne $Channel.SelectSingleNode('./*[local-name()="Filter"]')) { $score += 1 }

    if ($Map.Contains($id)) {
        if ($score -lt [int]$Map[$id].Score) {
            $Map[$id] = @{ Entry = (Get-ChannelEntry -Channel $Channel -ConnectorName $ConnectorName); Score = $score }
        }
    }
    else {
        $Map[$id] = @{ Entry = (Get-ChannelEntry -Channel $Channel -ConnectorName $ConnectorName); Score = $score }
    }
}

function Add-ChannelsFromNode {
    <#
    .SYNOPSIS
        Walks a <Channels> element in document order, adding channels (and optionally
        resolving <Include> fragments) to the de-duplication map.
    #>
    param(
        [System.Xml.XmlNode]$ChannelsNode,
        [string]$ConnectorName,
        [System.Collections.Specialized.OrderedDictionary]$Map,
        [string]$ChannelsDir,
        [switch]$ResolveIncludes,
        [System.Collections.Generic.HashSet[string]]$Visited
    )

    foreach ($node in $ChannelsNode.ChildNodes) {
        if ($node.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
        $ln = $node.LocalName
        if ($ln -eq 'Channel') {
            Add-ChannelToMap -Channel $node -ConnectorName $ConnectorName -Map $Map
        }
        elseif ($ln -eq 'Include' -and $ResolveIncludes) {
            $file = $node.GetAttribute('File')
            if ($file -and $Visited.Add($file)) {
                $fragPath = Join-Path $ChannelsDir "$file.xml"
                if (Test-Path $fragPath) {
                    $frag = $null
                    try { [xml]$frag = Get-Content $fragPath -Raw -ErrorAction Stop } catch { $frag = $null }
                    if ($frag) {
                        $fragRoot = $frag.SelectSingleNode('/*[local-name()="Channels"]')
                        if ($fragRoot) {
                            Add-ChannelsFromNode -ChannelsNode $fragRoot -ConnectorName $ConnectorName -Map $Map `
                                -ChannelsDir $ChannelsDir -ResolveIncludes:$ResolveIncludes -Visited $Visited
                        }
                    }
                }
            }
        }
    }
}

function Get-HwxChannels {
    <#
    .SYNOPSIS
        Collects de-duplicated I/O channels from an .hwx XML for a given target generation.
    .DESCRIPTION
        Gathers <Channel> nodes from both module-level <Channels> blocks and connector-scoped
        <Connector>/<Channels> blocks whose @Target matches the requested target (or that have
        no @Target). Optionally resolves <Include> channel fragments. De-duplicates by channel
        ID, preferring the variant without ToVersion/Filter, and preserves document order.
    #>
    param(
        [System.Xml.XmlNode]$HwxXml,
        [string]$Target,
        [string]$ChannelsDir,
        [switch]$ResolveIncludes
    )

    $map = [System.Collections.Specialized.OrderedDictionary]::new()
    $visited = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)

    foreach ($ch in $HwxXml.SelectNodes('//*[local-name()="Channels"]')) {
        $blkTarget = $ch.GetAttribute('Target')
        if ($blkTarget -and $Target -and $blkTarget -ne $Target) { continue }

        $connName = $null
        if ($ch.ParentNode -and $ch.ParentNode.LocalName -eq 'Connector') {
            $connName = $ch.ParentNode.GetAttribute('Name')
        }

        Add-ChannelsFromNode -ChannelsNode $ch -ConnectorName $connName -Map $map `
            -ChannelsDir $ChannelsDir -ResolveIncludes:$ResolveIncludes -Visited $visited
    }

    $list = [System.Collections.Generic.List[object]]::new()
    foreach ($key in $map.Keys) { $list.Add($map[$key].Entry) }
    return $list
}

#endregion

#region GetIOMappingConfig Orchestrator

function Invoke-GetIOMappingConfig {
    param(
        [string]$ProjectPath,
        [string]$ASContentPath,
        [string]$ModuleType,
        [string]$ModuleVersion,
        [string]$Target,
        [switch]$ResolveIncludes
    )

    $result = [ordered]@{
        success         = $false
        moduleType      = $ModuleType
        moduleVersion   = $null
        resolvedTarget  = $null
        targetSource    = $null
        cpu             = $null
        resolveIncludes = [bool]$ResolveIncludes
        hwxFile         = $null
        channelCount    = 0
        channels        = @()
        error           = $null
    }

    if (-not $ModuleType) { $result.error = "HwModuleType parameter is required for GetIOMappingConfig action."; return $result }

    $hwx = Resolve-HwxFile -ASContentPath $ASContentPath -ModuleType $ModuleType -ModuleVersion $ModuleVersion
    if ($hwx.Version) { $result.moduleVersion = $hwx.Version }
    if ($hwx.HwxFile) { $result.hwxFile = $hwx.HwxFile }
    if ($hwx.Error) { $result.error = $hwx.Error; return $result }

    if ($Target) {
        $result.resolvedTarget = $Target
        $result.targetSource   = "parameter"
    }
    else {
        $cpuTarget = Get-ProjectCpuTarget -ProjectDir $ProjectPath -ASContentPath $ASContentPath
        $result.resolvedTarget = $cpuTarget.Target
        $result.targetSource   = $cpuTarget.Source
        $result.cpu            = $cpuTarget.Cpu
    }

    try { [xml]$hwxXml = Get-Content $hwx.HwxFile -Raw -ErrorAction Stop }
    catch { $result.error = "Failed to parse '$($hwx.HwxFile)': $_"; return $result }

    $channelsDir = Join-Path (Join-Path $ASContentPath "Hardware") "Channels"
    $channels = Get-HwxChannels -HwxXml $hwxXml -Target $result.resolvedTarget -ChannelsDir $channelsDir -ResolveIncludes:$ResolveIncludes

    $result.channels     = @($channels)
    $result.channelCount = @($channels).Count
    $result.success      = $true
    return $result
}

#endregion

#region GetPossibleHWConnections Orchestrator

function Invoke-GetPossibleHWConnections {
    param(
        [string]$ASContentPath,
        [string]$ModuleType,
        [string]$ModuleVersion
    )

    $result = [ordered]@{
        success        = $false
        moduleType     = $ModuleType
        moduleVersion  = $null
        hwxFile        = $null
        connectorCount = 0
        connectors     = @()
        error          = $null
    }

    if (-not $ModuleType) { $result.error = "HwModuleType parameter is required for GetPossibleHWConnections action."; return $result }

    $hwx = Resolve-HwxFile -ASContentPath $ASContentPath -ModuleType $ModuleType -ModuleVersion $ModuleVersion
    if ($hwx.Version) { $result.moduleVersion = $hwx.Version }
    if ($hwx.HwxFile) { $result.hwxFile = $hwx.HwxFile }
    if ($hwx.Error) { $result.error = $hwx.Error; return $result }

    try { [xml]$hwxXml = Get-Content $hwx.HwxFile -Raw -ErrorAction Stop }
    catch { $result.error = "Failed to parse '$($hwx.HwxFile)': $_"; return $result }

    $connectorNodes = $hwxXml.SelectNodes('/*[local-name()="Module"]/*[local-name()="Connectors"]/*[local-name()="Connector"]')
    $connectors = [System.Collections.Generic.List[object]]::new()

    foreach ($conn in $connectorNodes) {
        $entry = [ordered]@{ name = $conn.GetAttribute('Name') }

        $type = $conn.GetAttribute('Type')
        if ($type) { $entry.type = $type }

        if (-not [string]::IsNullOrEmpty($conn.GetAttribute('Male'))) { $entry.male = ($conn.GetAttribute('Male') -eq 'true') }

        $networkType = $conn.GetAttribute('NetworkType')
        if ($networkType) { $entry.networkType = $networkType }

        $label = $conn.GetAttribute('Label')
        if ($label) { $entry.label = $label }

        # Compatible connection types.
        $connTypes = [System.Collections.Generic.List[string]]::new()
        foreach ($c in $conn.SelectNodes('./*[local-name()="Connections"]/*[local-name()="Connection"]')) {
            $cn = $c.GetAttribute('Name')
            if ($cn) { $connTypes.Add($cn) }
        }
        if ($connTypes.Count -gt 0) { $entry.connectionTypes = @($connTypes) }

        # Auto-connect routing recipes (target module type + target connector).
        $autoList = [System.Collections.Generic.List[object]]::new()
        foreach ($ac in $conn.SelectNodes('./*[local-name()="AutoConnect"]')) {
            $acEntry = [ordered]@{
                targetModule    = $ac.GetAttribute('ModuleID')
                targetConnector = $ac.GetAttribute('ConnectorName')
            }
            $pref = $ac.GetAttribute('PreferredConnectionName')
            if ($pref) { $acEntry.preferredConnectionName = $pref }
            if (-not [string]::IsNullOrEmpty($ac.GetAttribute('Required'))) { $acEntry.required = ($ac.GetAttribute('Required') -eq 'true') }
            $autoList.Add($acEntry)
        }
        if ($autoList.Count -gt 0) { $entry.autoConnect = @($autoList) }

        $connectors.Add($entry)
    }

    $result.connectors     = @($connectors)
    $result.connectorCount = $connectors.Count
    $result.success        = $true
    return $result
}

#endregion

#region GetHWModuleInfo Orchestrator

function Invoke-GetHWModuleInfo {
    param(
        [string]$ASContentPath,
        [string]$ModuleType,
        [string]$ModuleVersion
    )

    $result = @{
        success        = $false
        moduleType     = $ModuleType
        moduleVersion  = $null
        description    = $null
        hwxFile        = $null
        classification = $null
        error          = $null
    }

    $modulesRoot = Join-Path (Join-Path $ASContentPath "Hardware") "Modules"
    $modulePath  = Join-Path $modulesRoot $ModuleType

    if (-not (Test-Path $modulePath)) { $result.error = "Module '$ModuleType' not found at: $modulePath"; return $result }

    $hwx = Resolve-HwxFile -ASContentPath $ASContentPath -ModuleType $ModuleType -ModuleVersion $ModuleVersion
    if ($hwx.Version) { $result.moduleVersion = $hwx.Version }
    if ($hwx.HwxFile) { $result.hwxFile = $hwx.HwxFile }
    if ($hwx.Error) { $result.error = $hwx.Error; return $result }

    $hwxFile = $hwx.HwxFile

    try {
        [xml]$hwxXml = Get-Content $hwxFile -Raw -ErrorAction Stop
    }
    catch {
        $result.error = "Failed to parse '$hwxFile': $_"
        return $result
    }

    $moduleNode = $hwxXml.SelectSingleNode('//*[local-name()="Module"]')
    if (-not $moduleNode) { $result.error = "No <Module> element found in: $hwxFile"; return $result }

    $desc = $moduleNode.GetAttribute('Description-en')
    if (-not $desc) {
        $descNode = $moduleNode.SelectSingleNode('.//*[@Description-en]')
        if ($descNode) { $desc = $descNode.GetAttribute('Description-en') }
    }
    if (-not $desc) { $result.error = "'Description-en' attribute not found in <Module> element of: $hwxFile"; return $result }

    $result.description = $desc

    $classificationNode = $hwxXml.SelectSingleNode('//*[local-name()="Classification"]')
    $classMap = [System.Collections.Specialized.OrderedDictionary]::new()
    if ($classificationNode) {
        foreach ($child in $classificationNode.ChildNodes) {
            if ($child.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
            $key = $child.LocalName
            $val = $child.GetAttribute('Value')
            if (-not $classMap.Contains($key)) {
                $classMap[$key] = [System.Collections.Generic.List[string]]::new()
            }
            if ($val) { $classMap[$key].Add($val) }
        }
    }
    $result.classification = $classMap
    $result.success        = $true
    return $result
}

#endregion

#region GetModuleProperties Orchestrator

function Resolve-LocationConfigLink {
    <#
    .SYNOPSIS
        Resolves the Technology-Package configuration file(s) that DEFINE a module's
        Location-based sub-config namespaces (e.g. an ACOPOS drive's "DriveConfiguration").

    .DESCRIPTION
        A module opts in to a sub-config namespace via <Properties AsHwAcLocation="..."> and
        supplies the namespace definition in one of two ways:

          (a) EXTERNAL link — an <Include Directory="<tp>/{VersionN}/Editors/..." File="..."/>
              child of <Properties>. The {VersionN} placeholder is bound by the matching
              VersionType<N> attribute (e.g. VersionType3="TechnologyPackage:%mappMotion%" =>
              {Version3} is the project's mappMotion TP version). The link is followed (through its
              nested <Include> chain) to the file(s) that declare the <Group ... ArAsHwAc="true">
              namespace roots. (Typical for ACOPOS drives -> "DriveConfiguration".)

          (b) INLINE definition — the <Properties> block itself contains the
              <Group ... ArAsHwAc="true"> namespace root(s) (no external include). (Typical for
              motors -> e.g. "M1"; these roots are already returned in this action's elements[].)

        The <Group ... ArAsHwAc="true"> ID(s) are exactly the values that become the "Location"
        prefix in Hardware.hw. Returns $null for modules that do not use a Location namespace.
    #>
    param(
        [System.Xml.XmlElement]$PropsNode,
        [string]$ASContentPath,
        [string]$ProjectDir,
        [string]$HwxFile
    )

    $asHwAc = $PropsNode.GetAttribute('AsHwAcLocation')
    if ([string]::IsNullOrEmpty($asHwAc)) { return $null }   # module does not use Location config

    $info = [ordered]@{
        asHwAcLocation    = $asHwAc
        definedInline     = $false
        technologyPackage = $null
        resolvedVersion   = $null
        versionSource     = $null
        dimensionVersions = [ordered]@{}
        entryFile         = $null
        namespaceFiles    = @()
        locationRoots     = @()
        unresolved        = @()
    }

    # 0. Inline namespace roots: <Group ArAsHwAc="true"> declared directly in this .hwx <Properties>.
    $inlineRoots = New-Object System.Collections.Generic.List[string]
    foreach ($g in @($PropsNode.SelectNodes('.//*[local-name()="Group"][@ArAsHwAc="true"]'))) {
        $gid = $g.GetAttribute('ID')
        if ($gid -and -not $inlineRoots.Contains($gid)) { $inlineRoots.Add($gid) }
    }
    if ($inlineRoots.Count -gt 0) {
        $info.definedInline = $true
        $info.entryFile     = $HwxFile
        $info.namespaceFiles = @([ordered]@{ file = $HwxFile; locationRoots = @($inlineRoots) })
        foreach ($r in $inlineRoots) { $info.locationRoots += $r }
    }

    # 1. Map {VersionN} placeholders -> their VersionType<N> binding.
    $verBindings = @{}   # N -> @{ kind = 'TechnologyPackage'|'AutomationRuntime'|'AutomationStudio'; tp = <name> }
    foreach ($attr in $PropsNode.Attributes) {
        if ($attr.Name -match '^VersionType(\d+)$') {
            $idx = $matches[1]
            if ($attr.Value -match '^TechnologyPackage:%(.+?)%$') {
                $verBindings[$idx] = @{ kind = 'TechnologyPackage'; tp = $matches[1] }
            }
            elseif ($attr.Value -match 'AutomationRuntime') { $verBindings[$idx] = @{ kind = 'AutomationRuntime' } }
            elseif ($attr.Value -match 'AutomationStudio')  { $verBindings[$idx] = @{ kind = 'AutomationStudio' } }
        }
    }

    # 2. Read project technology-package versions from the .apj (<TechnologyPackages><TP Version=.../>).
    $tpVersions = @{}   # tp-name (lower) -> version
    $projectAsVersion = $null
    try {
        $apj = Get-ProjectFile -ProjectDir $ProjectDir
        $projectAsVersion = Get-ProjectVersion -ApjFile $apj
        [xml]$apjXml = Get-Content $apj -Raw -ErrorAction Stop
        $tpRoot = $apjXml.SelectSingleNode('/*[local-name()="Project"]/*[local-name()="TechnologyPackages"]')
        if ($tpRoot) {
            foreach ($tp in $tpRoot.ChildNodes) {
                if ($tp.NodeType -ne [System.Xml.XmlNodeType]::Element) { continue }
                $v = $tp.GetAttribute('Version')
                if ($v) { $tpVersions[$tp.LocalName.ToLower()] = $v }
            }
        }
    }
    catch { }

    $tpRootDir = Join-Path $ASContentPath 'TechnologyPackages'

    # Resolve a TP version: project-declared first, else highest installed folder.
    $resolveTpVersion = {
        param($tpName)
        $key = $tpName.ToLower()
        if ($tpVersions.ContainsKey($key)) { return @{ ver = $tpVersions[$key]; src = 'project' } }
        $tpDir = Join-Path $tpRootDir $tpName
        if (Test-Path $tpDir) {
            $highest = $null
            foreach ($d in (Get-ChildItem $tpDir -Directory -ErrorAction SilentlyContinue)) {
                if (-not $highest -or (Compare-Versions $d.Name $highest) -gt 0) { $highest = $d.Name }
            }
            if ($highest) { return @{ ver = $highest; src = 'installed-highest' } }
        }
        return @{ ver = $null; src = 'unresolved' }
    }

    # 2b. Resolve a concrete version for every VersionType<N> dimension (informative + reused for
    #     {VersionN} substitution and per-dimension FromVersion<N>/ToVersion<N> filtering).
    foreach ($n in ($verBindings.Keys | Sort-Object)) {
        $b = $verBindings[$n]
        $entry = [ordered]@{}
        switch ($b.kind) {
            'TechnologyPackage' {
                $r = & $resolveTpVersion $b.tp
                $entry.binding = "TechnologyPackage:$($b.tp)"
                $entry.version = $r.ver
                $entry.source  = $r.src
            }
            'AutomationRuntime' {
                # AR version is not reliably available from the .apj; use the project AS version as a proxy.
                $entry.binding = 'AutomationRuntime'
                $entry.version = $projectAsVersion
                $entry.source  = if ($projectAsVersion) { 'project-as-proxy' } else { 'unresolved' }
            }
            'AutomationStudio' {
                $entry.binding = 'AutomationStudio'
                $entry.version = $projectAsVersion
                $entry.source  = if ($projectAsVersion) { 'project' } else { 'unresolved' }
            }
        }
        $info.dimensionVersions[$n] = $entry
    }

    # Substitute every {VersionN} token in a Directory string; returns $null if any stays unresolved.
    $substituteVersions = {
        param($dir)
        $out = $dir
        foreach ($m in [regex]::Matches($dir, '\{Version(\d+)\}')) {
            $n = $m.Groups[1].Value
            $dv = $info.dimensionVersions[$n]
            if ($dv -and $dv.version) {
                if (-not $script:__primaryTp -and $dv.binding -like 'TechnologyPackage:*') {
                    $script:__primaryTp = $dv.binding
                    $info.technologyPackage = ($dv.binding -replace '^TechnologyPackage:', '')
                    $info.resolvedVersion   = $dv.version
                    $info.versionSource     = $dv.source
                }
                $out = $out -replace [regex]::Escape($m.Value), $dv.version
            }
        }
        if ($out -match '\{Version\d+\}') { return $null }
        return $out
    }
    $script:__primaryTp = $null

    # Build an absolute path from a TP-editor Directory + File pair.
    $resolvePath = {
        param($dir, $file)
        $rd = & $substituteVersions $dir
        if (-not $rd) { return $null }
        $p = Join-Path $tpRootDir (Join-Path $rd $file)
        if ([System.IO.Path]::GetExtension($p) -eq '') { $p = "$p.xml" }
        try { return [System.IO.Path]::GetFullPath($p) } catch { return $null }
    }

    # 3. Find the entry <Include> (version-templated TP editor reference) under <Properties>.
    $entryPath = $null
    foreach ($inc in $PropsNode.ChildNodes) {
        if ($inc.NodeType -ne [System.Xml.XmlNodeType]::Element -or $inc.LocalName -ne 'Include') { continue }
        $dir = $inc.GetAttribute('Directory'); $file = $inc.GetAttribute('File')
        if ([string]::IsNullOrEmpty($file) -or $dir -notmatch '\{Version\d+\}') { continue }
        $entryPath = & $resolvePath $dir $file
        break
    }
    if (-not $entryPath) {
        # No external link. That is expected for inline namespaces; only an error otherwise.
        if (-not $info.definedInline) {
            $info.unresolved += 'no version-templated TP <Include> under <Properties> and no inline ArAsHwAc root'
        }
        return $info
    }
    if (-not $info.definedInline) { $info.entryFile = $entryPath }
    if (-not (Test-Path $entryPath -PathType Leaf)) { $info.unresolved += "entry file not found: $entryPath"; return $info }

    # 4. Walk the include chain to the file(s) declaring <Group ArAsHwAc="true"> namespace roots.
    #    Files that yield roots are the namespace definitions; we do not descend into their
    #    fragment includes (those are members inside the namespace, not new roots).
    $nsFiles = New-Object System.Collections.Generic.List[object]
    $roots   = New-Object System.Collections.Generic.List[string]
    foreach ($r in $info.locationRoots) { [void]$roots.Add($r) }   # seed with any inline roots
    $seen    = New-Object System.Collections.Generic.HashSet[string]
    $queue   = New-Object System.Collections.Generic.Queue[string]
    $queue.Enqueue($entryPath)
    while ($queue.Count -gt 0 -and $seen.Count -lt 50) {
        $cur = $queue.Dequeue()
        if (-not $seen.Add($cur.ToLower())) { continue }
        if (-not (Test-Path $cur -PathType Leaf)) { continue }
        try { [xml]$cx = Get-Content $cur -Raw -ErrorAction Stop } catch { continue }

        $localRoots = @()
        foreach ($g in @($cx.SelectNodes('//*[local-name()="Group"][@ArAsHwAc="true"]'))) {
            $gid = $g.GetAttribute('ID')
            if ($gid) { $localRoots += $gid; if (-not $roots.Contains($gid)) { $roots.Add($gid) } }
        }
        if ($localRoots.Count -gt 0) {
            $nsFiles.Add([ordered]@{ file = $cur; locationRoots = $localRoots })
            continue   # namespace found here; its includes are members, not roots
        }
        # No roots yet: follow nested TP-editor includes to find the definition file.
        foreach ($inc in @($cx.SelectNodes('//*[local-name()="Include"]'))) {
            $d = $inc.GetAttribute('Directory'); $f = $inc.GetAttribute('File')
            if ([string]::IsNullOrEmpty($f) -or [string]::IsNullOrEmpty($d)) { continue }
            $np = & $resolvePath $d $f
            if ($np) { $queue.Enqueue($np) }
        }
    }

    # Merge external results with any inline ones already recorded.
    $mergedFiles = New-Object System.Collections.Generic.List[object]
    foreach ($nf in $info.namespaceFiles) { $mergedFiles.Add($nf) }
    foreach ($nf in $nsFiles) { $mergedFiles.Add($nf) }
    $info.namespaceFiles = $mergedFiles
    $info.locationRoots  = $roots
    return $info
}

function Invoke-GetModuleProperties {
    <#
    .SYNOPSIS
        Reads the <Properties> node(s) from a module's .hwx and emits the permitted
        configuration properties as a JSON-compatible ordered hashtable tree.
        Uses the shared as-propertygrid engine (same model as as-tp-config-api).
        Always resolves cross-reference includes and always includes internal parameters.
    #>
    param(
        [string]$ProjectPath,
        [string]$ASContentPath,
        [string]$ModuleType,
        [string]$ModuleVersion,
        [string]$Target,
        [string]$ParentElement   = "Root",
        [int]$MaxDepth           = 1,
        [int]$MaxElements        = 500
    )

    $result = [ordered]@{
        success              = $false
        moduleType           = $ModuleType
        moduleVersion        = $null
        hwxFile              = $null
        resolvedTarget       = $null
        targetSource         = $null
        cpu                  = $null
        parentElement        = $(if ([string]::IsNullOrWhiteSpace($ParentElement)) { 'Root' } else { $ParentElement })
        maxDepth             = $MaxDepth
        unresolvedIncludes   = @()
        locationConfig       = $null
        emittedCount         = 0
        elements             = @()
        error                = $null
    }

    if (-not $ModuleType) { $result.error = "HwModuleType is required for GetModuleProperties."; return $result }

    # 1. Resolve .hwx file
    $hwx = Resolve-HwxFile -ASContentPath $ASContentPath -ModuleType $ModuleType -ModuleVersion $ModuleVersion
    if ($hwx.Version) { $result.moduleVersion = $hwx.Version }
    if ($hwx.HwxFile) { $result.hwxFile       = $hwx.HwxFile }
    if ($hwx.Error)   { $result.error = $hwx.Error; return $result }

    try { [xml]$hwxXml = Get-Content $hwx.HwxFile -Raw -ErrorAction Stop }
    catch { $result.error = "Failed to parse '$($hwx.HwxFile)': $_"; return $result }

    # 2. Resolve target generation (same logic as GetIOMappingConfig)
    if ($Target) {
        $result.resolvedTarget = $Target
        $result.targetSource   = "parameter"
    }
    else {
        $cpuTarget = Get-ProjectCpuTarget -ProjectDir $ProjectPath -ASContentPath $ASContentPath
        $result.resolvedTarget = $cpuTarget.Target
        $result.targetSource   = $cpuTarget.Source
        $result.cpu            = $cpuTarget.Cpu
    }

    # 3. Find the best-matching <Properties> block.
    #    Priority: exact Target match > no-Target block (drives) > first block.
    $allProps = @($hwxXml.SelectNodes('/*[local-name()="Module"]/*[local-name()="Properties"]'))
    if ($allProps.Count -eq 0) {
        $result.error = "No <Properties> element found in '$($hwx.HwxFile)'."
        return $result
    }

    $propsNode = $null
    # Try exact target match
    foreach ($p in $allProps) {
        $pt = $p.GetAttribute('Target')
        if ($pt -and $pt -ieq $result.resolvedTarget) { $propsNode = $p; break }
    }
    # Fallback: no-Target block (drives, panels with no SG target restriction)
    if (-not $propsNode) {
        foreach ($p in $allProps) {
            if ([string]::IsNullOrEmpty($p.GetAttribute('Target'))) { $propsNode = $p; break }
        }
    }
    # Fallback: first block
    if (-not $propsNode) { $propsNode = $allProps[0] }

    # 3b. Resolve the Technology-Package config file(s) that define this module's Location-based
    #     sub-config namespaces (e.g. ACOPOS DriveConfiguration). Null for modules that don't use it.
    $result.locationConfig = Resolve-LocationConfigLink -PropsNode $propsNode `
        -ASContentPath $ASContentPath -ProjectDir $ProjectPath -HwxFile $hwx.HwxFile

    # 4a. Extract in-document <Macro ID="..."> definitions (panel/terminal .hwx files embed
    #     their macro fragments directly in the same file, referenced via <Include Macro="..."/>).
    $hwxMacroMap = @{}
    foreach ($macroNode in @($hwxXml.SelectNodes('/*[local-name()="Module"]/*[local-name()="Macros"]/*[local-name()="Macro"]'))) {
        $macroId = $macroNode.GetAttribute('ID')
        if (-not [string]::IsNullOrEmpty($macroId) -and -not $hwxMacroMap.ContainsKey($macroId)) {
            $hwxMacroMap[$macroId] = $macroNode
        }
    }

    # 4b. Clone <Properties> into a standalone document so the engine can modify it safely.
    $doc  = New-Object System.Xml.XmlDocument
    $doc.XmlResolver = $null
    $clone = $doc.ImportNode($propsNode, $true)
    [void]$doc.AppendChild($clone)
    $root = $doc.DocumentElement

    # 5. Assemble: always resolve includes + templates + version filter.
    #    In-document macros (from $hwxMacroMap) are spliced in directly.
    #    External cross-references (<Include File="ArCfg..."/>) are pulled from AS/Hardware/.
    #    AS-internal fragments (a few File= groups such as ArCfgGroupOPC/Communication) that
    #    are not shipped as standalone XML are reported in 'unresolvedIncludes'.
    $hwVersion   = $hwx.Version
    $hardwareDir = Join-Path $ASContentPath "Hardware"

    # 5a. Build the per-dimension version maps for modules that participate in a TP sub-config
    #     namespace (AsHwAcLocation). Properties of such modules gate on FromVersion<N>/ToVersion<N>
    #     where N is a TP/AR/AS dimension (e.g. FromVersion3 = mappMotion). A single scalar version
    #     cannot satisfy these, so we map each suffix to its resolved dimension version. Modules
    #     without a namespace (e.g. the CPU) keep the original scalar behaviour (empty maps).
    $lc = $result.locationConfig
    $verMap   = @{}
    $tokenMap = @{}
    if ($lc -and $lc.dimensionVersions) {
        foreach ($n in $lc.dimensionVersions.Keys) {
            $v = $lc.dimensionVersions[$n].version
            if ($v) { $tokenMap[$n] = $v }
        }
        $verMap = @{ '' = $hwVersion }
        foreach ($k in $tokenMap.Keys) { $verMap[$k] = $tokenMap[$k] }
    }

    # Pre-seed global template definitions (ArCfgTemplates*.xml)
    $tplMap = New-PgGlobalTemplateMap -IncludeRoot $hardwareDir

    $root = Get-AssembledPropertiesRoot -EntryNode $root -IncludeRoot $hardwareDir `
        -Version $hwVersion -TemplateMap $tplMap -MacroMap $hwxMacroMap `
        -VersionMap $verMap -VersionTokenMap $tokenMap

    $result.unresolvedIncludes = @(Get-PgUnresolvedIncludes)

    # 5b. Assemble and graft externally-linked Location sub-config namespaces (e.g. ACOPOS
    #     DriveConfiguration) so they are browsable through the same -ParentElement drill-down.
    #     The definition lives in a TP editor file whose <Include>s are version-templated
    #     (mappMotion/{Version3}/Editors/...) and rooted at AS/TechnologyPackages; they are resolved
    #     with the maps built above. Inline namespaces (e.g. motors -> M1) already live in the grid.
    $nsFileCount = if ($lc -and $lc.namespaceFiles) { [int]$lc.namespaceFiles.Count } else { 0 }
    if ($lc -and -not $lc.definedInline -and $nsFileCount -gt 0) {
        $tpDir = Join-Path $ASContentPath "TechnologyPackages"
        $nsUnresolved = New-Object System.Collections.Generic.List[string]
        foreach ($nf in $lc.namespaceFiles) {
            if ($nf.file -ieq $hwx.HwxFile) { continue }   # inline roots are already in the grid
            $nsDoc = $null
            try { $nsDoc = Read-PgXmlFile $nf.file } catch { continue }
            $nsRoot = Get-AssembledPropertiesRoot -EntryNode $nsDoc.DocumentElement -IncludeRoot $tpDir `
                -Version $hwVersion -TemplateMap @{} -EntryFilePath $nf.file `
                -VersionMap $verMap -VersionTokenMap $tokenMap
            foreach ($u in (Get-PgUnresolvedIncludes)) { [void]$nsUnresolved.Add($u) }
            # Graft each ArAsHwAc namespace root group (e.g. DriveConfiguration) under the grid root.
            foreach ($g in @($nsRoot.SelectNodes('./*[local-name()="Group"][@ArAsHwAc="true"]'))) {
                $imp = $doc.ImportNode($g, $true)
                [void]$root.AppendChild($imp)
            }
        }
        if ($nsUnresolved.Count -gt 0) {
            $merged = @($result.unresolvedIncludes) + @($nsUnresolved)
            $result.unresolvedIncludes = @($merged | Select-Object -Unique)
        }
    }

    # Filter out TP-link includes that were tracked as "unresolved" during .hwx grid assembly
    # simply because the file doesn't exist under AS/Hardware — they are resolved by step 5b.
    if ($lc -and $lc.entryFile) {
        $resolvedFileNames = New-Object System.Collections.Generic.HashSet[string]
        [void]$resolvedFileNames.Add([System.IO.Path]::GetFileName($lc.entryFile).ToLower())
        foreach ($nf in $lc.namespaceFiles) {
            [void]$resolvedFileNames.Add([System.IO.Path]::GetFileName($nf.file).ToLower())
        }
        $result.unresolvedIncludes = @($result.unresolvedIncludes | Where-Object {
            $name = ($_ -replace '^File:', '').ToLower()
            -not $resolvedFileNames.Contains($name)
        })
    }

    # 6. Navigate to ParentElement scope. Internal/hidden parameters are always included.
    $scope = $null
    try { $scope = Find-PgScopeNode -Root $root -ParentElement $ParentElement }
    catch {
        $result.error = $_.Exception.Message
        return $result
    }

    # 7. Walk the tree and emit JSON models
    $pg = Get-PropertyGridElements -Root $root -ScopeNode $scope `
        -ParentElement $ParentElement -MaxDepth $MaxDepth -MaxElements $MaxElements

    $result.emittedCount = $pg.emittedCount
    $result.elements     = $pg.elements
    $result.success      = $true
    $result.xmlAuthoring = Get-XmlAuthoringGuide -Profile 'hardware' -ModuleType $ModuleType -ModuleVersion $result.moduleVersion
    if ($pg.truncated) {
        $result.truncated = $true
        $result.hint      = $pg.hint
    }
    return $result
}

#endregion

#region GetPermittedHwClassifications Orchestrator

function Invoke-GetPermittedHwClassifications {
    param(
        [string]$ASContentPath
    )

    $result = [ordered]@{
        success       = $false
        asContentPath = $ASContentPath
        catalogFile   = $null
        count         = 0
        values        = @()
        error         = $null
    }

    if (-not $ASContentPath) {
        $result.error = "AS content path could not be resolved."
        return $result
    }

    $catalogFile = Join-Path (Join-Path $ASContentPath "Hardware") "HardwareCatalogLayout.xml"
    $result.catalogFile = $catalogFile

    if (-not (Test-Path $catalogFile -PathType Leaf)) {
        $result.error = "Hardware catalog file not found: $catalogFile"
        return $result
    }

    $content = $null
    try {
        $content = Get-Content $catalogFile -Raw -ErrorAction Stop
    }
    catch {
        $result.error = "Failed to read '$catalogFile': $_"
        return $result
    }

    $rx  = [regex]'(?i)\bValue\s*=\s*"([^"]+)"'
    $set = New-Object System.Collections.Generic.HashSet[string] ([System.StringComparer]::OrdinalIgnoreCase)

    foreach ($m in $rx.Matches($content)) {
        $v = $m.Groups[1].Value
        if (-not [string]::IsNullOrWhiteSpace($v)) {
            [void]$set.Add($v)
        }
    }

    $result.values  = @($set) | Sort-Object
    $result.count   = $result.values.Count
    $result.success = $true
    return $result
}

#endregion

#region Main Logic

$ProjectPath = Resolve-Path $ProjectPath
$projectFile = Get-ProjectFile -ProjectDir $ProjectPath
$projectName = [System.IO.Path]::GetFileNameWithoutExtension($projectFile)

Write-Banner "B&R AS Physical HW API"
Write-Host "Project: $projectName"
Write-Host "Path:    $ProjectPath"
Write-Host "Action:  $Action"

if ($Action -eq "AddHWModule") {
    Write-Host "HwModuleType: $HwModuleType"
    if ($HwModuleVersion) { Write-Host "HwModuleVersion: $HwModuleVersion" }
    Write-Host "HwFile: $HwFileName"
}
elseif ($Action -eq "SearchHWModule") {
    Write-Host "HwModuleType (search): $HwModuleType"
}
elseif ($Action -eq "SearchByClassification") {
    Write-Host "ClassificationKeyword (search): $ClassificationKeyword"
}
elseif ($Action -eq "GetHWModuleInfo") {
    Write-Host "HwModuleType: $HwModuleType"
    if ($HwModuleVersion) { Write-Host "HwModuleVersion: $HwModuleVersion" }
}
elseif ($Action -eq "GetIOMappingConfig") {
    Write-Host "HwModuleType: $HwModuleType"
    if ($HwModuleVersion) { Write-Host "HwModuleVersion: $HwModuleVersion" }
    if ($Target) { Write-Host "Target: $Target" }
    if ($ResolveIncludes) { Write-Host "ResolveIncludes: true" }
}
elseif ($Action -eq "GetPossibleHWConnections") {
    Write-Host "HwModuleType: $HwModuleType"
    if ($HwModuleVersion) { Write-Host "HwModuleVersion: $HwModuleVersion" }
}
elseif ($Action -eq "GetModuleProperties") {
    Write-Host "HwModuleType: $HwModuleType"
    if ($HwModuleVersion) { Write-Host "HwModuleVersion: $HwModuleVersion" }
    if ($Target)          { Write-Host "Target: $Target" }
    if ($ParentElement -and $ParentElement -ne "Root") { Write-Host "ParentElement: $ParentElement" }
    Write-Host "MaxDepth: $MaxDepth  MaxElements: $MaxElements"
}
elseif ($Action -eq "GetPermittedHwClassifications") {
    # No additional parameters needed
}

$selectedAS    = Resolve-ASInstallation -ProjectFile $projectFile
$asContentPath = Get-ASContentPath -ASPath $selectedAS.Path

Write-Host ("=" * 60) -ForegroundColor Yellow

$exitCode = 0

switch ($Action) {
    "AddHWModule" {
        if (-not $HwModuleType) { Write-Failure "HwModuleType parameter is required."; exit 1 }

        $addHwResult = Invoke-AddHWModule `
            -ProjectPath    $ProjectPath `
            -ASContentPath  $asContentPath `
            -HwModuleType   $HwModuleType `
            -HwModuleVersion $HwModuleVersion `
            -HwFileName     $HwFileName

        Write-Output (ConvertTo-AsJson -InputObject $addHwResult -Depth 5)

        if ($addHwResult.success) {
            if ($addHwResult.skipped) {
                Write-Success "AddHWModule: module already exists in Hardware.hw"
            }
            else {
                Write-Success "AddHWModule completed: $($addHwResult.moduleName) ($($addHwResult.moduleType) v$($addHwResult.moduleVersion))"
            }
        }
        else {
            Write-Failure "AddHWModule failed: $($addHwResult.error)"
            $exitCode = 1
        }
    }
    "SearchHWModule" {
        if (-not $HwModuleType) { Write-Failure "HwModuleType parameter is required."; exit 1 }

        $searchResult = Invoke-SearchHWModule -ASContentPath $asContentPath -SearchPattern $HwModuleType

        Write-Output (ConvertTo-AsJson -InputObject $searchResult -Depth 5)

        if (-not $searchResult.success) {
            Write-Failure "SearchHWModule failed: $($searchResult.error)"
            $exitCode = 1
        }
    }
    "SearchByClassification" {
        if (-not $ClassificationKeyword) { Write-Failure "ClassificationKeyword parameter is required."; exit 1 }

        $classResult = Invoke-SearchByClassification -ASContentPath $asContentPath -Keyword $ClassificationKeyword

        Write-Output (ConvertTo-AsJson -InputObject $classResult -Depth 5)

        if ($classResult.success) {
            Write-Success "SearchByClassification completed: $($classResult.matchCount) module(s)."
        }
        else {
            Write-Failure "SearchByClassification failed: $($classResult.error)"
            $exitCode = 1
        }
    }
    "GetHWModuleInfo" {
        if (-not $HwModuleType) { Write-Failure "HwModuleType parameter is required."; exit 1 }

        $infoResult = Invoke-GetHWModuleInfo `
            -ASContentPath $asContentPath `
            -ModuleType    $HwModuleType `
            -ModuleVersion $HwModuleVersion

        Write-Output (ConvertTo-AsJson -InputObject $infoResult -Depth 5)

        if ($infoResult.success) {
            Write-Host ""
            Write-Host "Module : $($infoResult.moduleType) v$($infoResult.moduleVersion)" -ForegroundColor White
            Write-Host "HWX    : $($infoResult.hwxFile)" -ForegroundColor Gray
            Write-Host "Desc   : $($infoResult.description)" -ForegroundColor Cyan
            if ($infoResult.classification -and $infoResult.classification.Count -gt 0) {
                Write-Host "Classification:" -ForegroundColor White
                foreach ($key in $infoResult.classification.Keys) {
                    $values = $infoResult.classification[$key] -join ', '
                    Write-Host "  $key`: $values" -ForegroundColor Gray
                }
            }
            Write-Success "GetHWModuleInfo completed."
        }
        else {
            Write-Host ""
            Write-Host "HELP: Module type '$HwModuleType' not found." -ForegroundColor Cyan
            Write-Host "Verify the module type using SearchHWModule first:" -ForegroundColor Cyan
            Write-Host "  1. Use 'mcp_b_r_automatio_search_help' to identify the product family and base prefix"
            Write-Host "  2. Search for the module: SearchHWModule -HwModuleType ""8LSA"""
            Write-Host "  3. Narrow progressively: SearchHWModule -HwModuleType ""8LSA45"""
            Write-Host "  4. Once you find the exact module type, try GetHWModuleInfo again"
            Write-Host ""
            Write-Failure "GetHWModuleInfo failed: $($infoResult.error)"
            $exitCode = 1
        }
    }
    "GetIOMappingConfig" {
        if (-not $HwModuleType) { Write-Failure "HwModuleType parameter is required."; exit 1 }

        $ioResult = Invoke-GetIOMappingConfig `
            -ProjectPath     $ProjectPath `
            -ASContentPath   $asContentPath `
            -ModuleType      $HwModuleType `
            -ModuleVersion   $HwModuleVersion `
            -Target          $Target `
            -ResolveIncludes:$ResolveIncludes

        Write-Output (ConvertTo-AsJson -InputObject $ioResult -Depth 8)

        if ($ioResult.success) {
            Write-Host ""
            Write-Host "Module : $($ioResult.moduleType) v$($ioResult.moduleVersion)" -ForegroundColor White
            Write-Host "Target : $($ioResult.resolvedTarget) (source: $($ioResult.targetSource))" -ForegroundColor Cyan
            Write-Host "Channels: $($ioResult.channelCount)" -ForegroundColor White
            foreach ($ch in $ioResult.channels) {
                $dir = if ($ch.direction) { $ch.direction } else { '?' }
                $dt  = if ($ch.dataType) { $ch.dataType } else { '?' }
                Write-Host ("  {0,-26} {1,-6} {2,-5} {3}" -f $ch.channelName, $dt, $dir, $ch.description) -ForegroundColor Gray
            }
            Write-Success "GetIOMappingConfig completed: $($ioResult.channelCount) channel(s)."
        }
        else {
            Write-Host ""
            Write-Host "HELP: Verify the module type with SearchHWModule / GetHWModuleInfo first." -ForegroundColor Cyan
            Write-Failure "GetIOMappingConfig failed: $($ioResult.error)"
            $exitCode = 1
        }
    }
    "GetPossibleHWConnections" {
        if (-not $HwModuleType) { Write-Failure "HwModuleType parameter is required."; exit 1 }

        $connResult = Invoke-GetPossibleHWConnections `
            -ASContentPath $asContentPath `
            -ModuleType    $HwModuleType `
            -ModuleVersion $HwModuleVersion

        Write-Output (ConvertTo-AsJson -InputObject $connResult -Depth 8)

        if ($connResult.success) {
            Write-Host ""
            Write-Host "Module : $($connResult.moduleType) v$($connResult.moduleVersion)" -ForegroundColor White
            Write-Host "Connectors: $($connResult.connectorCount)" -ForegroundColor White
            foreach ($conn in $connResult.connectors) {
                $types = if ($conn.connectionTypes) { $conn.connectionTypes -join ', ' } else { '' }
                Write-Host ("  {0,-6} {1,-7} [{2}]" -f $conn.name, $conn.type, $types) -ForegroundColor Gray
                foreach ($ac in $conn.autoConnect) {
                    Write-Host ("      auto-> {0}.{1}" -f $ac.targetModule, $ac.targetConnector) -ForegroundColor DarkGray
                }
            }
            Write-Success "GetPossibleHWConnections completed: $($connResult.connectorCount) connector(s)."
        }
        else {
            Write-Host ""
            Write-Host "HELP: Verify the module type with SearchHWModule / GetHWModuleInfo first." -ForegroundColor Cyan
            Write-Failure "GetPossibleHWConnections failed: $($connResult.error)"
            $exitCode = 1
        }
    }
    "GetModuleProperties" {
        if (-not $HwModuleType) { Write-Failure "HwModuleType parameter is required."; exit 1 }

        $mpResult = Invoke-GetModuleProperties `
            -ProjectPath     $ProjectPath `
            -ASContentPath   $asContentPath `
            -ModuleType      $HwModuleType `
            -ModuleVersion   $HwModuleVersion `
            -Target          $Target `
            -ParentElement   $ParentElement `
            -MaxDepth        $MaxDepth `
            -MaxElements     $MaxElements

        Write-Output (ConvertTo-AsJson -InputObject $mpResult -Depth 64)

        if (-not $mpResult.success) {
            Write-Host ""
            Write-Host "HELP: Verify the module type with SearchHWModule / GetHWModuleInfo first." -ForegroundColor Cyan
            if ($mpResult.error -match 'ParentElement segment') {
                Write-Host "HINT: Run without -ParentElement (or with Root) to see top-level elements, then drill in." -ForegroundColor Cyan
            }
            Write-Failure "GetModuleProperties failed: $($mpResult.error)"
            $exitCode = 1
        }
    }
    "GetPermittedHwClassifications" {
        $classificationsResult = Invoke-GetPermittedHwClassifications -ASContentPath $asContentPath

        Write-Output (ConvertTo-AsJson -InputObject $classificationsResult -Depth 6)

        if ($classificationsResult.success) {
            Write-Success "GetPermittedHwClassifications completed: $($classificationsResult.count) value(s)."
        }
        else {
            Write-Failure "GetPermittedHwClassifications failed: $($classificationsResult.error)"
            $exitCode = 1
        }
    }
}

exit $exitCode

#endregion
