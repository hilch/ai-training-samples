---
description: 'Guidelines for developing SuperTrak control programs for B&R Automation Studio.'
applyTo: '**/*.st,**/*.var,**/*.typ'
---

# B&R SuperTrak Control Instructions

Use this when writing IEC 61131-3 (Structured Text) code that controls a **B&R SuperTrak** — a long-stator linear transport system where independently controlled magnetic **shuttles** (a.k.a. pallets) move along a track made of **sections** and stop at configured **targets** (process stations).

## Required libraries

Two libraries are mandatory and must be added before writing any control code. (These cannot be added via `AS Logical API: Add Library`. If they are not present in the project, prompt the user to download the latest SuperTrak Distribution Package from the B&R website.)

- **`SuperTrak`** — low-level motion and trak control (`SuperTrakInit`, `SuperTrakCyclic1`, `SuperTrakExit`).
- **`StCom`** — higher-level interface function blocks (`StControl`, `StSection`, `StTarget`/`StTargetExt`, `StPallet`) that build on the SuperTrak library.

SuperTrak also requires a collection of binary `.dat` files for configuration. These will live on the target's User partition. These are edited by a separate Windows-based program called TrakMaster while the machine is running. TrakMaster is a critical part of SuperTrak development/maintenance and, while not used for programming, is required to configure the system (e.g., to configure the layout of the trak). TrakMaster is outside the scope of these instructions.

## Layered architecture (build from the bottom up)

Control a SuperTrak in three layers. Always implement them in this order:

```text
Layer 3  High-level control      User-created program       ← decides WHEN to release shuttles
Layer 2  StCom interface tasks   StControl                  ← enable/disable system
                                 StSection                  ← enable/monitor sections
                                 StTarget/StTargetExt       ← enable/monitor targets
                                 StPallet                   ← enable/monitor shuttles
Layer 1  Low-level cyclic        SuperTrakInit/Cyclic1/Exit  ← low-level cyclic control loop
```

- **Layer 1 (SuperTrak library)** handles low-level/motion control of the trak.
- **Layer 2 (StCom library)** provides higher-level handshakes (read a shuttle position, release a shuttle from a target, etc.) and is the layer application code talks to.
- **Layer 3** is application logic that only reads statuses and writes commands through a global (e.g. `gSuperTrak`) — it never calls the SuperTrak or StCom libraries directly. This logic is machine-specific and must be written by the application developer.

### The `gSuperTrak` interface

Layers 2 and 3 should communicate through a global interface. The exact interface name and type may vary between projects, but these instructions will assume a type of `SuperTrak_type` and a global variable named `gSuperTrak`. Here is an example type definition for `SuperTrak_type`:

```st
// SuperTrak_type definition
    SuperTrak_type :     STRUCT  (*Main SuperTrak command/parameter/status structure*)
        System : STSystemInterfaceType; (*SuperTrak System interface*)
        Section : ARRAY[1..ST_SECTION_MAX]OF STSectionInterfaceType; (*SuperTrak Sections interface*)
        Target : ARRAY[1..ST_TARGET_MAX]OF STTargetInterfaceType; (*SuperTrak Targets interface*)
        Shuttle : ARRAY[1..ST_SHUTTLE_MAX]OF STShuttleInterfaceType; (*SuperTrak Shuttles interface*)
    END_STRUCT;
```
*Section, Target, and Shuttle arrays are indexed from 1 to match the SuperTrak hardware numbering.*

Every member follows the same `Cmd` / `Par` / `Status` / `Info` shape:

- **`Cmd`** — what the application wants (e.g. `Power`, `Release.ReleaseToTarget`, `SetShuttleID`).
- **`Par`** — parameters for the command (`Velocity` [mm/s], `Acceleration` [m/s²], `Direction`, `DestinationTarget`, offsets).
- **`Status`** — command response / live state (`Active`, `Ready`, `Error`, `ShuttlePresent`, `Position`, `ShuttleReleased`, `ShuttleCount`).
- **`Info`** — extra diagnostics (faults/warnings bits, section/shuttle counts).

