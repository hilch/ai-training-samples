<#
.SYNOPSIS
    Manage B&R Automation Studio Logical view: add libraries and program tasks.

.DESCRIPTION
    Actions:
    - AddLibrary  : Resolve and deploy a library (+ transitive deps) from the AS installation
                    into Logical/Libraries/, update Package.pkg, Cpu.sw, and .apj TechPackages.
    - AddSTTask   : Create a Structured Text program task folder under Logical/<TaskPath>/<TaskName>,
                    write the 4 standard IEC files, register in Package.pkg hierarchy, and add a
                    Task entry to the active configuration's .sw file.
    - AddAnsiCTask: Create an ANSI C program task folder under Logical/<TaskPath>/<TaskName>,
                    write the standard ANSI C files, register in Package.pkg hierarchy, and add a
                    Task entry to the active configuration's .sw file.

.PARAMETER ProjectPath
    Path to the Automation Studio project directory (containing .apj file).

.PARAMETER Action
    The action to perform: AddLibrary | AddSTTask | AddAnsiCTask
    Default: AddLibrary

.PARAMETER LibraryName
    Name of the library to add (required when Action=AddLibrary).

.PARAMETER MinVersion
    Minimum version constraint for the library (optional).

.PARAMETER TaskPath
    Folder path relative to Logical/ where the task will be created (required when Action=AddSTTask or AddAnsiCTask).
    Examples: "Drive", "Service/SubFolder"

.PARAMETER TaskName
    Name of the program task to create, max 10 characters (required when Action=AddSTTask or AddAnsiCTask).

.PARAMETER TaskClass
    Cyclic task class 1-8 for Cpu.sw registration (optional, default 4).

.PARAMETER SwFileName
    Name of the .sw file to register the task in (optional, default "Cpu.sw").

.EXAMPLE
    .\use-as-logical-api.ps1 -ProjectPath . -Action AddLibrary -LibraryName MTBasics

.EXAMPLE
    .\use-as-logical-api.ps1 -ProjectPath . -Action AddLibrary -LibraryName McAxis -MinVersion 6.0.0

.EXAMPLE
    .\use-as-logical-api.ps1 -ProjectPath . -Action AddSTTask -TaskPath Drive -TaskName ConvCtrl

.EXAMPLE
    .\use-as-logical-api.ps1 -ProjectPath . -Action AddSTTask -TaskPath Service/Sub -TaskName Ctrl -TaskClass 2

.EXAMPLE
    .\use-as-logical-api.ps1 -ProjectPath . -Action AddAnsiCTask -TaskPath Service -TaskName TelnetCli -TaskClass 4
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [ValidateScript({ Test-Path $_ -PathType Container })]
    [string]$ProjectPath,

    [Parameter()]
    [ValidateSet("AddLibrary", "AddSTTask", "AddAnsiCTask")]
    [string]$Action = "AddLibrary",

    [Parameter()]
    [string]$LibraryName,

    [Parameter()]
    [string]$MinVersion = "",

    [Parameter()]
    [string]$TaskPath = "",

    [Parameter()]
    [ValidateLength(1, 10)]
    [string]$TaskName = "",

    [Parameter()]
    [ValidateRange(1, 8)]
    [int]$TaskClass = 4,

    [Parameter()]
    [string]$SwFileName = "Cpu.sw"
)

# Dot-source shared utilities
. "$PSScriptRoot/../../as-shared/scripts/as-shared.ps1"

#region Library Search and Resolution

function Get-ASLibrarySearchPaths {
    <#
    .SYNOPSIS
        Builds the list of library search paths from AS installation.
    #>
    param([string]$ASContentPath)

    $searchPaths = @()

    if (-not (Test-Path $ASContentPath)) {
        Write-Warning "AS content path not found: $ASContentPath"
        return $searchPaths
    }

    # Add Library, Library_2, Library_3, etc.
    Get-ChildItem -Path $ASContentPath -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -match '^Library(_\d+)?$' } |
        ForEach-Object {
            $searchPaths += @{ Path = $_.FullName; TechPackageName = $null; TechPackageVersion = $null }
        }

    # Add TechnologyPackages/<tp>/<version>/Library/ paths
    $tpRoot = Join-Path $ASContentPath "TechnologyPackages"
    if (Test-Path $tpRoot) {
        foreach ($tp in Get-ChildItem -Path $tpRoot -Directory -ErrorAction SilentlyContinue) {
            foreach ($ver in Get-ChildItem -Path $tp.FullName -Directory -ErrorAction SilentlyContinue) {
                if ($ver.Name -notmatch '^[Vv]?\d+(\.\d+)*$') { continue }
                $libDir = Join-Path $ver.FullName "Library"
                if (Test-Path $libDir) {
                    $searchPaths += @{
                        Path               = $libDir
                        TechPackageName    = $tp.Name
                        TechPackageVersion = $ver.Name -replace '^[Vv]', ''
                    }
                }
            }
        }
    }

    return $searchPaths
}

function Get-ApjTechnologyPackages {
    <#
    .SYNOPSIS
        Parses the .apj file and returns existing technology packages as a hashtable.
    #>
    param([string]$ApjFile)

    $result = @{}
    $content = Get-Content $ApjFile -Raw

    if ($content -match '<TechnologyPackages>([\s\S]*?)</TechnologyPackages>') {
        $tpBlock = $matches[1]
        $entryPattern = '<(\w+)\s+Version="([^"]+)"\s*/>'
        $tpBlock | Select-String -Pattern $entryPattern -AllMatches | ForEach-Object {
            foreach ($m in $_.Matches) {
                $result[$m.Groups[1].Value.ToLower()] = $m.Groups[2].Value
            }
        }
    }

    return $result
}

