---
description: Coding guidelines for B&R Automation Studio projects (IEC 61131-3 Structured Text, Types, Variables).
applyTo: "**/*.typ,**/*.var,**/*.st"
---

# B&R Automation Studio Coding Guidelines

You are an expert in B&R Automation Studio (6.0+) and IEC 61131-3 Structured Text.
Follow these guidelines when generating code for `.st`, `.var`, and `.typ` files in B&R Automation Studio projects.

## Naming Conventions

### Variables
- **Local Variables:** Use `CamelCase` starting with uppercase.
  - Example: `ActPressure`, `CommandCount`.
  - **Exception:** Loop/index variables use lowercase single letters or short lowercase names (e.g., `i`, `j`).
- **Global Variables:** Use `CamelCase` with prefix `g`.
  - Example: `gMainInterface`, `gMainConfig`.
- **Constants:** Use `SCREAMING_SNAKE_CASE`.
  - Example: `MAX_RETRY_COUNT`, `DEFAULT_TIMEOUT_MS`.
- **Pointers:** Use prefix `p` + descriptive name.
  - Example: `pAxisStatus`, `pRecipeData`.
- **IO Variables:** Use specific prefixes based on type:
  - `di` (Digital Input): `diStartButton`
  - `do` (Digital Output): `doLampGreen`
  - `ai` (Analog Input): `aiTankLevel`
  - `ao` (Analog Output): `aoValveSetpoint`
  - `at` (Temperature): `atOvenTemp`
  - `si` (Safety Input): `siGuardClosed`

### Types and Structures
- **User Types:** Start uppercase, `CamelCase`, end with `Type`.
  - Example: `RecipeType`, `LocalHwType`.
- **Substructures:** Pattern `<parent><NewName>Type`.
  - Example: `RecipeTimeType` (child of `RecipeType`).
- **Enumerators:** Name ends with `Enum`. Members are `ENUMNAME_VALUE` in `SCREAMING_SNAKE_CASE`.
  - Example: `AlarmReactionEnum` with member `ALARMREACTION_STOP`.

### Function Blocks
- **Instances:** Name should begin with the FB type.
  - Example: `TON_delay`, `TON_0`.
  - Arrays: `TON_` (omit suffix numbers).

## Documentation
- **Descriptions:** All variables and types in `.var` and `.typ` files MUST have a description comment.
- **Units:** Include engineering units in brackets `[unit]` within the description where relevant.
  - Example: `ActPressure : REAL; (* Actual hydraulic pressure [bar] *)`

## Project Structure & File Organization

### Package File Management (`Package.pkg`)
The `Package.pkg` file is the backbone of the Automation Studio project structure. It lists all files and sub-packages contained within a directory.
**CRITICAL:** Whenever you create, rename, or delete a file or folder, you **MUST** update the corresponding `Package.pkg` file.

**Example `Package.pkg`:**
```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
    <Object Type="File">Global.typ</Object>
    <Object Type="File">Global.var</Object>
    <Object Type="Package">Main</Object>
    <Object Type="File">MyNewFile.st</Object> <!-- Added file -->
  </Objects>
</Package>
```

### Folder Hierarchy
Organize the project in a logical package hierarchy. Each functional module or task has its own package.

```text
ProjectName/
├─ Global.typ              # Global type definitions
├─ Global.var              # Global variable declarations
├─ Module1/                # Task package
│  ├─ global.typ           # Module-specific global types
│  ├─ global.var           # Module-specific global variables
│  ├─ local.st             # Main task program (Init, Cyclic, Exit)
│  ├─ local.typ            # Local type definitions
│  ├─ local.var            # Local variable declarations
│  ├─ SubModule/           # Optional nested sub-package
│  │  ├─ ...               # Same structure
```

### Task Files
- **`local.st` (Task Program):** Contains `_INIT`, `_CYCLIC`, and `_EXIT` routines.
  - **`_INIT`:** One-time initialization logic (reset variables, configure FBs).
  - **`_CYCLIC`:** Recurring logic executed every task cycle.
  - **`_EXIT`:** Cleanup on task shutdown.
- Use **actions** (`.st` action files) to split complex cyclic logic into named, readable pieces.

### Adding a New Task (Checklist)

**Important:** Task names in `Cpu.sw` are limited to **10 characters**. Longer logical folder names must be truncated only in the `<Task Name="…">` attribute — the `Source` path, `Package.pkg` entry, and logical folder name are **not** subject to this limit.

When adding a new task use the skill `as-logical-api : Add ST Task` to add the task and register it in `Cpu.sw` and `Package.pkg`!


## Coding Best Practices

### Function Blocks
- Call function blocks at the end of the scan or outside state machines.
- Assign parameters before the call.
- Example:
  ```iec-st
  TON_delay.PT := config.delayTime;
  TON_delay.IN := TRUE;
  TON_delay();
  ```

### Edge Detection
- Prefer using `R_TRIG` / `F_TRIG` function blocks for edge detection.
- In looped or indexed code, explicitly track old values rather than relying on implicit `EDGEPOS`/`EDGENEG`.

### Alarm Handling
- Use `MpAlarmX` function blocks for alarm management.
- Check alarm reactions using `MpAlarmXCheckReaction`.

---

## Examples

### Type Declaration (.typ)
```iec-st
TYPE
    (* Main local structure *)
    LocalType : STRUCT
        Hw : LocalHwType;       (* Hardware I/O references *)
        State : MyStateEnum;    (* Current state machine state *)
        Timer : TON;            (* General purpose timer *)
    END_STRUCT;

    (* State enumeration *)
    MyStateEnum :
    (
        MYSTATE_IDLE := 0,
        MYSTATE_RUNNING := 10,
        MYSTATE_ERROR := 99
    );
END_TYPE
```

### Variable Declaration (.var)
```iec-st
VAR
    Local : LocalType;          (* Local module data *)
    TON_watchdog : TON;         (* Watchdog timer *)
END_VAR

VAR CONSTANT
    MAX_ITEMS : USINT := 10;    (* Maximum number of items *)
END_VAR
```

### Task Cyclic Code (.st)
```iec-st
PROGRAM _CYCLIC

    (* State Machine *)
    CASE Local.State OF
        MYSTATE_IDLE:
            IF diStartButton THEN
                Local.State := MYSTATE_RUNNING;
            END_IF;

        MYSTATE_RUNNING:
            doLampGreen := TRUE;
            (* ... process logic ... *)

        MYSTATE_ERROR:
            doLampGreen := FALSE;
    END_CASE;

    (* FB Calls - always at end of scan *)
    Local.Timer();

END_PROGRAM
```
