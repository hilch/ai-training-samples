<#
.SYNOPSIS
    Shared utility functions for B&R Automation Studio project management scripts.

.DESCRIPTION
    Dot-source this file from other AS skill scripts to access common helpers:
    - UTF-8 NoBOM encoding instance
    - ASInstallation class
    - AS installation registry detection
    - Project file / version parsing
    - AS content path helpers
    - Active configuration resolution
    - .sw file lookup
    - Version comparison
    - Console output helpers (Write-Banner, Write-Step, Write-Success, Write-Failure)

    Usage:
        . "$PSScriptRoot/../../as-shared/scripts/as-shared.ps1"
#>

# UTF-8 encoding WITHOUT BOM - required by B&R Automation Studio for .st/.typ/.var files.
# [System.Text.Encoding]::UTF8 includes a BOM and causes compiler errors 1852/1120.
$script:Utf8NoBom = New-Object System.Text.UTF8Encoding $false

#region Classes and Types

class ASInstallation {
    [string]$Version
    [string]$Path
    [string]$SharedPath

    ASInstallation([string]$version, [string]$path, [string]$sharedPath) {
        $this.Version = $version
        $this.Path = $path
        $this.SharedPath = $sharedPath
    }
}

#endregion

#region Registry Detection Functions

function Get-InstalledASVersions {
    <#
    .SYNOPSIS
        Discovers all installed Automation Studio versions from Windows registry.
    .DESCRIPTION
        Scans HKLM:\SOFTWARE\WOW6432Node for BR_AS_* keys and extracts
        version, install path, and shared path for each installation.
        Returns results sorted by version descending (newest first).
    #>
    $installations = @()
    $regPath = "HKLM:\SOFTWARE\WOW6432Node"

    try {
        $keys = Get-ChildItem -Path $regPath -ErrorAction SilentlyContinue |
        Where-Object { $_.PSChildName -like "BR_AS_*" }

        foreach ($key in $keys) {
            try {
                $asPath = (Get-ItemProperty -Path $key.PSPath -ErrorAction SilentlyContinue).BuRSharedFilesPath
                $sharedPath = (Get-ItemProperty -Path $key.PSPath -ErrorAction SilentlyContinue).BuRAutStudioPath

                $csKey = Join-Path $key.PSPath "ControlStudio"
                if (Test-Path $csKey) {
                    $version = (Get-ItemProperty -Path $csKey -ErrorAction SilentlyContinue).ProgrVersion
                    if ($asPath -and $version) {
                        $installations += [ASInstallation]::new($version, $asPath, $sharedPath)
                    }
                }
            }
            catch { continue }
        }
    }
    catch {
        Write-Warning "Could not read AS installations from registry: $_"
    }

    # Sort by version descending (newest first)
    return $installations | Sort-Object {
        $cleanVersion = $_.Version -replace '\s*SP.*$', ''
        $cleanVersion = $cleanVersion -replace '\.(\d)\.(\d)\.', '.0$1.0$2.'
        $cleanVersion = $cleanVersion -replace '\.(\d)$', '.0$1'
        try { [version]$cleanVersion } catch { [version]"0.0.0.0" }
    } -Descending
}

#endregion

#region Project Parsing Functions

function Get-ProjectFile {
    <#
    .SYNOPSIS
        Finds the .apj file in a project directory.
    #>
    param([string]$ProjectDir)

    $apjFile = Get-ChildItem -Path $ProjectDir -Filter "*.apj" -File | Select-Object -First 1
    if (-not $apjFile) {
        throw "No .apj file found in: $ProjectDir"
    }
    return $apjFile.FullName
}

function Get-ProjectVersion {
    <#
    .SYNOPSIS
        Extracts the AS version from a project's .apj file.
    #>
    param([string]$ApjFile)

    $content = Get-Content $ApjFile -Raw
    if ($content -match 'AutomationStudio Version="([\d\.]+)"') {
        return $matches[1]
    }
    return $null
}

function Get-ProjectWorkingVersion {
    <#
    .SYNOPSIS
        Extracts the working version from a project's .apj file.
    #>
    param([string]$ApjFile)

    $content = Get-Content $ApjFile -Raw
    if ($content -match 'WorkingVersion="([\d\.]+)"') {
        return $matches[1]
    }
    return $null
}

function Find-CompatibleASInstallation {
    <#
    .SYNOPSIS
        Finds an AS installation compatible with the project version.
    .DESCRIPTION
        Matches the project's major.minor version against installed versions.
        Falls back to the newest installation if no exact match is found.
    #>
    param(
        [string]$ProjectVersion,
        [ASInstallation[]]$Installations
    )

    if (-not $ProjectVersion -or $Installations.Count -eq 0) {
        return $Installations | Select-Object -First 1
    }

    $projParts = $ProjectVersion.Split('.')
    $projMajorMinor = "$($projParts[0]).$($projParts[1])"

    foreach ($install in $Installations) {
        $instParts = $install.Version.Split('.')
        $instMajorMinor = "$($instParts[0]).$($instParts[1])"
        if ($projMajorMinor -eq $instMajorMinor) {
            return $install
        }
    }

    Write-Warning "No exact AS version match for $ProjectVersion. Using newest installed version."
    return $Installations | Select-Object -First 1
}

#endregion

#region Output Functions

function Write-Banner {
    param([string]$Title)
    Write-Host ""
    Write-Host ("=" * 60) -ForegroundColor Yellow
    Write-Host "  $Title" -ForegroundColor Yellow
    Write-Host ("=" * 60) -ForegroundColor Yellow
}