function Resolve-LibraryVersion {
    <#
    .SYNOPSIS
        Finds the best (highest) version folder for a library under a base path.
    #>
    param(
        [string]$LibBasePath,
        [string]$MinVersion = ""
    )

    if (-not (Test-Path $LibBasePath)) { return $null }

    $versionDirs = Get-ChildItem -Path $LibBasePath -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -match '^[Vv]?\d+(\.\d+)*$' }

    # No versioned subfolders — content directly in base folder
    if ($versionDirs.Count -eq 0) {
        $lbyFiles = Get-ChildItem -Path $LibBasePath -Filter "*.lby" -File -ErrorAction SilentlyContinue
        if ($lbyFiles.Count -gt 0) { return $LibBasePath }
        return $null
    }

    # Filter by MinVersion if specified
    $candidates = @()
    foreach ($dir in $versionDirs) {
        $ver = $dir.Name -replace '^[Vv]', ''
        if ($MinVersion -and (Compare-Versions $ver $MinVersion) -lt 0) { continue }
        $candidates += @{ Path = $dir.FullName; Version = $ver }
    }

    if ($candidates.Count -eq 0) { return $null }

    $best = $candidates[0]
    foreach ($c in $candidates) {
        if ((Compare-Versions $c.Version $best.Version) -gt 0) { $best = $c }
    }
    return $best.Path
}

function Get-LibraryLanguage {
    <#
    .SYNOPSIS
        Detects the library language from the .lby filename stem.
    #>
    param([string]$LibContentPath)

    $lbyFile = Get-ChildItem -Path $LibContentPath -Filter "*.lby" -File -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($lbyFile) {
        return [System.IO.Path]::GetFileNameWithoutExtension($lbyFile.Name)
    }
    return "IEC"
}

function Get-LibraryVersion {
    <#
    .SYNOPSIS
        Extracts the library version from the .lby file or folder name.
    #>
    param([string]$LibContentPath)

    $lbyFile = Get-ChildItem -Path $LibContentPath -Filter "*.lby" -File -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($lbyFile) {
        $content = Get-Content $lbyFile.FullName -Raw -ErrorAction SilentlyContinue
        if ($content -match '<Library\s+[^>]*Version="([^"]+)"') {
            return $matches[1]
        }
    }

    $folderName = Split-Path $LibContentPath -Leaf
    return $folderName -replace '^[Vv]', ''
}

function Find-Library {
    <#
    .SYNOPSIS
        Searches all library search paths for a named library.
    .DESCRIPTION
        Mapp-aware: respects pinned technology package versions from the project .apj.
    #>
    param(
        [string]$LibraryName,
        [string]$MinVersion,
        [array]$SearchPaths,
        [hashtable]$ExistingTechPackages
    )

    $tpPaths      = $SearchPaths | Where-Object { $_.TechPackageName }
    $regularPaths = $SearchPaths | Where-Object { -not $_.TechPackageName }

    $tpGroups = @{}
    foreach ($sp in $tpPaths) {
        $tpKey = $sp.TechPackageName.ToLower()
        if (-not $tpGroups.ContainsKey($tpKey)) { $tpGroups[$tpKey] = @() }
        $tpGroups[$tpKey] += $sp
    }

    foreach ($tpName in $tpGroups.Keys) {
        $tpSearchPaths  = $tpGroups[$tpName]
        $pinnedVersion  = $ExistingTechPackages[$tpName]

        if ($pinnedVersion) {
            $pinnedPath = $tpSearchPaths | Where-Object { $_.TechPackageVersion -eq $pinnedVersion } | Select-Object -First 1
            if (-not $pinnedPath) { continue }

            $libBase  = Join-Path $pinnedPath.Path $LibraryName
            $resolved = Resolve-LibraryVersion -LibBasePath $libBase -MinVersion $MinVersion
            if ($resolved) {
                return @{
                    Name               = $LibraryName
                    Version            = Get-LibraryVersion -LibContentPath $resolved
                    SourcePath         = $resolved
                    Language           = Get-LibraryLanguage -LibContentPath $resolved
                    TechPackageName    = $tpSearchPaths[0].TechPackageName
                    TechPackageVersion = $pinnedVersion
                    TechPackagePinned  = $true
                }
            }
            elseif (Test-Path (Join-Path $pinnedPath.Path $LibraryName)) {
                return @{ Error = "Technology package '$($tpSearchPaths[0].TechPackageName)' is pinned at version $pinnedVersion in the project. Library '$LibraryName' >= $MinVersion is not available in that version." }
            }
        }
        else {
            $bestResult    = $null
            $bestTpVersion = $null
            foreach ($sp in $tpSearchPaths) {
                $libBase  = Join-Path $sp.Path $LibraryName
                $resolved = Resolve-LibraryVersion -LibBasePath $libBase -MinVersion $MinVersion
                if ($resolved) {
                    if (-not $bestTpVersion -or (Compare-Versions $sp.TechPackageVersion $bestTpVersion) -gt 0) {
                        $bestTpVersion = $sp.TechPackageVersion
                        $bestResult    = $resolved
                    }
                }
            }
            if ($bestResult) {
                $matchingSp = $tpSearchPaths | Where-Object { $_.TechPackageVersion -eq $bestTpVersion } | Select-Object -First 1
                return @{
                    Name               = $LibraryName
                    Version            = Get-LibraryVersion -LibContentPath $bestResult
                    SourcePath         = $bestResult
                    Language           = Get-LibraryLanguage -LibContentPath $bestResult
                    TechPackageName    = $matchingSp.TechPackageName
                    TechPackageVersion = $bestTpVersion
                    TechPackagePinned  = $false
                }
            }
        }
    }

    foreach ($sp in $regularPaths) {
        $libBase  = Join-Path $sp.Path $LibraryName
        $resolved = Resolve-LibraryVersion -LibBasePath $libBase -MinVersion $MinVersion
        if ($resolved) {
            return @{
                Name               = $LibraryName
                Version            = Get-LibraryVersion -LibContentPath $resolved
                SourcePath         = $resolved
                Language           = Get-LibraryLanguage -LibContentPath $resolved
                TechPackageName    = $null
                TechPackageVersion = $null
                TechPackagePinned  = $false
            }
        }
    }

    return $null
}

#endregion

#region Dependency Resolution

