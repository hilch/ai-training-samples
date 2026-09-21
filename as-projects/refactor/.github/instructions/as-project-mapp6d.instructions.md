---
description: Guidelines for configuring ACOPOS6D assembly and layout.
applyTo: '**/*.st,**/*.var,**/*.typ,**/*6D.var,**/*.assembly6d,**/*.layout6d'
---

# B&R Automation Studio mapp6D configuration file guidelines

Use this when creating or changing ACOPOS6D tasks and related mapp6D configuration in Automation Studio 6.  
**Goal:** build valid Structured Text tasks and consistent mapp6D configuration.

## Scope

- Applies to mapp6D configuration files under `Physical/<Configuration>/<CPU>/mapp6D/` and ST code that references them:
  - `Physical/<Configuration>/<CPU>/mapp6D/**/*.assembly6d`
  - `Physical/<Configuration>/<CPU>/mapp6D/**/*.layout6d`
  - matching `Package.pkg` files
  - `Logical/**/*.st`
  - `Logical/**/*.var`
  - `Logical/**/*.typ`
- Do not edit `Logical/Libraries/mapp6D/*`.

## Project Pattern to Follow for Configuration Files

mapp6D is structured like this:

```text
mapp6D/
  Package.pkg (SubType="mapp6D" PackageType="mapp6D")
  <AssemblyName>/
    c<AssemblyName>.assembly6d
    l<AssemblyName>.layout6d
    ... optional subpackages with additional configuration files
```

If you add, remove, or rename files or folders, always update the related `Package.pkg`.

## Package.pkg Rules

- Root `mapp6D/Package.pkg` must include:
  - `SubType="mapp6D"`
  - `PackageType="mapp6D"`
- Subpackages use plain `<Package ...>` (no subtype).

Example:

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package SubType="mapp6D" PackageType="mapp6D" xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
    <Object Type="File">cAsm6D.assembly6d</Object>
    <Object Type="File">lAsm6D.layout6d</Object>
  </Objects>
</Package>
```

# Assembly Configuration File Structure (*.assembly6d)

- Follow the example structure provided below.
- Keep IDs and group structure consistent for ACOPOS6D assemblies.

Example structure:

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Configuration>
  <Element ID="gAssembly6D" Type="assembly6d">
    <Property ID="Simulation6D" Value="SimOn" />
    <Group ID="Ethernet6D">
      <Property ID="IpAddress" Value="192.168.10.100" />
      <Property ID="SubnetMask" Value="255.255.255.0" />
      <Property ID="Gateway" Value="192.168.10.1" />
    </Group>
    <Group ID="Shuttles">
      <Property ID="MaxShuttleCount" Value="5" />
      <Property ID="ShuttleIDMode" Value="RealTimeID" />
      <Property ID="RealModeDetectionType" Value="Any" />
      <Group ID="DefaultPosition">
        <Property ID="DefZ" Value="1.0" /> <!-- default value is 1.0, valid range is 0.5..4.0, unit is mm -->
        <Property ID="DefRx" Value="0.0" /> <!-- default value is 0.0, valid range is -0.1..0.1, unit is mrad -->
        <Property ID="DefRy" Value="0.0" /> <!-- default value is 0.0, valid range is -0.1..0.1, unit is mrad -->
        <Property ID="DefRz" Value="0.0" /> <!-- default value is 0.0, valid range is -0.1..0.1, unit is mrad -->
      </Group>
      <Group ID="Limits">
        <Group ID="InPlaneLimits">
          <Property ID="InPlaneVelLim" Value="1.0" /> <!-- default value is 1.0, valid range is 0.01..2.0, unit is m/s -->
          <Property ID="InPlaneAccLim" Value="10.0" /> <!-- default value is 10.0, valid range is 1.0..20.0, unit is m/s² -->
          <Property ID="InPlaneEmergDec" Value="10.0" /> <!-- default value is 10.0, valid range is 1.0..20.0, unit is m/s² -->
        </Group>
        <Group ID="ShortAxLim">
          <Group ID="ShortAxAccLim">
            <Property ID="AccZLim" Value="10.0" /> <!-- default value is 10.0, valid range is 0.5..20.0, unit is m/s² -->
            <Property ID="AccRxLim" Value="10.0" /><!-- default value is 10.0, valid range is 0.15..10.0, unit is mrad/s² -->
            <Property ID="AccRyLim" Value="10.0" /><!-- default value is 10.0, valid range is 0.15..10.0, unit is mrad/s² -->
            <Property ID="AccRzLim" Value="10.0" /><!-- default value is 10.0, valid range is 0.6..20.0, unit is mrad/s² -->
          </Group>
          <Group ID="ShortAxVelLim">
            <Property ID="VelZLim" Value="0.15" /> <!-- default value is 0.15, valid range is 0.0005..0.5, unit is m/s -->
            <Property ID="VelRxLim" Value="0.1" /> <!-- default value is 0.1, valid range is 0.02..1.0, unit is rad/s -->
            <Property ID="VelRyLim" Value="0.1" /> <!-- default value is 0.1, valid range is 0.02..1.0, unit is rad/s -->
            <Property ID="VelRzLim" Value="0.3" /> <!-- default value is 0.3, valid range is 0.02..1.0, unit is rad/s -->
          </Group>
        </Group>
      </Group>
      <Group ID="UserData">
        <Property ID="UserDataVariable" />
      </Group>
    </Group>
    <Group ID="CyclicChannel6D">
      <Group ID="ReadChannel">
        <Property ID="Xaxis" Value="TRUE" /> <!-- default value is TRUE, set to FALSE to disable reading shuttle position in X axis -->
        <Property ID="Yaxis" Value="TRUE" /> <!-- default value is TRUE, set to FALSE to disable reading shuttle position in Y axis -->
        <Property ID="Zaxis" Value="TRUE" /> <!-- default value is TRUE, set to FALSE to disable reading shuttle position in Z axis -->
        <Property ID="Rxaxis" Value="TRUE" /> <!-- default value is TRUE, set to FALSE to disable reading shuttle position in Rx axis -->
        <Property ID="Ryaxis" Value="TRUE" /> <!-- default value is TRUE, set to FALSE to disable reading shuttle position in Ry axis -->
        <Property ID="Rzaxis" Value="TRUE" /> <!-- default value is TRUE, set to FALSE to disable reading shuttle position in Rz axis -->
      </Group>
    </Group>
    <Group ID="SystemReactions6D">
      <Property ID="LostCommReaction6D" Value="Continue" />
    </Group>
    <Group ID="Fieldbus6D">
      <Property ID="SyncMode6D" Value="ExternalPlkSync" />
      <Property ID="IFName" Value="IF3" />
      <Property ID="NodeNumber" Value="1" />
    </Group>
    <Group ID="IOMapp">
      <Property ID="Input" Value="::gCommInput6D" />
      <Property ID="Output" Value="::gCommOutput6D" />
    </Group>
    <Group ID="Layout">
      <Property ID="Layout6D[1]" Value="AsmLayout6D" />
    </Group>
  </Element>
</Configuration>
```