function Write-Step {
    param([string]$Message)
    Write-Host "`n>> $Message" -ForegroundColor Cyan
}

function Write-Success {
    param([string]$Message)
    Write-Host "[SUCCESS] $Message" -ForegroundColor Green
}

function Write-Failure {
    param([string]$Message)
    Write-Host "[FAILED] $Message" -ForegroundColor Red
}

#endregion

#region Version Comparison

function Compare-Versions {
    <#
    .SYNOPSIS
        Compares two version strings numerically.
    .DESCRIPTION
        Strips optional 'V'/'v' prefix, splits by '.', compares each segment.
        Returns -1 if A < B, 0 if equal, 1 if A > B.
    #>
    param([string]$VersionA, [string]$VersionB)

    $a = ($VersionA -replace '^[Vv]', '').Split('.') | ForEach-Object {
        $n = 0; if ([int]::TryParse($_, [ref]$n)) { $n } else { 0 }
    }
    $b = ($VersionB -replace '^[Vv]', '').Split('.') | ForEach-Object {
        $n = 0; if ([int]::TryParse($_, [ref]$n)) { $n } else { 0 }
    }

    $len = [Math]::Max($a.Count, $b.Count)
    for ($i = 0; $i -lt $len; $i++) {
        $va = if ($i -lt $a.Count) { $a[$i] } else { 0 }
        $vb = if ($i -lt $b.Count) { $b[$i] } else { 0 }
        if ($va -lt $vb) { return -1 }
        if ($va -gt $vb) { return 1 }
    }
    return 0
}

#endregion

#region AS Content Path Helper

function Get-ASContentPath {
    <#
    .SYNOPSIS
        Returns the AS content path (containing Library/, TechnologyPackages/, Hardware/, System/).
    #>
    param([string]$ASPath)
    return Join-Path $ASPath "AS"
}

#endregion

#region Project Configuration Helpers

function Get-ActiveConfiguration {
    <#
    .SYNOPSIS
        Reads the active configuration name from LastUser.set.
    #>
    param([string]$ProjectDir)

    $lastUserSet = Join-Path $ProjectDir "LastUser.set"
    if (-not (Test-Path $lastUserSet)) { return $null }

    $content = Get-Content $lastUserSet -Raw
    if ($content -match 'ActiveConfigurationName="([^"]+)"') {
        return $matches[1]
    }
    return $null
}

function Find-SwFile {
    <#
    .SYNOPSIS
        Finds the .sw file matching SwFileName in the active configuration's CPU folder.
    #>
    param(
        [string]$ProjectDir,
        [string]$ActiveConfig,
        [string]$SwFileName
    )

    $configDir = Join-Path (Join-Path $ProjectDir "Physical") $ActiveConfig
    if (-not (Test-Path $configDir)) { return $null }

    foreach ($cpuDir in Get-ChildItem -Path $configDir -Directory -ErrorAction SilentlyContinue) {
        $swFile = Join-Path $cpuDir.FullName $SwFileName
        if (Test-Path $swFile) { return $swFile }
    }
    return $null
}

function Resolve-ASInstallation {
    <#
    .SYNOPSIS
        Resolves the AS installation path for a given project file.
    .DESCRIPTION
        Reads the project version, discovers installed AS versions from the registry,
        and selects the best compatible installation. Falls back to the default path
        if no installation is found.
        Returns the ASInstallation object (or a synthetic one with the fallback path).
    #>
    param([string]$ProjectFile)

    $projectVersion = Get-ProjectVersion -ApjFile $ProjectFile
    $workingVersion = Get-ProjectWorkingVersion -ApjFile $ProjectFile
    Write-Host "Project Version: $projectVersion (Working: $workingVersion)"

    $asInstallations = Get-InstalledASVersions
    if ($asInstallations.Count -eq 0) {
        Write-Warning "No Automation Studio installations found in registry. Falling back to default path."
        return [ASInstallation]::new("unknown", "C:\Program Files (x86)\BRAutomation\AS6", "")
    }

    Write-Host "`nInstalled AS versions:"
    foreach ($install in $asInstallations) {
        Write-Host "  - $($install.Version) at $($install.Path)"
    }

    $selected = Find-CompatibleASInstallation -ProjectVersion $projectVersion -Installations $asInstallations
    Write-Host "Selected AS: $($selected.Version)" -ForegroundColor Green
    return $selected
}

#endregion

#region JSON Output Helper

function ConvertTo-AsJson {
    <#
    .SYNOPSIS
        Serializes an object to compact JSON without HTML-escaping <, >, ', &.
    .DESCRIPTION
        PowerShell 5.1's ConvertTo-Json uses System.Web.Script.Serialization.JavaScriptSerializer
        which unconditionally HTML-encodes <, >, ', & as \u003c, \u003e, \u0027, \u0026. There is
        no built-in parameter or switch to disable this in PS 5.1 — it is hard-coded in the
        underlying .NET 4.x serializer. The substitution below is the standard workaround.
        PowerShell 7+ does not have this issue (it uses System.Text.Json with relaxed escaping),
        so this function can be replaced with a plain ConvertTo-Json call if the project migrates.
    #>
    param(
        [Parameter(Mandatory = $true)]$InputObject,
        [int]$Depth = 64
    )
    $json = ConvertTo-Json -InputObject $InputObject -Depth $Depth -Compress
    if ($null -eq $json) { return $json }
    return $json.Replace('\u003c', '<').Replace('\u003e', '>').Replace('\u0027', "'").Replace('\u0026', '&')
}

#endregion