function Get-LibraryDependencies {
    <#
    .SYNOPSIS
        Parses .lby files for <Dependency> entries.
    #>
    param([string]$LibContentPath)

    $deps    = @()
    $lbyFiles = Get-ChildItem -Path $LibContentPath -Filter "*.lby" -File -ErrorAction SilentlyContinue

    foreach ($lbyFile in $lbyFiles) {
        $content = Get-Content $lbyFile.FullName -Raw -ErrorAction SilentlyContinue
        if (-not $content) { continue }

        $depPattern = '<Dependency\s+([^>]+)/>'
        $depMatches = [regex]::Matches($content, $depPattern)
        foreach ($m in $depMatches) {
            $attrs = $m.Groups[1].Value
            if ($attrs -match 'ObjectName\s*=\s*"([^"]+)"') {
                $objName = $matches[1]
                $fromVer = $null
                $toVer   = $null
                if ($attrs -match 'FromVersion\s*=\s*"([^"]+)"') { $fromVer = $matches[1] }
                if ($attrs -match 'ToVersion\s*=\s*"([^"]+)"')   { $toVer   = $matches[1] }
                $deps += @{ ObjectName = $objName; FromVersion = $fromVer; ToVersion = $toVer }
            }
        }
    }

    return $deps
}

function Resolve-LibraryWithDependencies {
    <#
    .SYNOPSIS
        BFS resolution of a library and all transitive dependencies.
    #>
    param(
        [string]$LibraryName,
        [string]$MinVersion,
        [array]$SearchPaths,
        [hashtable]$ExistingTechPackages,
        [string]$ProjectLibDir
    )

    $resolved      = @()
    $skipped       = @()
    $warnings      = @()
    $techPackages  = @{}
    $resolvedNames = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)

    if (Test-Path $ProjectLibDir) {
        foreach ($dir in Get-ChildItem -Path $ProjectLibDir -Directory -ErrorAction SilentlyContinue) {
            $resolvedNames.Add($dir.Name) | Out-Null
        }
    }

    $queue = [System.Collections.Queue]::new()
    $queue.Enqueue(@{ Name = $LibraryName; MinVersion = $MinVersion })

    while ($queue.Count -gt 0) {
        $item   = $queue.Dequeue()
        $name   = $item.Name
        $minVer = $item.MinVersion

        if ($resolvedNames.Contains($name)) {
            $existsInProject = Test-Path (Join-Path $ProjectLibDir $name)
            if ($existsInProject -and ($resolved | Where-Object { $_.Name -eq $name }).Count -eq 0) {
                $skipped += @{ Name = $name; Reason = "already exists in project" }
            }
            continue
        }
        $resolvedNames.Add($name) | Out-Null

        $lib = Find-Library -LibraryName $name -MinVersion $minVer -SearchPaths $SearchPaths -ExistingTechPackages $ExistingTechPackages

        if (-not $lib) {
            $warnings += "Library '$name' (>= $minVer) not found in AS installation"
            continue
        }
        if ($lib.Error) {
            return @{ Libraries = @(); TechPackages = @{}; Skipped = @(); Warnings = @(); Error = $lib.Error }
        }

        $resolved += $lib
        Write-Host "  Resolved: $($lib.Name) v$($lib.Version) from $($lib.SourcePath)" -ForegroundColor Gray

        if ($lib.TechPackageName) {
            $tpKey = $lib.TechPackageName.ToLower()
            if (-not $techPackages.ContainsKey($tpKey)) {
                $techPackages[$tpKey] = @{ Name = $lib.TechPackageName; Version = $lib.TechPackageVersion }
            }
        }

        $deps = Get-LibraryDependencies -LibContentPath $lib.SourcePath
        foreach ($dep in $deps) {
            if (-not $resolvedNames.Contains($dep.ObjectName)) {
                $depMin = ""
                if ($dep.FromVersion -and $dep.FromVersion -ne "0.0.0") { $depMin = $dep.FromVersion }
                $queue.Enqueue(@{ Name = $dep.ObjectName; MinVersion = $depMin })
            }
        }
    }

    return @{ Libraries = $resolved; TechPackages = $techPackages; Skipped = $skipped; Warnings = $warnings; Error = $null }
}

#endregion

#region Library Deployment

function Install-Library {
    param(
        [string]$SourcePath,
        [string]$LibraryName,
        [string]$ProjectLibDir
    )

    $destDir = Join-Path $ProjectLibDir $LibraryName
    if (Test-Path $destDir) {
        Write-Host "  Library '$LibraryName' already deployed, skipping." -ForegroundColor Yellow
        return $false
    }

    Copy-Item -Path $SourcePath -Destination $destDir -Recurse -Force
    return $true
}

function Update-LibrariesPackagePkg {
    param(
        [string]$ProjectLibDir,
        [array]$Libraries
    )

    $pkgPath = Join-Path $ProjectLibDir "Package.pkg"
    if (-not (Test-Path $pkgPath)) {
        $entries = ($Libraries | ForEach-Object {
            $lang = if ($_.Language -eq 'IEC') { 'IEC' } else { 'binary' }
            "    <Object Type=`"Library`" Language=`"$lang`">$($_.Name)</Object>"
        }) -join "`n"

        $content = @"
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
$entries
  </Objects>
</Package>
"@
        New-Item -ItemType Directory -Path $ProjectLibDir -Force | Out-Null
        [System.IO.File]::WriteAllText($pkgPath, $content, $script:Utf8NoBom)
        return
    }

    $content = Get-Content $pkgPath -Raw
    $existingLibs = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
    $objPattern = '<Object\s[^>]*>([^<]+)</Object>'
    [regex]::Matches($content, $objPattern) | ForEach-Object {
        $existingLibs.Add($_.Groups[1].Value.Trim()) | Out-Null
    }

    $newEntries = @()
    foreach ($lib in $Libraries) {
        if ($existingLibs.Contains($lib.Name)) { continue }
        $lang = if ($lib.Language -eq 'IEC') { 'IEC' } else { 'binary' }
        $newEntries += "    <Object Type=`"Library`" Language=`"$lang`">$($lib.Name)</Object>"
    }

    if ($newEntries.Count -eq 0) { return }

    $insertPoint = $content.IndexOf('</Objects>')
    if ($insertPoint -eq -1) { Write-Warning "Could not find </Objects> in Package.pkg"; return }

    $content = $content.Substring(0, $insertPoint) + ($newEntries -join "`n") + "`n" + $content.Substring($insertPoint)
    [System.IO.File]::WriteAllText($pkgPath, $content, $script:Utf8NoBom)
}