Application code in Layer 3 reads `Status`/`Info` and writes `Cmd`/`Par`. It must never write `Status`/`Info` (those are owned by the Layer 2 tasks).

### Array-size constants

`ST_SECTION_MAX`, `ST_TARGET_MAX`, and `ST_SHUTTLE_MAX` size the `gSuperTrak` arrays. When adding sections, targets, or shuttles, update these constants (and the matching `STSystem` `SystemParameters.NrOf*`). **`ST_TARGET_MAX` must be divisible by 4. If the number of targets isn't divisible by 4, round up to the next multiple of 4.**

### Layer 1 — the cyclic task

This is the hard requirement to run a SuperTrak. **This task MUST run in `Cyclic#1` with an 800 µs cycle time and 0 tolerance.** Register it first in `Cpu.sw`. The task's cycle time can be checked in the Automation Studio configuration's Hardware.hw file. Look for:

```xml
<Parameter ID="Cyclic1Duration" Value="800" />
<Parameter ID="Cyclic1Tolerance" Value="0" />
```

Follow these rules exactly:
- Initialize once in `_INIT` with `SuperTrakInit();`.
- In `_CYCLIC`, call `SuperTrakCyclic1()` every cycle to run the motion control loop. Call `SuperTrakCyclic1()` every cycle unconditionally regardless of init status — the library manages its own internal readiness. Do not gate `SuperTrakCyclic1()` on the result of `SuperTrakInit()`; doing so will prevent the motion controller from running its required cyclic processing.
- Call `SuperTrakExit()` in `_EXIT`.

Do not duplicate this task or move it out of `Cyclic#1`.

### Layer 2 — StCom interface tasks

Each task wraps one StCom concern and exchanges data **only** through the global control structure. Do not call StCom function blocks from application code. Layer 3 application code must communicate with Layer 2 exclusively by reading and writing the shared `gSuperTrak` global variable — never by calling StCom FBs or Layer 2 task programs directly.

| StCom FB | Global handle | Responsibility |
|----------|---------------|----------------|
| `StControl` | `gSuperTrak.System` | Enable/disable the whole trak; system faults/warnings |
| `StSection` | `gSuperTrak.Section[1..ST_SECTION_MAX]` | Enable/monitor each section; section faults/warnings |
| `StTarget`/`StTargetExt` | `gSuperTrak.Target[1..ST_TARGET_MAX]` | Target presence, set shuttle ID; **target-indexed releases** (`ReleaseToTarget` / `ReleaseToOffset` addressed by target) |
| `StPallet` | `gSuperTrak.Shuttle[1..ST_SHUTTLE_MAX]` | Shuttle position/velocity/state; **shuttle-indexed releases** (`ReleaseToTarget` / `ReleaseToOffset` addressed by shuttle ID) |