- Keep these IDs unchanged in naming and hierarchy: `Simulation6D`, `Ethernet6D`, `Shuttles`, `CyclicChannel6D`, `SystemReactions6D`, `Fieldbus6D`, `IOMapp`, `Layout`.

## Assembly Configuration Guidelines (*.assembly6d)

### File Naming
- Each `assembly6d` file must have a unique name.  
  - Example: `cAsm6D.assembly6d` located in the `Asm6D/` directory.

### Assembly Definition
- Inside each `assembly6d` file, there must be exactly one assembly element defined.
- The assembly element must have a unique ID.  
  - Example: `gAssembly6D`.
  - Naming pattern for the assembly element should be consistent across the project, for example using `gAssembly6D_<AssemblyName>` to ensure uniqueness and clarity in multi-assembly projects.

### Summary
- 1 file = 1 assembly
- Unique file name and unique assembly ID are both mandatory

## Segment simulation in the assembly configuration file.
- Segment simulation must be explicitly defined:
  - `Simulation6D="SimOn"` when simulation is used.
  - `Simulation6D="SimOff"` when simulation is not used.

## Ethernet configuration must be defined in the assembly configuration file.
- Ethernet configuration must define `IpAddress`, `SubnetMask`, and `Gateway` in `Ethernet6D`.

## Shuttle configuration must be defined in the assembly configuration file.
- Shuttle configuration must define `MaxShuttleCount`, `ShuttleIDMode`, and `RealModeDetectionType` in `Shuttles`.

- `MaxShuttleCount`:
  - Valid range: integer `1..80`.

- `ShuttleIDMode`:
  - Valid values: `RealTimeID`, `AbsoluteIDNoR`, `AbsoluteIDWR`.
  - `RealTimeID`: shuttle gets incremental ID when placed on a segment.
  - `AbsoluteIDNoR`: absolute ID without orientation detection.
  - `AbsoluteIDWR`: absolute ID with orientation detection.

- `RealModeDetectionType`: default `Any`, valid values as below
  | Value | Shuttle Type |
  |---|---|
  | `Any` | Combination of different shuttle types |
  | `120x120` | S3 shuttle size 120x120 |
  | `180x120` | S3 shuttle size 180x120 |
  | `240x120` | S3 shuttle size 240x120 |
  | `180x180` | S3 shuttle size 180x180 |
  | `210x180` | S3 shuttle size 210x180 |
  | `210x210` | S3 shuttle size 210x210 |
  | `240x240` | S3 shuttle size 240x240 |
  | `330x210` | S3 shuttle size 330x210 |
  | `300x300` | S3 shuttle size 300x300 |
  | `330x330` | S3 shuttle size 330x330 |
  | `450x450` | S3 shuttle size 450x450 |
  | `S4_160x160` | S4 shuttle size 160x160 |
  | `S4_160x240` | S4 shuttle size 160x240 |
  | `S4_160x320` | S4 shuttle size 160x320 |
  | `S4_240x240` | S4 shuttle size 240x240 |
  | `S4_240x280` | S4 shuttle size 240x280 |
  | `S4_280x280` | S4 shuttle size 280x280 |
  | `S4_320x320` | S4 shuttle size 320x320 |
  | `S4_280x440` | S4 shuttle size 280x440 |
  | `S4_400x400` | S4 shuttle size 400x400 |
  | `S4_440x440` | S4 shuttle size 440x440 |
  | `S4_600x600` | S4 shuttle size 600x600 |