function Update-CpuSwLibraries {
    param(
        [string]$ProjectDir,
        [array]$Libraries
    )

    $physicalDir = Join-Path $ProjectDir "Physical"
    if (-not (Test-Path $physicalDir)) { Write-Warning "Physical directory not found"; return }

    foreach ($config in Get-ChildItem -Path $physicalDir -Directory -ErrorAction SilentlyContinue) {
        foreach ($cpu in Get-ChildItem -Path $config.FullName -Directory -ErrorAction SilentlyContinue) {
            $cpuSwPath = Join-Path $cpu.FullName "Cpu.sw"
            if (-not (Test-Path $cpuSwPath)) { continue }

            Write-Host "  Updating: $($config.Name)/$($cpu.Name)/Cpu.sw" -ForegroundColor Gray
            $content = Get-Content $cpuSwPath -Raw

            $existingLibs = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
            $loPattern = '<LibraryObject\s+[^>]*Name="([^"]+)"'
            [regex]::Matches($content, $loPattern) | ForEach-Object {
                $existingLibs.Add($_.Groups[1].Value) | Out-Null
            }

            $newEntries = @()
            foreach ($lib in $Libraries) {
                if ($existingLibs.Contains($lib.Name)) { continue }
                $source = "Libraries.$($lib.Name).lby"
                $lang   = $lib.Language
                $newEntries += "    <LibraryObject Name=`"$($lib.Name)`" Source=`"$source`" Memory=`"UserROM`" Language=`"$lang`" Debugging=`"true`" />"
            }

            if ($newEntries.Count -eq 0) { continue }

            if ($content -match '<Libraries>([\s\S]*?)</Libraries>') {
                $insertPoint = $content.IndexOf('</Libraries>')
                $content = $content.Substring(0, $insertPoint) + ($newEntries -join "`n") + "`n" + $content.Substring($insertPoint)
            }
            elseif ($content -match '<SwConfiguration[^>]*/\s*>') {
                $libBlock = "  <Libraries>`n" + ($newEntries -join "`n") + "`n  </Libraries>"
                $content  = $content -replace '<SwConfiguration([^>]*)/\s*>', "<SwConfiguration`$1>`n$libBlock`n</SwConfiguration>"
            }
            elseif ($content -match '</SwConfiguration>') {
                $insertPoint = $content.IndexOf('</SwConfiguration>')
                $libBlock    = "  <Libraries>`n" + ($newEntries -join "`n") + "`n  </Libraries>`n"
                $content     = $content.Substring(0, $insertPoint) + $libBlock + $content.Substring($insertPoint)
            }

            [System.IO.File]::WriteAllText($cpuSwPath, $content, $script:Utf8NoBom)
        }
    }
}

