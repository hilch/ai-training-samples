---
name: as-compile
description: "Use when building, compiling, rebuilding, cleaning, transferring, deploying, or downloading a B&R Automation Studio project to a PLC or ARsim simulator. Triggers: build project, compile AS, rebuild, clean artifacts, transfer to PLC, download to controller, deploy firmware, ARsim, RUC package, installation package PIP, build errors, fix compile errors."
argument-hint: "Build | Rebuild | Clean | Transfer [IP] | BuildAndTransfer [IP]"
---

# AS Compile

Build and transfer B&R Automation Studio projects using PowerShell, with auto-detection of AS/PVI installations, configuration discovery, and auto-generated PIL files.

## When to Use

- Build or rebuild the project (incremental or full)
- Read and fix build errors in source code
- Transfer a compiled project to a PLC or ARsim simulator
- Generate a RUC package or Project Installation Package (PIP)
- Clean build artifacts (`Temp/`, `Binaries/`, `Diagnosis/`)

**Prefer the VS Code tasks** (`AS: Build`, `AS: Rebuild`, `AS: Transfer`, etc.) when running interactively. Use the script directly when orchestrating from code.

## Script

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-compile/scripts/invoke-as-build.ps1 `
  -ProjectPath <project-path> -Action <action> [options]
```

Full script: [invoke-as-build.ps1](./scripts/invoke-as-build.ps1)

## Parameters

| Parameter       | Description                                                     | Default      |
|-----------------|-----------------------------------------------------------------|--------------|
| `-ProjectPath`  | Path to AS project directory (required)                         | —            |
| `-Action`       | `Build`, `Rebuild`, `Transfer`, `BuildAndTransfer`, `Clean`     | `Build`      |
| `-Configuration`| Configuration name, or `all` for every configuration           | Auto-detect  |
| `-TargetIP`     | IP address for transfer (PLC or ARsim)                          | `127.0.0.1`  |
| `-ShowWarnings` | Display build warnings in output                                | Off          |
| `-NoClean`      | Skip pre-clean step before build                                | Off          |
| `-InstallMode`  | `Consistent` or `InstallDuringTaskOperation`                    | `Consistent` |
| `-PILFile`      | Custom PIL file path                                            | Auto-generate|
| `-BuildPIP`     | Generate Project Installation Package                           | Off          |

## Procedure

### Build the project

1. Prefer the VS Code task **AS: Build** for a standard incremental build.
2. Or run directly:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .github/skills/as-compile/scripts/invoke-as-build.ps1 `
     -ProjectPath .
   ```
3. Check output for errors. Errors are reported with file path and line number — fix them in source.

### Rebuild (clean + full build)

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-compile/scripts/invoke-as-build.ps1 `
  -ProjectPath . -Action Rebuild
```

### Build all configurations

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-compile/scripts/invoke-as-build.ps1 `
  -ProjectPath . -Configuration all
```

### Transfer to ARsim (localhost)

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-compile/scripts/invoke-as-build.ps1 `
  -ProjectPath . -Action Transfer
```

### Build and transfer to a PLC

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-compile/scripts/invoke-as-build.ps1 `
  -ProjectPath . -Action BuildAndTransfer -TargetIP 192.168.1.100
```

### Clean build artifacts

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-compile/scripts/invoke-as-build.ps1 `
  -ProjectPath . -Action Clean
```

### Show warnings

Add `-ShowWarnings` to any build command to include warnings in output.

## Reading Build Errors

Errors are formatted as:

```
<file>(<line>): error <code>: <message>
```

Fix errors in the source file at the reported location. Never edit files in `Binaries/`, `Temp/`, or `Diagnosis/`.

## Build Result Output

Each configuration reports a status line, followed by a total. Counts are parsed from the compiler's `Build: X error(s), [Y security risk(s),] Z warning(s)` line. The **security risk(s)** count is only present (and only printed) when the AS version is AS6.7 or higher:

The final banner and exit code are interpreted from the build result:

| Exit code | Status      | Banner                              |
|-----------|-------------|-------------------------------------|
| `0`       | `OK`        | Operation completed successfully!   |
| `1`       | `WARNINGS`  | Operation completed with warnings!  |
| `3`       | `FAILED`    | Operation failed!                   |

Security risks and warnings do **not** fail the build (exit code stays `1`); only errors produce a failed result.

## Notes

- Auto-detects installed AS and PVI versions from the Windows registry.
- Configuration is auto-detected from the project if not specified.
- Windows only. Requires B&R Automation Studio 4.x/6.x, PVI, and PowerShell 5.1+.