- `DefaultPosition`:
  - Must define `DefZ`, `DefRx`, `DefRy`, `DefRz`.
  
- In `UserData`, `UserDataVariable` can be defined to link a user-defined variable in PLC to the shuttle. For example, if a user defines `UserDataVariable="::gShuttleUserData6D"`, they can use the `gShuttleUserData6D` variable in PLC to read or write user data of the shuttle. This can consist of product status, product type, process state, or any other information that the user wants to link to the shuttle or products on the shuttle. Users can use the `MC_BR_ShCopyUserData_Acp6D` function block to copy data between shuttle user data and a PLC variable.

## Cyclic channel must be defined in the assembly configuration file.

- `CyclicChannel6D/ReadChannel`:
  - Must define `Xaxis`, `Yaxis`, `Zaxis`, `Rxaxis`, `Ryaxis`, `Rzaxis`

## System reaction must be defined in the assembly configuration file.
- `SystemReactions6D`:
  - `LostCommReaction6D` valid values: `Continue`, `Stop`, `PowerOff`
    - `Continue`: system will try to continue operation while recovering communication
    - `Stop`: system will stop all shuttle movement immediately until communication is recovered
    - `PowerOff`: system will perform emergency stop and power off all segments until communication is recovered
  - Default should be `Continue`

## Fieldbus must be defined in the assembly configuration file.
- `Fieldbus6D`:
  - Must define `SyncMode6D`, `IFName`, `NodeNumber`
  - `SyncMode6D` valid values: `InternalSync`, `ExternalPlkSync`, `External6DSync`
  - `SyncMode6D` default should be `ExternalPlkSync`
  - `IFName` values depend on hardware (commonly `IF1`, `IF2`, `IF3`)
  - `IFName` default should be `IF3`
  - `NodeNumber` valid range: integer `1..239`, default `1`

## I/O mapping must be defined in the assembly configuration file.
- `IOMapp`:
  - Must define `Input` and `Output`
  - Use valid global variable references, for example:
    - `Input="::gCommInput6D"`
    - `Output="::gCommOutput6D"`

## Layout reference must be defined in the assembly configuration file.
- `Layout`:
  - Must define `Layout6D[1]`
  - Value must reference a valid `layout6d` object in the project (for example `AsmLayout6D`)

# Layout configuration file structure (*.layout6d)
- Follow the structure of existing `layout6d` files in this repository
- Keep the file name and object name aligned with assembly references
- Ensure all referenced segments and objects exist in the project and are registered in Package.pkg

Example structure:

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Configuration>
  <Element ID="AsmLayout6D" Type="layout6d">
    <Property ID="GridSize" Value="0" />
    <Group ID="UserOrigin">
      <Property ID="OffsetX" Value="0" />
      <Property ID="OffsetY" Value="0" />
    </Group>
    <Group ID="Segments">
      <Property ID="SegRow" Value="4" />
      <Property ID="SegCol" Value="4" />
      <Group ID="Segment[1]">
        <Property ID="TypeGrid240" Value="0" />
        <Property ID="TypeGrid120" Value="0" />
        <Property ID="TypeGrid320" Value="0" />
        <Property ID="TypeGrid160" Value="0" />
        <Property ID="X" Value="0" />
        <Property ID="Y" Value="0" />
        <Property ID="Name" Value="SegA" />
        <Property ID="IsMaster" Value="True" />
        <Property ID="PowerSectorName" />
        <Group ID="Gaps">
          <Property ID="Left" Value="0" />
          <Property ID="Bottom" Value="0" />
          <Property ID="Right" Value="0" />
          <Property ID="Top" Value="0" />
          <Property ID="S4Left" Value="0" />
          <Property ID="S4Bottom" Value="0" />
          <Property ID="S4Right" Value="0" />
          <Property ID="S4Top" Value="0" />
        </Group>
        <Group ID="BorderID">
          <Property ID="Left" Value="0" />
          <Property ID="Bottom" Value="0" />
          <Property ID="Right" Value="0" />
          <Property ID="Top" Value="0" />
        </Group>
      </Group>
    </Group>
    <Selector ID="Synchronization" Value="NotUsed" />
  </Element>