function Update-ApjTechnologyPackages {
    param(
        [string]$ApjFile,
        [hashtable]$RequiredTechPackages,
        [hashtable]$ExistingTechPackages
    )

    $added    = @()
    $existing = @()

    if ($RequiredTechPackages.Count -eq 0) {
        return @{ Added = $added; Existing = $existing }
    }

    $content    = Get-Content $ApjFile -Raw
    $newEntries = @()

    foreach ($tpKey in $RequiredTechPackages.Keys) {
        $tp = $RequiredTechPackages[$tpKey]
        if ($ExistingTechPackages.ContainsKey($tpKey)) {
            $existing += @{ Name = $tp.Name; Version = $ExistingTechPackages[$tpKey] }
        }
        else {
            $newEntries += "    <$($tp.Name) Version=`"$($tp.Version)`" />"
            $added      += @{ Name = $tp.Name; Version = $tp.Version }
        }
    }

    if ($newEntries.Count -eq 0) { return @{ Added = $added; Existing = $existing } }

    if ($content -match '<TechnologyPackages>([\s\S]*?)</TechnologyPackages>') {
        $insertPoint = $content.IndexOf('</TechnologyPackages>')
        $content = $content.Substring(0, $insertPoint) + ($newEntries -join "`n") + "`n" + $content.Substring($insertPoint)
    }
    else {
        $tpBlock     = "  <TechnologyPackages>`n" + ($newEntries -join "`n") + "`n  </TechnologyPackages>"
        $insertPoint = $content.IndexOf('</Project>')
        if ($insertPoint -ne -1) {
            $content = $content.Substring(0, $insertPoint) + $tpBlock + "`n" + $content.Substring($insertPoint)
        }
    }

    [System.IO.File]::WriteAllText($ApjFile, $content, $script:Utf8NoBom)
    return @{ Added = $added; Existing = $existing }
}

#endregion

#region AddLibrary Orchestrator

function Invoke-AddLibrary {
    param(
        [string]$ProjectPath,
        [string]$ProjectFile,
        [string]$LibraryName,
        [string]$MinVersion,
        [string]$ASContentPath
    )

    $result = @{
        success                    = $false
        librariesAdded             = @()
        librariesSkipped           = @()
        technologyPackagesAdded    = @()
        technologyPackagesExisting = @()
        warnings                   = @()
        error                      = $null
    }

    if (-not $LibraryName) {
        $result.error = "LibraryName parameter is required for AddLibrary action."
        return $result
    }

    Write-Host "Scanning AS installation for libraries..." -ForegroundColor Cyan
    $searchPaths = Get-ASLibrarySearchPaths -ASContentPath $ASContentPath
    if ($searchPaths.Count -eq 0) {
        $result.error = "No library search paths found in AS installation at: $ASContentPath"
        return $result
    }
    Write-Host "  Found $($searchPaths.Count) search paths" -ForegroundColor Gray

    $existingTP = Get-ApjTechnologyPackages -ApjFile $ProjectFile
    Write-Host "  Existing technology packages: $($existingTP.Count)" -ForegroundColor Gray

    $projectLibDir = Join-Path (Join-Path $ProjectPath "Logical") "Libraries"
    Write-Host "`nResolving library '$LibraryName' (>= $MinVersion)..." -ForegroundColor Cyan

    $resolution = Resolve-LibraryWithDependencies `
        -LibraryName $LibraryName `
        -MinVersion $MinVersion `
        -SearchPaths $searchPaths `
        -ExistingTechPackages $existingTP `
        -ProjectLibDir $projectLibDir

    if ($resolution.Error) { $result.error = $resolution.Error; return $result }

    $result.warnings         = $resolution.Warnings
    $result.librariesSkipped = $resolution.Skipped

    if ($resolution.Libraries.Count -eq 0) {
        if ($resolution.Skipped.Count -gt 0) {
            $result.success = $true
            $result.warnings += "Library '$LibraryName' and all dependencies already exist in project."
        }
        else {
            $result.error = "Library '$LibraryName' (>= $MinVersion) not found in AS installation."
        }
        return $result
    }

    Write-Host "`nDeploying $($resolution.Libraries.Count) libraries..." -ForegroundColor Cyan
    New-Item -ItemType Directory -Path $projectLibDir -Force | Out-Null

    foreach ($lib in $resolution.Libraries) {
        $deployed = Install-Library -SourcePath $lib.SourcePath -LibraryName $lib.Name -ProjectLibDir $projectLibDir
        if ($deployed) {
            $result.librariesAdded += @{ name = $lib.Name; version = $lib.Version; source = $lib.SourcePath }
            Write-Host "  Deployed: $($lib.Name) v$($lib.Version)" -ForegroundColor Green
        }
    }

    Write-Host "`nUpdating Package.pkg..." -ForegroundColor Cyan
    Update-LibrariesPackagePkg -ProjectLibDir $projectLibDir -Libraries $resolution.Libraries

    Write-Host "Updating Cpu.sw files..." -ForegroundColor Cyan
    Update-CpuSwLibraries -ProjectDir $ProjectPath -Libraries $resolution.Libraries

    if ($resolution.TechPackages.Count -gt 0) {
        Write-Host "Updating technology packages in .apj..." -ForegroundColor Cyan
        $tpResult = Update-ApjTechnologyPackages -ApjFile $ProjectFile -RequiredTechPackages $resolution.TechPackages -ExistingTechPackages $existingTP
        $result.technologyPackagesAdded    = $tpResult.Added
        $result.technologyPackagesExisting = $tpResult.Existing
    }

    $result.success = $true
    return $result
}

#endregion

#region AddSTTask Functions

function Initialize-PackagePkgHierarchy {
    param(
        [string]$LogicalDir,
        [string]$TaskPath
    )

    $segments   = $TaskPath -split '[/\\]' | Where-Object { $_ -ne '' }
    $currentDir = $LogicalDir

    foreach ($segment in $segments) {
        $childDir      = Join-Path $currentDir $segment
        $parentPkgPath = Join-Path $currentDir "Package.pkg"

        if (-not (Test-Path $childDir)) {
            New-Item -ItemType Directory -Path $childDir -Force | Out-Null
        }

        if (-not (Test-Path $parentPkgPath)) {
            $newPkg = @(
                '<?xml version="1.0" encoding="utf-8"?>',
                '<?AutomationStudio FileVersion="4.9"?>',
                '<Package xmlns="http://br-automation.co.at/AS/Package">',
                '  <Objects>',
                '  </Objects>',
                '</Package>'
            ) -join "`n"
            [System.IO.File]::WriteAllText($parentPkgPath, $newPkg, $script:Utf8NoBom)
        }

        $parentPkgContent = Get-Content $parentPkgPath -Raw
        $escapedSegment   = [regex]::Escape($segment)
        if ($parentPkgContent -notmatch "<Object\s[^>]*>$escapedSegment</Object>") {
            $entry       = "    <Object Type=`"Package`">$segment</Object>"
            $insertPoint = $parentPkgContent.IndexOf('</Objects>')
            if ($insertPoint -ne -1) {
                $parentPkgContent = $parentPkgContent.Substring(0, $insertPoint) + $entry + "`n  " + $parentPkgContent.Substring($insertPoint)
                [System.IO.File]::WriteAllText($parentPkgPath, $parentPkgContent, $script:Utf8NoBom)
            }
        }

        $childPkgPath = Join-Path $childDir "Package.pkg"
        if (-not (Test-Path $childPkgPath)) {
            $childPkg = @(
                '<?xml version="1.0" encoding="utf-8"?>',
                '<?AutomationStudio FileVersion="4.9"?>',
                '<Package xmlns="http://br-automation.co.at/AS/Package">',
                '  <Objects>',
                '  </Objects>',
                '</Package>'
            ) -join "`n"
            [System.IO.File]::WriteAllText($childPkgPath, $childPkg, $script:Utf8NoBom)
        }

        $currentDir = $childDir
    }
}

function Limit-TaskName {
    param(
        [string]$FullName,
        [System.Collections.Generic.HashSet[string]]$UsedNames
    )

    $maxLen = 10
    
    # Defensive guard: if the caller passed $null (e.g. no existing tasks),
    # use an empty set so .Contains() can never be called on a null value.
    if (-not $UsedNames) {
        $UsedNames = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
    }

    if ($FullName.Length -le $maxLen -and -not $UsedNames.Contains($FullName)) {
        $UsedNames.Add($FullName) | Out-Null
        return $FullName
    }

    $truncated = $FullName.Substring(0, [Math]::Min($FullName.Length, $maxLen))
    if (-not $UsedNames.Contains($truncated)) {
        $UsedNames.Add($truncated) | Out-Null
        return $truncated
    }

    $counter = 1
    while ($true) {
        $suffix    = $counter.ToString()
        $prefixLen = $maxLen - $suffix.Length
        $prefix    = $FullName.Substring(0, [Math]::Min($FullName.Length, $prefixLen))
        $candidate = $prefix + $suffix
        if (-not $UsedNames.Contains($candidate)) {
            $UsedNames.Add($candidate) | Out-Null
            return $candidate
        }
        $counter++
    }
}

