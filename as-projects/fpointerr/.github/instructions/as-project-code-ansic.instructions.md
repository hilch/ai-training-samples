---
description: Guidelines for programming B&R PLC applications in ANSI C and C headers for Automation Studio.
applyTo: "**/*.c,**/*.h"
---

# B&R Automation Studio ANSI C Guidelines

You are an expert in B&R Automation Studio (6.0+) and ANSI C for Automation Runtime.
Follow these guidelines when generating code for `.c` and `.h` files in B&R Automation Studio projects.

## Naming Conventions

### Variables
- **Use *.var files for declarations**. 
  - **DO NOT** use declarations in *.h and *.c files.
  - **DO NOT** use 'static' declarations
  - **DO NOT** use declarations on stack
- **Local Variables:** Use `PascalCase` starting always with uppercase and no underscore.
  - Example: `ActPressure`, `CommandCount`.
  - **Exception:** variables that are declared locally in functions should be `camelCase` (e.g. `myVar`)
  - **Exception:** for(){}, do {} while(), while(){}/index variables use lowercase single letters or short lowercase names (e.g., 	`i`, `j`).
  - **ALWAYS** place local variables in a *.var file inside the ANSI-C POU
- **Global Variables:** Use `camelCase` with prefix `g`.
  - Example: `gMainInterface`, `gMainConfig`.
	- **ALWAYS** place Global Variables in a *.var file used for Module-specific global variables
	- **ALWAYS** place Types for Global Variables in a *.typ file used for Module-specific global types
- **Constants:** Use `SCREAMING_SNAKE_CASE`.
  - Example: `MAX_RETRY_COUNT`, `DEFAULT_TIMEOUT_MS`.
	- **ALWAYS** place global constants in a *.var file used for Module-specific global variables	
	- **ALWAYS** place local constants in a *.var file inside the ANSI-C POU	
- **Pointers:** Use prefix `p` + descriptive name.
  - Example: `pAxisStatus`, `pRecipeData`.
- **IO Variables:** Use `camelCase`. Use specific prefixes based on type:
  - `di` (Digital Input): `diStartButton`
  - `do` (Digital Output): `doLampGreen`
  - `ai` (Analog Input): `aiTankLevel`
  - `ao` (Analog Output): `aoValveSetpoint`
  - `at` (Temperature): `atOvenTemp`
  - `si` (Safety Input): `siGuardClosed`

### Types and Structures
- **use *.typ files for definition** 
   - DO NOT use struct definition in *.h and *.c files for all variables in *.var files.
- **use typedefs from <bur/plctypes>** instead of C types in function signatures and for return values.
  - Example: `signed int myFunc( float param1, unsigned short* param2)` becomes `INT myFunc( REAL param1, UINT* param2)`
- **User Types:** Start uppercase, `PascalCase`, end with `Type`.
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

## Purpose

- Use ANSI C in Automation Studio for problem-oriented program sections such as calculations, algorithms, protocol handling, and reuse of existing C code.
- DO NOT treat ANSI C code as a standalone desktop application. It is still part of a PLC task or POU executed by Automation Runtime.
- Prefer ANSI C only when it materially improves clarity, reuse, or deterministic performance over IEC code.

## Project Structure & File Organization

### Package File Management (`Package.pkg`)
The `Package.pkg` file defines the Automation Studio project structure for each logical folder.
**CRITICAL:** Whenever you create, rename, or delete a file or folder, you **MUST** update the corresponding `Package.pkg` file.

**Example `Package.pkg`:**
```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
    <Object Type="File">Global.typ</Object>
    <Object Type="File">Global.var</Object>
    <Object Type="Package">Module</Object>
  </Objects>
</Package>
```

### Folder Hierarchy of a package
Organize the project in a logical package hierarchy. Each functional module or task has its own package.

```text
ProjectName/
├─ Global.typ              # Global type definitions
├─ Global.var              # Global variable declarations
├─ Module1/                # Task package
│  ├─ Module1.typ          # Module-specific global types
│  ├─ Module1.var          # Module-specific global variables
│  ├─ Package.pkg          # contains folder structure
│  ├─ SubModule1/          # Optional nested sub-package
│  │  ├─ ...               # Same structure
```

### Adding a New Task (Checklist)

**Important:** Task names in `Cpu.sw` are limited to **10 characters**. Longer logical folder names must be truncated only in the `<Task Name="…">` attribute — the `Source` path, `Package.pkg` entry, and logical folder name are **not** subject to this limit.

When adding a new task use the skill `as-logical-api : AddAnsiCTask` to add the task and register it in `Cpu.sw` and `Package.pkg`!


## ANSI-C POU management

Whenever you create, rename, or delete `.c`, `.h`, or related task files, you **MUST** update the corresponding `ANSIC.prg` file.

Use `PascalCase` for all filenames.

In addition to the program or library description file and the declaration files for data types and variables, an ANSI C POU consists of one or more source files. The following files can be included in an ANSI C POU:

| Filename        | Description                                                                         |
|-----------------|-------------------------------------------------------------------------------------|
| AnsiC.prg       | Description file for programs whose source code is created in ANSI C                |
| [FileName].var  | Declaration file for local variables inside ANSI-C POU                              |
| [FileName].typ  | Declaration file for local types inside ANSI-C POU.                                 |
| [FileName].c    | ANSI C source file(s) for implementing the initialization and cyclic program parts. |
| [FileName].h    | ANSI C include file(s) for declaring C-internal data types, variables, and macros.  |
| [FileName].s    | Assembler source text file.                                                         |
| [FileName].o    | Object file                                                                         |
| [FileName].a    | ANSI C object library.                                                              |