</Configuration>
```

## Layout Configuration Guidelines (*.layout6d)

### Layout Definition
- Keep these IDs unchanged in naming and hierarchy: `GridSize`, `UserOrigin`, `Segments`, `SegRow`, `SegCol`, `Segment[1]`, `TypeGrid240`, `TypeGrid120`, `TypeGrid320`, `TypeGrid160`, `X`, `Y`, `Name`, `IsMaster`, `PowerSectorName`, `Gaps`, `Left`, `Bottom`, `Right`, `Top`, `S4Left`, `S4Bottom`, `S4Right`, `S4Top`, `BorderID`, and `Synchronization`

### Grid size definition in layout configuration file
- `GridSize` default value is `0` and valid range is `0..3`

### Rules for user-defined offset in layout configuration file
- In ID `UserOrigin`, users can set a user-defined offset for the assembly origin. The defined offset is considered in shuttle position calculation and can be used to align the shuttle position with external references in the application. Users can set X and Y offsets in `OffsetX` and `OffsetY`, respectively, in mm.

### Rules for Segment definition in layout configuration file
- In one layout (`<Element Type="layout6d">`), the `Segment` IDs and `Name` properties must be unique to avoid conflicts. For example, if `Segment[1]` has `Name="SegA"`, no other segment in that same layout can have `Name="SegA"`.
- Naming pattern for `Segment` IDs should be consistent across the project, for example using `Segment[1]`, `Segment[2]`, etc. to ensure clarity and maintainability in multi-segment layouts.
- Naming pattern for `Name` property should be consistent across the project, for example using `SegA`, `SegB`, etc. to ensure clarity and maintainability in multi-segment layouts.
- In a configuration, `IsMaster` should be set to `True` for only one segment.
- IDs for elements with `Type="layout6d"` must be unique across the project (all `*.layout6d` files). For example, if one layout uses `ID="AsmLayout6D"`, no other layout element in the project may reuse `ID="AsmLayout6D"`.
- There can be multiple layouts with `Type="layout6d"` defined in a single file, but only one layout can be referenced by the assembly configuration file through `Layout6D[1]`. For example, if there are two layouts with IDs `AsmLayout6D` and `AsmLayout6D_Variant`, only one of them can be referenced by `Layout6D[1]`.
- Naming pattern for Layout IDs should be consistent across the project, for example using `AsmLayout6D_<Layout>` to ensure clarity and maintainability in multi-layout projects.

### Rules on the defined `GridSize`, value assignment to `TypeGrid240`, `TypeGrid120`, `TypeGrid320`, `TypeGrid160` must be followed
**Configuration table:**

| GridSize | System Type | Grid Type | Active TypeGrid property | Allowed values for active property | Other TypeGrid properties |
|---|---|---|---|---|---|
| 0 | S3 | Full Grid | `TypeGrid240` | `0` | Ignored |
| 1 | S3 | Half Grid | `TypeGrid120` | `0`, `1`, `2` | Ignored |
| 2 | S4 | Full Grid | `TypeGrid320` | `0` | Ignored |
| 3 | S4 | Half Grid | `TypeGrid160` | `0`, `1`, `2` | Ignored |

#### Segment type value summary

  - Segment table for TypeGrid240, TypeGrid120, TypeGrid320, TypeGrid160: 
  | Value | Segment shape |
  |---|---|
  | `0` | Square segment |
  | `1` | Horizontal rectangle segment (wider in X direction) |
  | `2` | Vertical rectangle segment (wider in Y direction) |

### Rules for user defined gap definition in layout configuration file
- For group ID `Gaps`, there can be values set for `Left`, `Bottom`, `Right`, `Top` or `S4Left`, `S4Bottom`, `S4Right`, `S4Top`. For example, if the layout is designed for S3 (`GridSize` is 0 or 1), value range is `0..4`. If the layout is designed for S4 (`GridSize` is 2 or 3), value range is `0..3`
  - `Left`, `Bottom`, `Right`, `Top`
    | Value | Gap |
    |---|---|
    | `0` | no gap |
    | `1` | 5mm gap |
    | `2` | 10mm gap |
    | `3` | 60mm gap |
    | `4` | 20mm gap |
  - `S4Left`, `S4Bottom`, `S4Right`, `S4Top`
    | Value | Gap |
    |---|---|
    | `0` | no gap |
    | `1` | 6.66mm gap |
    | `2` | 13.33mm gap |
    | `3` | 26.66mm gap |

### Synchronization definition in layout configuration file

- If `Synchronization` is set to `Used`, the defined `SegmentID` and `Edge` determine when the layout synchronizes with the assembly during shuttle movement. For example, if `SegmentID` is `1` and `Edge` is `0`, the layout synchronizes when a shuttle crosses the TOP edge of Segment 1.
- In a configuration, if `Synchronization` is set to `NotUsed`, the layout will not synchronize with the assembly.
- If set to `Used`, `SegmentID` and `Edge` should be defined. For example:
```xml
    <Selector ID="Synchronization" Value="Used">
      <Property ID="SegmentID" Value="1" />
      <Property ID="Edge" Value="0" />
    </Selector>