function Get-ExistingTaskNames {
    param([string]$SwFilePath)

    $names   = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
    $content = Get-Content $SwFilePath -Raw

    $taskPattern = '<Task\s+[^>]*Name="([^"]+)"'
    [regex]::Matches($content, $taskPattern) | ForEach-Object {
        $names.Add($_.Groups[1].Value) | Out-Null
    }

    # Wrap in array (comma operator) so PowerShell does not enumerate/unroll the
    # HashSet on return. An empty set would otherwise be emitted as nothing,
    # leaving the caller with $null and causing a null-method infinite loop.
    return ,$names
}

function Register-TaskInSwFile {
    param(
        [string]$SwFilePath,
        [string]$ShortName,
        [string]$Source,
        [int]$TaskClassNum,
        [string]$Language = "IEC"
    )

    $content       = Get-Content $SwFilePath -Raw
    $taskClassName = "Cyclic#$TaskClassNum"
    $taskEntry     = "    <Task Name=`"$ShortName`" Source=`"$Source`" Memory=`"UserROM`" Language=`"$Language`" Debugging=`"true`"></Task>"

    $tcPattern = "<TaskClass\s+Name=`"$taskClassName`"\s*>"

    if ($content -match $tcPattern) {
        if ($content -match "(<TaskClass\s+Name=`"$taskClassName`"\s*>)\s*(</TaskClass>)") {
            $fullMatch   = $matches[0]
            $replacement = "$($matches[1])`n$taskEntry`n  $($matches[2])"
            $content     = $content.Replace($fullMatch, $replacement)
        }
        else {
            $tcOpenMatch = [regex]::Match($content, "<TaskClass\s+Name=`"$taskClassName`"\s*>")
            if ($tcOpenMatch.Success) {
                $searchStart = $tcOpenMatch.Index + $tcOpenMatch.Length
                $closeIdx    = $content.IndexOf('</TaskClass>', $searchStart)
                if ($closeIdx -ne -1) {
                    $content = $content.Substring(0, $closeIdx) + "$taskEntry`n  " + $content.Substring($closeIdx)
                }
            }
        }
    }
    elseif ($content -match "<TaskClass\s+Name=`"$taskClassName`"\s*/>") {
        $selfCloseMatch = [regex]::Match($content, "<TaskClass\s+Name=`"$taskClassName`"\s*/>")
        if ($selfCloseMatch.Success) {
            $replacement = "<TaskClass Name=`"$taskClassName`">`n$taskEntry`n  </TaskClass>"
            $content     = $content.Substring(0, $selfCloseMatch.Index) + $replacement + $content.Substring($selfCloseMatch.Index + $selfCloseMatch.Length)
        }
    }
    else {
        $newTc       = "  <TaskClass Name=`"$taskClassName`">`n$taskEntry`n  </TaskClass>"
        $insertPoint = $content.IndexOf('</SwConfiguration>')
        if ($insertPoint -ne -1) {
            $content = $content.Substring(0, $insertPoint) + "$newTc`n" + $content.Substring($insertPoint)
        }
        elseif ($content -match '<SwConfiguration[^>]*/\s*>') {
            # Self-closing <SwConfiguration .../> — expand to open/close and insert TaskClass
            $content = $content -replace '<SwConfiguration([^>]*)/\s*>', "<SwConfiguration`$1>`n$newTc`n</SwConfiguration>"
        }
    }

    [System.IO.File]::WriteAllText($SwFilePath, $content, $script:Utf8NoBom)
}

