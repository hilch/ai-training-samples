<#
.SYNOPSIS
    Lists all installed B&R Automation Studio versions and their installation paths.

.DESCRIPTION
    Uses shared utility functions to read the Windows registry and discover all installed
    Automation Studio versions. Outputs version and path for each installation found,
    sorted newest-first.

.EXAMPLE
    .\get-as-installations.ps1
#>

# Dot-source shared utilities (provides Get-InstalledASVersions, ASInstallation class)
. "$PSScriptRoot/../../as-shared/scripts/as-shared.ps1"

$installations = Get-InstalledASVersions

if ($installations.Count -eq 0) {
    Write-Warning "No Automation Studio installations found in registry."
    exit 1
}

Write-Host "Installed Automation Studio versions (newest first):"
foreach ($install in $installations) {
    Write-Host ""
    Write-Host "  Version : $($install.Version)"
    Write-Host "  Path    : $($install.Path)"
    if ($install.SharedPath) {
        Write-Host "  Shared  : $($install.SharedPath)"
    }
}
Write-Host ""