```
#### Synchronization edge definition in layout configuration file

- `Edge`: default value is `0` and valid range is as below
  | Value | Synchronization edge |
  |---|---|
  | `0` | TOP edge of segment |
  | `1` | RIGHT edge of segment |
  | `2` | BOTTOM edge of segment |
  | `3` | LEFT edge of segment |

# Coding mapp6D in Structured Text code

## Primary rules for communication between mapp6D and the 6D controller

- User should use Input and Output variables defined in the assembly configuration file in ST code and in the assembly configuration.
  - For example, if the assembly configuration file defines `Input="::gCommInput6D"` and `Output="::gCommOutput6D"`, the user should use `gCommInput6D` and `gCommOutput6D` in the ST code in the INIT part of the task so that mapp6D and 6D controller communication continues.
- User should use the assembly reference defined in the assembly configuration file in ST code.
  - For example, the user should use `gAssembly6D` as the assembly reference in the ST code, as defined in the assembly configuration file `<Element ID="gAssembly6D" Type="assembly6d">`.
  - However, the user will not be able to see `<Element ID="gAssembly6D" Type="assembly6d">` in Automation Studio.
- Global variables used by the assembly mapping should be declared in a global variable file. Example:
```iec-st
VAR_GLOBAL
  gAssembly6D : McAcp6DAssemblyType;
  gCommInput6D : McAcp6DCommInputType;
  gCommOutput6D : McAcp6DCommOutputType;