function Invoke-AddAnsiCTask {
    param(
        [string]$ProjectPath,
        [string]$TaskPath,
        [string]$TaskName,
        [int]$TaskClass,
        [string]$SwFileName
    )

    $result = @{
        success       = $false
        taskPath      = $TaskPath
        taskName      = $TaskName
        taskClass     = $TaskClass
        swFile        = $null
        shortenedName = $null
        error         = $null
    }

    if (-not $TaskName)  { $result.error = "TaskName parameter is required for AddAnsiCTask action."; return $result }
    if (-not $TaskPath)  { $result.error = "TaskPath parameter is required for AddAnsiCTask action."; return $result }
    if ($TaskName.Length -gt 10) { $result.error = "TaskName must be at most 10 characters. Got: '$TaskName' ($($TaskName.Length) chars)."; return $result }
    if ($TaskClass -lt 1 -or $TaskClass -gt 8) { $result.error = "TaskClass must be between 1 and 8. Got: $TaskClass."; return $result }

    $activeConfig = Get-ActiveConfiguration -ProjectDir $ProjectPath
    if (-not $activeConfig) { $result.error = "Could not determine active configuration from LastUser.set."; return $result }
    Write-Host "Active configuration: $activeConfig" -ForegroundColor Cyan

    $swFilePath = Find-SwFile -ProjectDir $ProjectPath -ActiveConfig $activeConfig -SwFileName $SwFileName
    if (-not $swFilePath) { $result.error = "Could not find '$SwFileName' in Physical/$activeConfig/."; return $result }
    $result.swFile = $swFilePath
    Write-Host "SW file: $swFilePath" -ForegroundColor Cyan

    $logicalDir = Join-Path $ProjectPath "Logical"
    $taskDir    = Join-Path (Join-Path $logicalDir $TaskPath) $TaskName
    if (Test-Path $taskDir) { $result.error = "Task folder already exists: $taskDir"; return $result }

    Write-Host "Ensuring folder hierarchy for: $TaskPath" -ForegroundColor Cyan
    Initialize-PackagePkgHierarchy -LogicalDir $logicalDir -TaskPath $TaskPath

    New-Item -ItemType Directory -Path $taskDir -Force | Out-Null
    Write-Host "Created task folder: $taskDir" -ForegroundColor Green

    $ansicPrg = @'
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Program SubType="ANSIC" xmlns="http://br-automation.co.at/AS/Program">
  <Files>
    <File Description="Local data types" Private="true">Types.typ</File>
    <File Description="Local variables" Private="true">Variables.var</File>
    <File Description="Local declarations, macros, and function prototypes" Private="true">Local.h</File>
    <File Description="Cyclic code">Cyclic.c</File>
    <File Description="Initialization code">Init.c</File>
    <File Description="Exit code">Exit.c</File>
  </Files>
</Program>
'@

    $typesTyp = @'

TYPE

END_TYPE
'@

    $variablesVar = @'

VAR

END_VAR
'@

    $headerGuard = "{0}_LOCAL_H" -f $TaskName.ToUpperInvariant()

    $localHeader = @"
#ifndef $headerGuard
#define $headerGuard

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#endif
"@

    $initC = @"
#include "Local.h"

void _INIT ${TaskName}INIT(void)
{
}
"@

    $cyclicC = @"
#include "Local.h"

void _CYCLIC ${TaskName}CYCLIC(void)
{
}
"@

    $exitC = @"
#include "Local.h"

void _EXIT ${TaskName}EXIT(void)
{
}
"@

    [System.IO.File]::WriteAllText((Join-Path $taskDir "ANSIC.prg"),    $ansicPrg,    $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Types.typ"),    $typesTyp,    $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Variables.var"),$variablesVar,$script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Local.h"),      $localHeader, $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Init.c"),       $initC,       $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Cyclic.c"),     $cyclicC,     $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Exit.c"),       $exitC,       $script:Utf8NoBom)
    Write-Host "  Written: ANSIC.prg, Types.typ, Variables.var, Local.h, Init.c, Cyclic.c, Exit.c" -ForegroundColor Gray

    $folderPkgPath    = Join-Path (Join-Path $logicalDir $TaskPath) "Package.pkg"
    $folderPkgContent = Get-Content $folderPkgPath -Raw
    $entry            = "    <Object Type=`"Program`" Language=`"ANSIC`">$TaskName</Object>"
    $insertPoint      = $folderPkgContent.IndexOf('</Objects>')
    if ($insertPoint -ne -1) {
        $folderPkgContent = $folderPkgContent.Substring(0, $insertPoint) + $entry + "`n  " + $folderPkgContent.Substring($insertPoint)
        [System.IO.File]::WriteAllText($folderPkgPath, $folderPkgContent, $script:Utf8NoBom)
        Write-Host "  Registered in: $folderPkgPath" -ForegroundColor Gray
    }

    $pathSegments = @($TaskPath -split '[/\\]' | Where-Object { $_ -ne '' })
    $source       = (@($pathSegments) + @($TaskName)) -join '.'
    $source      += '.prg'

    $usedNames  = Get-ExistingTaskNames -SwFilePath $swFilePath
    $shortName  = Limit-TaskName -FullName $TaskName -UsedNames $usedNames
    $result.shortenedName = $shortName
    Write-Host "  Task name in $SwFileName`: $shortName (Source: $source)" -ForegroundColor Gray

    Register-TaskInSwFile -SwFilePath $swFilePath -ShortName $shortName -Source $source -TaskClassNum $TaskClass -Language "ANSIC"
    Write-Host "  Registered in $SwFileName TaskClass Cyclic#$TaskClass" -ForegroundColor Green

    $result.success = $true
    return $result
}

