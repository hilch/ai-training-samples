---
name: as-utility
description: "Use when needing to discover the AS installation path for a project, retrieving prohibited/reserved names, and validating variable or file names. Triggers: list AS versions, get AS path, installed Automation Studio, AS installation path, get-as-installations, AS registry, which AS version, prohibited names, reserved names, check file name, check variable name, allowed names, name prohibited, name allowed."
argument-hint: "GetInstalledVersions | GetProhibitedNames | CheckProhibitedName <name>"
---

# AS Utility

Small utility helpers for working with the B&R Automation Studio tech stack and project environment.

## When to Use

- **List installed AS versions** to discover which Automation Studio versions are installed and their paths
- Identify the installation path for mappView widget source code, hardware modules, or library browsing
- Verify which AS version is active for a given project
- **Get prohibited/reserved names** when naming PLC variables, files, or components
- **Validate a name** to check if it is allowed or prohibited by Automation Studio naming rules

## Scripts

### Get Automation Studio Installations

Lists all installed Automation Studio versions from the Windows registry, sorted newest-first.

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-utility/scripts/get-as-installations.ps1
```

Full script: [get-as-installations.ps1](./scripts/get-as-installations.ps1)

**No parameters required.** Output example:

```
Installed Automation Studio versions (newest first):

  Version : 6.5.2.12 SP
  Path    : C:\Program Files (x86)\BRAutomation\AS6
  Shared  : C:\ProgramData\BRAutomation\AS6

  Version : 4.12.6.106
  Path    : C:\Program Files\BRAutomation4\AS412
```

### Get Prohibited Names in Automation Studio

Reads the prohibited/reserved names from `ReservedNames.txt` located in the B&R Automation Studio installation directory and prints them.

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-utility/scripts/get-prohibited-names.ps1
```

Full script: [get-prohibited-names.ps1](./scripts/get-prohibited-names.ps1)

**No parameters required.**

### Check if a Name is Prohibited in Automation Studio

Checks if a given name (for a variable, file, or component) matches any prohibited or reserved name in B&R Automation Studio (supporting wildcards like `Ar*`).
Outputs `NAME PROHIBITED` in red or `NAME ALLOWED` in green.

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-utility/scripts/check-prohibited-name.ps1 -Name "<name>"
```

Full script: [check-prohibited-name.ps1](./scripts/check-prohibited-name.ps1)

**Parameters:**
- `-Name` (Mandatory): The string/name to check.

## Notes

- Windows only. Requires PowerShell 5.1+
