<#
.SYNOPSIS
    Returns the list of prohibited/reserved names in B&R Automation Studio.

.DESCRIPTION
    Discovers the B&R Automation Studio installation, reads the 'ReservedNames.txt' file,
    and returns its contents. Supports resolving the AS path using the shared utilities registry detection.

.EXAMPLE
    .\get-prohibited-names.ps1
#>

# Dot-source shared utilities (provides Get-InstalledASVersions, ASInstallation class)
. "$PSScriptRoot/../../as-shared/scripts/as-shared.ps1"

$installations = Get-InstalledASVersions
$asPath = $null

foreach ($install in $installations) {
    if ($install.Path) {
        $testPath = Join-Path $install.Path "AS\ReservedNames.txt"
        if (Test-Path $testPath) {
            $asPath = $install.Path
            break
        }
    }
}

if (-not $asPath) {
    $asPath = "C:\Program Files (x86)\BRAutomation\AS6"
}

$reservedNamesPath = Join-Path $asPath "AS\ReservedNames.txt"

if (-not (Test-Path $reservedNamesPath)) {
    Write-Warning "ReservedNames.txt not found at planned path: $reservedNamesPath"
    exit 1
}

$reservedNames = Get-Content $reservedNamesPath

Write-Host "Prohibited/Reserved Names in B&R Automation Studio:"
foreach ($name in $reservedNames) {
    $trimmed = $name.Trim()
    if (-not [string]::IsNullOrWhiteSpace($trimmed)) {
        Write-Host "  $trimmed"
    }
}
exit 0