END_VAR
```

##  Overview of function blocks
mapp6D is used to control ACOPOS 6D shuttles on a planar surface using magnetic levitation. Programming is done via function blocks in Structured Text.
This guide covers:

- Basic setup
- Initialization
- Shuttle control
- Motion commands
- Error handling

# General overview of function blocks

##  Execute function block

### Input of the function block
- User should pass a reference of Assembly, Shuttle, MacroShuttle, Segment, PowerSector, or Zone based on the type of function block.
- User should use `Execute` to execute the function block.
- There will be parameters in the function block to set based on function block type.
- Optional: there can be `CommandLabel` as input, especially for movement function blocks.

### Output of the function block
- User should use `Done` when function block execution is completed
- User should use `Error` when there is error in function block execution and use `ErrorID` to know error number.
- `Busy` status indicates that the function block is currently executing.
- `CommandAborted` indicates that the command was aborted before completion, which can occur due to various reasons.
- There can be other outputs based on function block type, such as information.

##  Enable function block

### Input of the function block
- User should pass a reference of Assembly, Shuttle, MacroShuttle, Segment, PowerSector, or Zone based on the type of function block.
- User should use `Enable` so that function block stays active.
- There will be parameters in the function block to set based on function block type.

### Output of the function block
- User should use `Valid` when function block is active and executing the command.
- User should use `Error` when there is error in function block execution and use `ErrorID` to know error number.
- `Busy` status indicates that the function block is currently executing.

## Division of function blocks
Function blocks are divided into different categories based on their function, as follows.
- Administrative: managing system state, initialization, configuration, or getting references of objects
- Motion: controlling shuttle movement
- Informative: reading status, information, or diagnostics from the system

# Programming for mapp6D function blocks

## System function blocks

- `MC_BR_AsmPowerOn_Acp6D`(`Execute`) to power on the system
- `MC_BR_AsmPowerOff_Acp6D`(`Execute`) to power off the system
- `MC_BR_AsmReset_Acp6D`(`Execute`) to reset the assembly state if state is `mcACP6D_ERRORSTOP`
- `MC_BR_AsmReadInfo_Acp6D`(`Enable`) to read assembly information like shuttle count, assembly state, controller state, accident shuttle count, etc.
- `MC_BR_AsmGetShuttle_Acp6D`(`Enable`) to get references and ShuttleID of shuttles on the assembly. Users should store the reference of each shuttle in a variable for later use in motion control. Similarly, there are other function blocks as shown below.
  - `MC_BR_AsmGetAccidentSh_Acp6D`(`Enable`) - to get reference and ShuttleID of accident shuttles on the assembly
  - `MC_BR_AsmGetSegment_Acp6D`(`Enable`) - to get reference of segments defined in the layout configuration file
  - `MC_BR_AsmGetPowerSec_Acp6D`(`Enable`) - to get reference of power sectors defined in the layout configuration file
  - `MC_BR_AsmGetShGroup_Acp6D`(`Enable`) - to get reference of shuttle groups created
  - `MC_BR_AsmGetZone_Acp6D`(`Enable`) - to get reference of zones created

## Basic Motion function blocks
- `MC_BR_MoveInPlane_Acp6D`(`Execute`) to move shuttle in X and Y direction
- `MC_BR_MoveShortAxis_Acp6D`(`Execute`) to move shuttle in Z, Rx, Ry, Rz direction
- `MC_BR_RotaryMotion_Acp6D`(`Execute`) to rotate shuttle in Rz direction to desired angle
- `MC_BR_RotaryMotionSpin_Acp6D`(`Execute`) to rotate shuttle in Rz direction for desired time duration and stop at desired angle
- `MC_BR_MoveArc_Acp6D`(`Execute`) to move shuttle to desired position following an arc trajectory. Users should set the center of the arc and the direction of movement in the parameters.
- `MC_BR_Move6D_Acp6D`(`Execute`) to move shuttle to desired position in all 6 axes X, Y, Z, Rx, Ry, Rz
- `MC_BR_ShStop_Acp6D`(`Execute`) to stop shuttle immediately with maximum deceleration
- `MC_BR_ShHalt_Acp6D`(`Execute`) performs a controlled stop of a shuttle and enables movements to be later resumed
- `MC_BR_ShBuffer_Acp6D`(`Execute`) to clear, hold and resume the buffered commands for a shuttle
- There are also macro function blocks that can trigger sequences of movement commands stored in the macro shuttle.
  - `MC_BR_MacroCreate_Acp6D`(`Execute`) to create a macro shuttle and store the reference in a variable for later use. Users can trigger all kinds of movement commands for the macro shuttle. This will store the movement commands in the macro shuttle but not execute them on an actual shuttle.
  - `MC_BR_MacroSave_Acp6D`(`Execute`) to save the macro shuttle movement commands.
  - `MC_BR_MacroClear_Acp6D`(`Execute`) to clear the macro shuttle movement commands.
  - `MC_BR_MacroDelete_Acp6D`(`Execute`) to delete the macro shuttle and its movement commands.
  - `MC_BR_MacroRun_Acp6D`(`Execute`) to execute the movement command stored in the macro shuttle to actual shuttle.

## Advanced Motion function blocks
- `MC_BR_MoveInPlaneAsync_Acp6D`(`Execute`) to move selected shuttles in desired X and Y position asynchronously
- `MC_BR_MoveCyclicPosition_Acp6D`(`Enable`) to move shuttle to desired cyclic position sent from PLC
- Users can form a group of multiple shuttles, then trigger a movement command to a single shuttle and let the entire group follow that master shuttle movement. For example, if Shuttle 1, 2, and 3 are in a group and a user triggers `MC_BR_MoveInPlane_Acp6D` to move Shuttle 1 to a desired position, Shuttle 2 and Shuttle 3 will follow the movement of Shuttle 1 with the same relative position.
  - `MC_BR_ShGroupCreate_Acp6D`(`Execute`) to create a shuttle group and store the reference in a variable for later use
  - `MC_BR_ShGroupAddShuttle_Acp6D`(`Execute`) to add shuttles to an existing shuttle group
  - `MC_BR_ShGroupClear_Acp6D`(`Execute`) to remove all shuttles from an existing shuttle group
  - `MC_BR_ShGroupDelete_Acp6D`(`Execute`) to delete an existing shuttle group
  - `MC_BR_ShGroupCoupleCtrl_Acp6D`(`Execute`) to control coupling and decoupling of shuttles in an existing shuttle group. For example, if Shuttle 1, 2, and 3 are in a group and the user enables coupled control for the shuttle group, when the user triggers `MC_BR_MoveInPlane_Acp6D` to move Shuttle 1 to a desired position, Shuttle 2 and Shuttle 3 will follow the movement of Shuttle 1 with the same relative position. If the user decouples the shuttle group, then when the user triggers `MC_BR_MoveInPlane_Acp6D` to move Shuttle 1 to a desired position, Shuttle 2 and Shuttle 3 will not follow the movement of Shuttle 1.
    - `MC_BR_ShGroupBuffer_Acp6D`(`Execute`) to control whether the movement command buffer for shuttles is blocked or released for shuttles that are specific to the group.
    - `MC_BR_ShGroupGetInfo_Acp6D`(`Execute`) to get information of the shuttle group like shuttle count in the group and couple state such as `mcACP6D_SH_GROUP_DECOUPLED` or `mcACP6D_SH_GROUP_COUPLED`.
- `MC_BR_ShPlanet_Acp6D`(`Execute`) to create a contiguous group in which a specific shuttle represents the center and the other shuttles move in relation to the center.

## Power Sector function blocks
- `MC_BR_PowerSecActivate_Acp6D`(`Execute`) to activate all segments in the power sector and identify shuttles in the power sector
- `MC_BR_PowerSecDeactivate_Acp6D`(`Execute`) to deactivate all segments in the power sector
- `MC_BR_PowerSecGetInfo_Acp6D`(`Execute`) to get information of the power sector such as SectorID, Name and State
- `MC_BR_PowerSecGetShuttle_Acp6D`(`Enable`) to get reference and ShuttleID of shuttles in the power sector
- `MC_BR_PowerSecRecover_Acp6D`(`Execute`) to open barrier of power sector after activating

## Zone function blocks
- `MC_BR_ZoneCreate_Acp6D`(`Execute`) to create a zone with defined area
- `MC_BR_ZonePowerOn_Acp6D`(`Execute`) to power on the zone that levitates shuttles in the zone
- `MC_BR_ZonePowerOff_Acp6D`(`Execute`) to power off the zone that drop shuttles in the zone
- `MC_BR_ZoneGetBarrierInfo_Acp6D`(`Execute`) to get information of the zone such as number of shuttles and state
- `MC_BR_ZoneOverride_Acp6D`(`Execute`) to limit speed and acceleration of shuttles in the zone to the defined value
- `MC_BR_ZoneBarrierCommand_Acp6D`(`Execute`) to open or close the barrier of the zone. For example, if a user sets `Command` to `mcACP6D_BARRIER_OPEN`, the barrier of the zone is opened and shuttles in the zone can move out of the zone freely. If a user sets `Command` to `mcACP6D_BARRIER_CLOSE`, the barrier of the zone is closed and shuttles in the zone cannot leave the zone.

## Shuttle Stereotype function blocks
- `MC_BR_ShStereotypeDefine_Acp6D`(`Execute`) to create a shuttle stereotype with defined parameters such as size, weight, center of gravity, etc.
- `MC_BR_ShStereotypeDelete_Acp6D`(`Execute`) to delete the shuttle stereotype
- `MC_BR_ShStereotypeGetInfo_Acp6D`(`Execute`) to get information of the shuttle stereotype such as size, weight, center of gravity, etc.
- `MC_BR_ShStereotypeAssign_Acp6D`(`Execute`) to assign the shuttle stereotype to shuttle

## Function blocks that affect all shuttles on the assembly
- `MC_BR_AsmHalt_Acp6D`(`Execute`) performs a controlled stop of all shuttles and enables movements to be later resumed
- `MC_BR_AsmStop_Acp6D`(`Execute`) to stop all shuttles immediately with maximum deceleration
- `MC_BR_AsmBuffer_Acp6D`(`Execute`) to clear, hold and resume the buffered commands for all shuttles on the assembly
- `MC_BR_AsmSetJerkLimit_Acp6D`(`Execute`) to set jerk limit for all shuttles on the assembly.
- `MC_BR_AsmStereotypeAssign_Acp6D`(`Execute`) to assign the shuttle stereotype to all shuttles on the assembly

## Segment function blocks
- `MC_BR_SegPowerOn_Acp6D`(`Execute`) to power on the segment to levitate shuttles on the segment
- `MC_BR_SegPowerOff_Acp6D`(`Execute`) to power off the segment to drop shuttles on the segment
- `MC_BR_SegGetInfo_Acp6D`(`Execute`) to get information of the segment such as temperature, power, position in assembly and type of segment.
- `MC_BR_SegBarrierCommand_Acp6D`(`Execute`) to open or close the barrier of the segment. For example, if a user sets `Command` to `mcACP6D_BARRIER_OPEN`, the barrier of the segment is opened and shuttles on the segment can move out of the segment freely. If a user sets `Command` to `mcACP6D_BARRIER_CLOSE`, the barrier of the segment is closed and shuttles on the segment cannot leave the segment.
- `MC_BR_SegGetBarrierInfo_Acp6D`(`Execute`) to get information of the barrier of the segment such as state of the barrier and number of shuttles

## Information function blocks for shuttle
- `MC_BR_ShReadInfo_Acp6D`(`Enable`) to read the information of the shuttle such as position, shuttleID and state
- `MC_BR_ReadCyclicPosition_Acp6D`(`Enable`) to read the position of the shuttle
- `MC_BR_ShGetInfo_Acp6D`(`Execute`) to get information of the shuttle such as position, shuttleID, state, Motion buffer, etc.
- `MC_BR_ShGetProperty_Acp6D`(`Execute`) to get shuttle type

## Miscellaneous function blocks for shuttle
- `MC_BR_ShGetPayload_Acp6D`(`Execute`) to get payload information of the shuttle such as weight
- `MC_BR_ShForceMode_Acp6D`(`Execute`) to switch shuttle between position and force mode, and setting force parameter in desired axis with desired value. For example, if user set `ControlMode` to `mcACP6D_FORCE_MODE` and set `Fx` to `10N`, the shuttle will be controlled in force mode in X direction with 10 Newton force.
- `MC_BR_ShCopyUserData_Acp6D`(`Execute`) to set user defined data to the shuttle and get the user defined data from the shuttle. For example, user can set `UserData` to `1234` and this value can be retrieved later by reading `UserData` from the shuttle. This can be used for user defined identification or classification of shuttles in the application.

# Writing ST code for mapp6D function blocks

## Execute function block in ST code

```iec-st
CASE StepCommand OF
  0:
  IF (AsmPowerOnDone = TRUE) THEN
    AcknowledgeBase := FALSE;
    IsPowerOnDone := FALSE;
    IsCmdAborted := FALSE;
    StepCommand := 1;
  ELSIF (AsmPowerOnAck = TRUE) THEN
    AcknowledgeBase := TRUE;
    IsPowerOnAck := FALSE;
    IsCmdAborted := FALSE;
    StepCommand := 1;
  END_IF;

  1: (* Power on the assembly *)
    fbAsmPowerOn.Execute := TRUE;
    fbAsmPowerOn.Zone := 0;
    fbAsmPowerOn.Option := mcACP6D_ASMPOWERON_AUTO;
    IF fbAsmPowerOn.Acknowledge AND AcknowledgeBase THEN
      fbAsmPowerOn.Execute := FALSE;
      AsmPowerOnAck := FALSE;
      IsPowerOnAck := TRUE;
      StepCommand := 0;
    ELSIF fbAsmPowerOn.Done THEN
      fbAsmPowerOn.Execute := FALSE;
      AsmPowerOnDone := FALSE;
      AsmPowerOnAck := FALSE;
      IsPowerOnAck := TRUE;
      IsPowerOnDone := TRUE;
      StepCommand := 0;
    ELSIF fbAsmPowerOn.CommandAborted THEN
      fbAsmPowerOn.Execute := FALSE;
      AsmPowerOnAck := FALSE;
      AsmPowerOnDone := FALSE;
      IsCmdAborted := TRUE;
      StepCommand := 0;
    ELSIF fbAsmPowerOn.Error THEN
      fbAsmPowerOn.Execute := FALSE;
      AsmPowerOnAck := FALSE;
      AsmPowerOnDone := FALSE;
      ErrorID := fbAsmPowerOn.ErrorID;
      (* handle error *)
    END_IF;