function Invoke-AddSTTask {
    param(
        [string]$ProjectPath,
        [string]$TaskPath,
        [string]$TaskName,
        [int]$TaskClass,
        [string]$SwFileName
    )

    $result = @{
        success       = $false
        taskPath      = $TaskPath
        taskName      = $TaskName
        taskClass     = $TaskClass
        swFile        = $null
        shortenedName = $null
        error         = $null
    }

    if (-not $TaskName)  { $result.error = "TaskName parameter is required for AddSTTask action."; return $result }
    if (-not $TaskPath)  { $result.error = "TaskPath parameter is required for AddSTTask action.";  return $result }
    if ($TaskName.Length -gt 10) { $result.error = "TaskName must be at most 10 characters. Got: '$TaskName' ($($TaskName.Length) chars)."; return $result }
    if ($TaskClass -lt 1 -or $TaskClass -gt 8) { $result.error = "TaskClass must be between 1 and 8. Got: $TaskClass."; return $result }

    $activeConfig = Get-ActiveConfiguration -ProjectDir $ProjectPath
    if (-not $activeConfig) { $result.error = "Could not determine active configuration from LastUser.set."; return $result }
    Write-Host "Active configuration: $activeConfig" -ForegroundColor Cyan

    $swFilePath = Find-SwFile -ProjectDir $ProjectPath -ActiveConfig $activeConfig -SwFileName $SwFileName
    if (-not $swFilePath) { $result.error = "Could not find '$SwFileName' in Physical/$activeConfig/."; return $result }
    $result.swFile = $swFilePath
    Write-Host "SW file: $swFilePath" -ForegroundColor Cyan

    $logicalDir = Join-Path $ProjectPath "Logical"
    $taskDir    = Join-Path (Join-Path $logicalDir $TaskPath) $TaskName
    if (Test-Path $taskDir) { $result.error = "Task folder already exists: $taskDir"; return $result }

    Write-Host "Ensuring folder hierarchy for: $TaskPath" -ForegroundColor Cyan
    Initialize-PackagePkgHierarchy -LogicalDir $logicalDir -TaskPath $TaskPath

    New-Item -ItemType Directory -Path $taskDir -Force | Out-Null
    Write-Host "Created task folder: $taskDir" -ForegroundColor Green

    $iecPrg = @'
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Program SubType="IEC" xmlns="http://br-automation.co.at/AS/Program">
  <Files>
    <File Description="Init, cyclic, exit code">Main.st</File>
    <File Description="Local data types" Private="true">Types.typ</File>
    <File Description="Local variables" Private="true">Variables.var</File>
  </Files>
</Program>
'@

    $mainSt = @'
PROGRAM _INIT

END_PROGRAM

PROGRAM _CYCLIC

END_PROGRAM

PROGRAM _EXIT

END_PROGRAM
'@

    $typesTyp = @'

TYPE

END_TYPE
'@

    $variablesVar = @'

VAR

END_VAR
'@

    [System.IO.File]::WriteAllText((Join-Path $taskDir "IEC.prg"),       $iecPrg,       $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Main.st"),       $mainSt,       $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Types.typ"),     $typesTyp,     $script:Utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $taskDir "Variables.var"), $variablesVar, $script:Utf8NoBom)
    Write-Host "  Written: IEC.prg, Main.st, Types.typ, Variables.var" -ForegroundColor Gray

    $folderPkgPath    = Join-Path (Join-Path $logicalDir $TaskPath) "Package.pkg"
    $folderPkgContent = Get-Content $folderPkgPath -Raw
    $entry            = "    <Object Type=`"Program`">$TaskName</Object>"
    $insertPoint      = $folderPkgContent.IndexOf('</Objects>')
    if ($insertPoint -ne -1) {
        $folderPkgContent = $folderPkgContent.Substring(0, $insertPoint) + $entry + "`n  " + $folderPkgContent.Substring($insertPoint)
        [System.IO.File]::WriteAllText($folderPkgPath, $folderPkgContent, $script:Utf8NoBom)
        Write-Host "  Registered in: $folderPkgPath" -ForegroundColor Gray
    }

    $pathSegments = @($TaskPath -split '[/\\]' | Where-Object { $_ -ne '' })
    $source       = (@($pathSegments) + @($TaskName)) -join '.'
    $source      += '.prg'

    $usedNames  = Get-ExistingTaskNames -SwFilePath $swFilePath
    $shortName  = Limit-TaskName -FullName $TaskName -UsedNames $usedNames
    $result.shortenedName = $shortName
    Write-Host "  Task name in $SwFileName`: $shortName (Source: $source)" -ForegroundColor Gray

    Register-TaskInSwFile -SwFilePath $swFilePath -ShortName $shortName -Source $source -TaskClassNum $TaskClass -Language "IEC"
    Write-Host "  Registered in $SwFileName TaskClass Cyclic#$TaskClass" -ForegroundColor Green

    $result.success = $true
    return $result
}

#endregion

#region Main Logic

$ProjectPath = Resolve-Path $ProjectPath
$projectFile = Get-ProjectFile -ProjectDir $ProjectPath
$projectName = [System.IO.Path]::GetFileNameWithoutExtension($projectFile)

Write-Banner "B&R AS Logical API"
Write-Host "Project: $projectName"
Write-Host "Path:    $ProjectPath"
Write-Host "Action:  $Action"

if ($Action -eq "AddLibrary") {
    Write-Host "Library:    $LibraryName"
    if ($MinVersion) { Write-Host "MinVersion: $MinVersion" }
}
elseif ($Action -eq "AddSTTask" -or $Action -eq "AddAnsiCTask") {
    Write-Host "TaskPath:  $TaskPath"
    Write-Host "TaskName:  $TaskName"
    Write-Host "TaskClass: $TaskClass"
    Write-Host "SwFile:    $SwFileName"
}

$exitCode = 0

switch ($Action) {
    "AddLibrary" {
        if (-not $LibraryName) { Write-Failure "LibraryName parameter is required."; exit 1 }

        $selectedAS     = Resolve-ASInstallation -ProjectFile $projectFile
        $asContentPath  = Get-ASContentPath -ASPath $selectedAS.Path

        Write-Host ("=" * 60) -ForegroundColor Yellow
        $addLibResult = Invoke-AddLibrary `
            -ProjectPath   $ProjectPath `
            -ProjectFile   $projectFile `
            -LibraryName   $LibraryName `
            -MinVersion    $MinVersion `
            -ASContentPath $asContentPath

        Write-Output ($addLibResult | ConvertTo-Json -Depth 5 -Compress)

        if ($addLibResult.success) {
            Write-Success "AddLibrary completed: $($addLibResult.librariesAdded.Count) added, $($addLibResult.librariesSkipped.Count) skipped"
        }
        else {
            Write-Failure "AddLibrary failed: $($addLibResult.error)"
            $exitCode = 1
        }
    }
    "AddSTTask" {
        Write-Host ("=" * 60) -ForegroundColor Yellow
        $addTaskResult = Invoke-AddSTTask `
            -ProjectPath $ProjectPath `
            -TaskPath    $TaskPath `
            -TaskName    $TaskName `
            -TaskClass   $TaskClass `
            -SwFileName  $SwFileName

        Write-Output ($addTaskResult | ConvertTo-Json -Depth 5 -Compress)

        if ($addTaskResult.success) {
            Write-Success "AddSTTask completed: $($addTaskResult.taskName) in $($addTaskResult.taskPath)"
        }
        else {
            Write-Failure "AddSTTask failed: $($addTaskResult.error)"
            $exitCode = 1
        }
    }
    "AddAnsiCTask" {
        Write-Host ("=" * 60) -ForegroundColor Yellow
        $addTaskResult = Invoke-AddAnsiCTask `
            -ProjectPath $ProjectPath `
            -TaskPath    $TaskPath `
            -TaskName    $TaskName `
            -TaskClass   $TaskClass `
            -SwFileName  $SwFileName

        Write-Output ($addTaskResult | ConvertTo-Json -Depth 5 -Compress)

        if ($addTaskResult.success) {
            Write-Success "AddAnsiCTask completed: $($addTaskResult.taskName) in $($addTaskResult.taskPath)"
        }
        else {
            Write-Failure "AddAnsiCTask failed: $($addTaskResult.error)"
            $exitCode = 1
        }
    }
}

exit $exitCode

#endregion