### Folder Hierarchy of ANSI-C package

```
├─ AnsiCprog/              # package with ANSI-C program
│  ├─ ANSIC.prg            # Description file for programs whose source code is created in ANSI C
│  ├─ Types.typ            # Module-specific local types
│  ├─ Variables.var        # Module-specific local variables
│  ├─ Local.h              # shared declarations, macros, constants, and prototypes
│  ├─ Init.c               # C source with _INIT function
│  ├─ Cyclic.c             # C source with _CYCLIC function
│  ├─ Exit.c               # C source with _EXIT function
```

**Example `ANSIC.prg`:**

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Program SubType="ANSIC" xmlns="http://br-automation.co.at/AS/Program">
  <Files>
    <File Description="Cyclic code">Cyclic.c</File>
    <File Description="Initialization code">Init.c</File>
    <File Description="Exit code">Exit.c</File>
    <File Description="Local data types" Private="true">Types.typ</File>
    <File Description="Local variables" Private="true">Variables.var</File>
  </Files>
</Program>
```


### ANSI C Task Files
- Follow the Automation Studio task lifecycle and place logic in the correct routine:
  - **`_INIT`** for one-time initialization, allocation, pointer setup, and runtime handle validation.
  - **`_CYCLIC`** for deterministic code executed every task cycle.
  - **`_EXIT`** for cleanup when the task actually requires it.
- Match the repository's existing file split where present. Prefer separate task source files such as `Init.c`, `Cyclic.c`, optional `Exit.c`, and one task-local header such as `Local.h`. 
- Keep shared declarations, macros, constants, and prototypes in the local header. Keep implementation logic in `.c` files.
- When `_DEFAULT_INCLUDES` is used, keep the Automation Studio default include pattern intact:

```c
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif
```

## Coding Rules

### Runtime Model
- Write ANSI C as PLC code with bounded execution time.
- Keep `_CYCLIC` deterministic and scan-safe.
- DO NOT implement blocking behavior, unbounded loops, interactive console I/O, and heavy work that can overrun the task cycle.
- Keep initialization logic in `_INIT` and cleanup in `_EXIT`.

### Data Types and Interfaces
- Prefer B&R and Automation Runtime data types, calling conventions, and supported libraries over host-platform assumptions.
- Respect B&R limitations for PLC variables and data types in ANSI C.
- Be explicit at mixed IEC/C boundaries. Verify declarations, sizes, memory layout expectations, pointers, and visibility before changing shared data.
- DO NOT introduce C++ constructs or desktop-specific APIs in ANSI C files.

### Headers
- Use task-local headers for shared task declarations and macros.
- Include only what the task needs.
- Prefer the local header plus required Automation Runtime headers over broad include chains.
- Keep macros simple and task-focused. Do not hide complex side effects in macros unless an existing local pattern already uses them.

### Error Handling
- Validate return codes immediately when allocating memory, opening runtime objects, or calling Automation Runtime services.
- On unrecoverable initialization failures, stop task execution safely and leave a clear status or error message for diagnostics.
- Keep failure paths explicit and easy to trace.

## Mixed IEC and ANSI C

- Keep C interfaces aligned with the corresponding IEC declarations.
- Do not change shared types or variable ownership casually.
- When using function blocks, runtime objects, or mapped PLC data from C, keep call order and state transitions explicit.

## Repository Conventions

- Follow naming, include style, and file layout already used by existing ANSI C tasks in `Logical/`.
- Prefer small, local changes over broad refactors.
- Do not edit generated files in `Binaries/`, `Diagnosis/`, or `Temp/`.

## Example Task

### Type Declaration (.typ)
```iec-st
(* Main local structure *)

TYPE
	LocalHwType : 	STRUCT 
		Param1 : REAL;
		Param2 : DINT;
	END_STRUCT;
	LocalType : 	STRUCT 
		Hw : LocalHwType; (* Hardware I/O references *)
		State : MyStateEnum; (* Current state machine state *)
		Timer : TON; (* General purpose timer *)
	END_STRUCT;
END_TYPE

(* State enumeration *)

TYPE
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
	Local : LocalType; (* Local module data *)
	TON_watchdog : TON; (* Watchdog timer *)
END_VAR
VAR CONSTANT
	MAX_ITEMS : USINT := 10; (* Maximum number of items *)
END_VAR
```

### _INIT

```c
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// avoid placing variable definitions here

void _INIT ProgramInit(void)
{
	Local.Hw.Param1 = 3.14; // initialize parameters
	Local.Hw.Param2 = 3;
	Local.Timer.PT = 4100; // initialize timer with 4.1 seconds
	Local.State = MYSTATE_IDLE; // initialize state machine
}
```

### _CYCLIC

```c
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// avoid placing variable definitions here

void _CYCLIC ProgramCyclic(void)
{
	/* state machine */
	switch( Local.State ){
		/* wait for commands */
		case MYSTATE_IDLE:
			if( diStartButton ){
				Local.State = MYSTATE_RUNNING;
			}
			break;

		/* execute commands */
		case MYSTATE_RUNNING:
			doLampGreen = 1;
			/* process logic ... */

			if( !siGuardClosed ){ /* unexpected behaviour */
				Local.State = MYSTATE_ERROR;
			}
			break;	
	
		/* error state */
		case MYSTATE_ERROR:
			break;		
	}

	/* FB Calls - always at end of scan */
	TON( &Local.Timer );
	TON( &TON_watchdog );

}
```

### _EXIT

```c
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// avoid placing variable definitions here

void _EXIT ProgramExit(void)
{

}
```