END_CASE;
fbAsmPowerOn(Assembly := ADR(gAssembly6D));
```

## Enable function block in ST code

Enable-type function blocks should stay active while `Enable := TRUE`, and should be evaluated cyclically with `Valid` and `Error` handling. Example:

```iec-st
fbAsmReadInfo.Enable := TRUE;
fbAsmReadInfo(Assembly := ADR(gAssembly6D));

IF fbAsmReadInfo.Valid THEN
  AssemblyState := fbAsmReadInfo.AssemblyState;
ELSIF fbAsmReadInfo.Error THEN
  ErrorID := fbAsmReadInfo.ErrorID;
END_IF;
```

# Best practices
- Execute should be reset only when `Acknowledge`, `CommandAborted` or `Error` is true. For example, if user triggers `MC_BR_AsmPowerOn_Acp6D` to power on the assembly, user must wait until `Acknowledge` is true to reset `Execute`. 
  - In ideal case, user should wait until `Done` is true to reset `Execute`.
  - If there is error in execution of function block, user should wait until `Error` is true to reset `Execute`.
  - If command is aborted in execution of function block, user should wait until `CommandAborted` is true to reset `Execute`.
- Setting `Execute` to false before `Acknowledge`, `Done`, `CommandAborted`, or `Error` become TRUE may cause unexpected behavior and should be avoided. When `Acknowledge` is true, it means that the command has been received and acknowledged by the 6D controller, but the action has not yet been completed.
- User should handle `Error` and `CommandAborted` status for each function block execution to make the system more robust. For example, if `Error` is true, user can read `ErrorID` to know the error and perform error handling such as retrying the command or alerting the operator. If `CommandAborted` is true, user can decide whether to retry the command or take other actions based on the application logic.
- For motion function blocks, users should consider using `Acknowledge` and `Done` status to trigger the next command to make sure commands are executed in sequence. For example, if a user wants to move a shuttle to position A and then to position B, they can trigger `MC_BR_MoveInPlane_Acp6D` to move the shuttle to position A, then wait until `Acknowledge` is true before triggering `MC_BR_MoveInPlane_Acp6D` to move the shuttle to position B. This ensures the shuttle moves to position A first and then to position B, enabling time efficiency and smooth movement of the shuttle.
- User should read shuttle references once the assembly is powered on. For example, user can trigger `MC_BR_AsmGetShuttle_Acp6D` to get shuttle references and store them in variables after powering on the assembly. This helps ensure shuttle references are valid and up to date.
- Users should perform time-based polling for execute function blocks instead of event-based polling. For example, users can use a timer to check the status of function block execution every 1000 ms instead of checking status in every cycle. This will reduce blocking of important function block execution and make the system more efficient. For example, `MC_BR_SegGetInfo_Acp6D`, `MC_BR_ShGetInfo_Acp6D`, `MC_BR_AsmGetInfo_Acp6D`, and `MC_BR_PowerSecGetInfo_Acp6D` can be polled at a certain time interval to get the latest system information.
- If users would like to know the error in the 6D controller, they can use `MC_BR_AsmGetInfo_Acp6D` to read `ErrorCode` when assembly state is `mcACP6D_ERRORSTOP`. This will help users identify the error in the 6D controller and perform error handling.