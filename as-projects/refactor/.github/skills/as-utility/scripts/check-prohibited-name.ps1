<#
.SYNOPSIS
    Checks if a variable or file name is prohibited in B&R Automation Studio.

.DESCRIPTION
    Discovers the B&R Automation Studio installation, reads the 'ReservedNames.txt' file,
    and checks if the input name is prohibited (supporting wildcards like `Ar*`).
    If prohibited, returns 'NAME PROHIBITED' in red text.
    If allowed, returns 'NAME ALLOWED' in green text.

.PARAMETER Name
    The variable, file name, or string to validate.

.EXAMPLE
    .\check-prohibited-name.ps1 -Name "ArControl"

.EXAMPLE
    .\check-prohibited-name.ps1 -Name "MyCustomVar"
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Name
)

# Dot-source shared utilities (provides Get-InstalledASVersions, ASInstallation class)
. "$PSScriptRoot/../../as-shared/scripts/as-shared.ps1"

if ([string]::IsNullOrWhiteSpace($Name)) {
    Write-Warning "Name parameter cannot be empty."
    exit 1
}

$nameToCheck = $Name.Trim()

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

$isProhibited = $false
$matchingPattern = $null

foreach ($line in $reservedNames) {
    $trimmedLine = $line.Trim()
    if ([string]::IsNullOrWhiteSpace($trimmedLine)) {
        continue
    }

    # Match case-insensitively using PowerShell -like operator
    # E.g. "ArControl" -like "Ar*" becomes True
    if ($nameToCheck -like $trimmedLine) {
        $isProhibited = $true
        $matchingPattern = $trimmedLine
        break
    }
}

if ($isProhibited) {
    Write-Host "NAME PROHIBITED" -ForegroundColor Red
    Write-Host "The name '$nameToCheck' is prohibited because it matches the reserved pattern '$matchingPattern'."
} else {
    Write-Host "NAME ALLOWED" -ForegroundColor Green
    Write-Host "The name '$nameToCheck' is allowed and doesn't match any reserved pattern."
}

exit 0
