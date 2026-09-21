---
name: as-logical-api
description: "Use when adding a library, registering a technology package, resolving library dependencies, or creating a new Structured Text (ST) task in a B&R Automation Studio project. Triggers: add library, add ST task, create program, register MTBasics, McAxis, mapp library, resolve dependency, new cyclic task, Package.pkg, Cpu.sw registration."
argument-hint: "AddLibrary <name> [MinVersion] | AddSTTask <path> <name> [TaskClass]"
---

# AS Logical API

Automation Studio **Logical view** helper — adds libraries and ST program tasks so that `Package.pkg`, `Cpu.sw`, and `.apj` technology package registrations are kept correct without hand-editing XML.

## When to Use

- **Add a library** (e.g. `MTBasics`, `McAxis`, `MpAlarmX`) to the project
- **Add a new ST program task** and register it in `Package.pkg` and `Cpu.sw`

## Script

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-logical-api/scripts/use-as-logical-api.ps1 `
  -ProjectPath <project-path> -Action <action> [options]
```

Full script: [use-as-logical-api.ps1](./scripts/use-as-logical-api.ps1)

| Parameter      | Description                                                     | Default      |
| -------------- | --------------------------------------------------------------- | ------------ |
| `-ProjectPath` | Path to AS project directory (required)                         | —            |
| `-Action`      | `AddLibrary` or `AddSTTask`                                     | `AddLibrary` |
| `-LibraryName` | Library to add (required for `AddLibrary`)                      | —            |
| `-MinVersion`  | Minimum version constraint for the library                      | none         |
| `-TaskPath`    | Folder under `Logical/` for new task (required for `AddSTTask`) | —            |
| `-TaskName`    | ST task name, max 10 chars (required for `AddSTTask`)           | —            |
| `-TaskClass`   | Cyclic task class 1–8                                           | `4`          |
| `-SwFileName`  | `.sw` file to register task in                                  | `Cpu.sw`     |

## Procedure

### Add a library

1. Identify the library name (e.g. `MTBasics`, `McAxis`, `MpAlarmX`, `AsIecCon`).
2. Run via VS Code task **AS Logical API: Add Library** — or directly:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .github/skills/as-logical-api/scripts/use-as-logical-api.ps1 `
     -ProjectPath . -Action AddLibrary -LibraryName MTBasics
   ```
3. Optionally pin a minimum version:
   ```powershell
   ... -LibraryName McAxis -MinVersion 6.0.0
   ```
4. Verify the library appears in `Logical/Libraries/` and is referenced in `Cpu.sw`.

### Add a new ST task

1. Decide the logical path (e.g. `Drive`) and a name ≤ 10 chars (e.g. `ConvCtrl`).
2. Run via VS Code task **AS Logical API: Add ST Task** — or directly:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .github/skills/as-logical-api/scripts/use-as-logical-api.ps1 `
     -ProjectPath . -Action AddSTTask -TaskPath Drive -TaskName ConvCtrl
   ```
3. Verify `Logical/Drive/ConvCtrl/` exists with `IEC.prg`, `Main.st`, `Types.typ`, `Variables.var`.
4. Verify `Package.pkg` in the parent folder and `Cpu.sw` include the new task.


### Add a new ANSI-C task

1. Decide the logical path (e.g. `Service`) and a name ≤ 10 chars (e.g. `TelnetCli`).
2. Run via VS Code task **AS Logical API: Add AnsiC Task** — or directly:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .github/skills/as-logical-api/scripts/use-as-logical-api.ps1 `
     -ProjectPath . -Action AddAnsiCTask -TaskPath Service -TaskName TelnetCli
   ```
3. Verify `Logical/Service/TelnetCli/` exists with `ANSIC.prg`, `Init.c`, `Cyclic.c`, `Exit.c`, `Types.typ`, `Variables.var`.
4. Verify `Package.pkg` in the parent folder and `Cpu.sw` include the new task.



## Notes

- Auto-detects installed AS versions from the Windows registry; project version matching is handled automatically
- Technology package registration in `.apj` is handled automatically when a library requires one
- Transitive library dependencies are resolved via `.lby` `<Dependency>` elements and deployed automatically
- If a technology package is already pinned in `.apj`, only the matching TP version's Library folder is searched (version conflicts are reported as an error)
- Output JSON is structured for agent parsing
- Windows only. Requires B&R Automation Studio 6.x and PowerShell 5.1+