**Release commands** — both `StTarget`/`StTargetExt` and `StPallet` support the same two release commands:
- **`ReleaseToTarget`** — release the shuttle to a destination target (stops at the target's position).
- **`ReleaseToOffset`** — release the shuttle to a destination target, but offset from that target's position by a configured value.

The commands are identical on both FBs; only the **addressing differs**. Use `gSuperTrak.Target[...]` when the application addresses the release **by target** (release whatever shuttle is at target N), and `gSuperTrak.Shuttle[...]` when it addresses **by shuttle ID** (release shuttle N specifically). Issue a given physical release through **one** path only — never both `Target[]` and `Shuttle[]` for the same move.

Best practice conventions for every interface task:
- Read inputs at the top by copying the relevant `gSuperTrak` substructure into a local variable of the same type (`Interface.Cmd := gSuperTrak.X.Cmd;`), run logic, write outputs at the bottom by copying back from the local variable (`gSuperTrak.X.Status := Interface.Status;`).
- Link each FB to the cyclic data via `StLink := ADR(gSTSystemLink)`. Enable StSection, StTarget/StTargetExt, and StPallet FBs only once `gSuperTrak.System.Status.Active` is true. StControl itself is enabled unconditionally.
- Reset FB errors by assigning the FB's reset input directly: `<FB>.ErrorReset := gSuperTrak.System.Cmd.Reset`. Propagate the error bit to the relevant `gSuperTrak.X.Status.Error` field and set a corresponding `gSuperTrak.X.Info` fault bit so Layer 3 can respond (e.g., halt releases, raise an alarm).

### Layer 3 — high-level control

The application-level control decides **when** shuttles move and issues releases through `gSuperTrak`. The standard release pattern (one-shot per target):

```st
// Set parameters, then raise the command exactly once
IF gSuperTrak.Target[i].Cmd.Release.ReleaseToTarget = FALSE THEN
    gSuperTrak.Target[i].Par.Release.Velocity      := Interface.Par.Velocity;
    gSuperTrak.Target[i].Par.Release.Acceleration  := Interface.Par.Acceleration;
    gSuperTrak.Target[i].Par.Release.Direction     := gSuperTrak.System.Par.GlobalDirection;
    gSuperTrak.Target[i].Par.Release.DestinationTarget := i + 1; // or whatever target is next
    gSuperTrak.Target[i].Cmd.Release.ReleaseToTarget   := TRUE;
END_IF;

// Clear the command once the release is acknowledged
IF gSuperTrak.Target[i].Status.ShuttleReleased THEN
    brsmemset(ADR(gSuperTrak.Target[i].Cmd), 0, SIZEOF(gSuperTrak.Target[i].Cmd));
END_IF;
```

- In a simulation, the trigger to release is often a timer standing in for a real process. **On a real machine, replace the timer with an actual signal** (e.g. a digital input from a robot, a vision "done" bit) — keep the same set-params-then-one-shot-command structure.
- Put release sequencing inside the program's RUN state, gated by the state machine and the system/section status.
- Expose the program through its own global structure (e.g., `gMachine`) using the same `Cmd`/`Par`/`Status` convention so other tasks (HMI, comms) can drive it.

## Checklist for new SuperTrak control code

1. Confirm `SuperTrak` + `StCom` (and dependencies) are registered
2. Confirm the function which calls `SuperTrakCyclic1` is in `Cyclic#1` @ 800 µs / 0 tolerance.
3. Read/write **only** through `gSuperTrak` (or the project's version of it) — never call StCom/SuperTrak FBs from application code.
4. Use the set-`Par` → raise one-shot `Cmd` → clear on `Status.ShuttleReleased` release pattern.
5. If sizing changes, update `ST_SECTION_MAX` / `ST_TARGET_MAX` (÷4) / `ST_SHUTTLE_MAX` and `STSystem.SystemParameters`
6. Register any new task with `AS Logical API: Add ST Task` and update the relevant `Package.pkg` / `Cpu.sw`.
7. When building the project, there may be a warning about licensing ("The project contains one or more components that need to be licensed."). This is normal and can be ignored.

## StCom Library Documentation
### Common Abbreviations
| **Abbreviation** | **Description** |
| --- | --- |
| St | SuperTrak |
| Ext | Extended |
| Com | Communication |
| Err | Error |
| Wrn | Warning |
| Info | Information |
| Succ | Success |
| Fb | Function block |
| Par | Parameter(s) |
| Nr | Number |
| ID | Identification |
| Idx | Index |
| Ctrl | Control |
| Stat | Status |
| Type | Structure definition |
| Enum | Enumerator definition |
| & | Pointer |

### Diagnostics - Logger
The function block "StControl" creates a logger module ("StComLib"), as soon as it is enabled. Detailed information about Errors, Warnings etc. can be found here.
Function blocks that can be used in multiple instances (StTarget, StTargetExt, StPallet, and StSection) are named as follows in the "OS Task" column:
 - StTarget #[TargetID] (e.g. StTarget_X with configuration StTarget_X.Target = 2 ... "StTarget #2")
 - StTargetExt #[TargetID]
 - StPallet #[PalletID]
 - StSection #[SectionID]

The name in "OS Task" is not related to the name of the function block in the variable declaration (e.g. StTarget_0 with configuration StTarget_0.Target = 2 will appear as "StTarget #2" and not as "StTarget #0"). Therefore it is suggested to name the function block after its configuration (e.g. StTarget_2 with StTarget_2.Target = 2). This makes it easier to identify the function block, which reported an error.
