---
description: Guidelines for B&R Automation Studio hardware configuration (Io mapping, Hardware topology).
applyTo: '**/*.hw,**/*.iom'
---

# B&R Automation Studio Hardware Configuration Guidelines

You are an expert in B&R Automation Studio (6.0+) hardware configuration.
Follow these guidelines when working with `.hw` and `.iom` files in the `Physical/` view of an Automation Studio project.

## General Instructions

- The `Physical/` folder mirrors the hardware topology. One folder per configuration (`Config1/`), one subfolder per CPU module.
- Never edit generated outputs in `Binaries/`, `Temp/`, or `Diagnosis/`.
- Whenever you add, remove, or rename a file or folder, update the corresponding `Package.pkg`.
- Always refer to the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) or `as-physical-hw-api` skill for hardware module documentation, parameter ranges, and valid XML schema.

## Folder Structure

```text
Physical/
└── Config1/
    ├── Hardware.hw            # Hardware topology (module assignments) <relevant for these instructions>
    ├── Hardware.hwl           # Hardware layout (slot assignments) <DO NOT EDIT>
    └── X20CP1686X/            # CPU module folder
        ├── Cpu.per            # CPU-specific parameters
        ├── Cpu.sw             # Software configuration (task scheduling)
        ├── IoMap.iom          # I/O channel mapping <relevant for these instructions>
        ├── PvMap.vvm          # Process Variable mapping between tasks
        ├── AccessAndSecurity/ # User, Role, OPC UA security
        ├── Connectivity/      # Network and fieldbus settings
        ├── mappView/          # mappView visualization configuration
        └── ...                # Technology package folders (mappMotion, etc.)
```

## Hardware Topology (`Hardware.hw`)

`Hardware.hw` models the complete physical hardware layout: what CPU is used, which modules exist, how they are wired, which network addresses they hold, and how they are parametrized. Every module is a top-level `<Module>` element; its physical connections, bus links, and configuration parameters are child nodes inside it.

**CRITICAL: Never invent connector names, target module types, or parameter IDs. Always use the `as-physical-hw-api` skill to retrieve the factual data from the AS installation before writing into the Hardware.hw file.**

### Overview — How Modules Relate

B&R hardware is assembled from different physical modules that must all appear as separate `<Module>` entries and be wired explicitly:

- **Fieldbus modules** (drives, bus controllers) communicate over a cabled network (e.g. POWERLINK). They require a `NodeNumber` attribute and `<Connection>` nodes with a `<Cable>` child.
- **X20 I/O slices** are a triple assembly: a **Bus Module** (backplane carrier), an **Electronics Module** (functional I/O), and a **Terminal Block** (field wiring). All three must be present and connected to each other.
- **Decentralized I/O modules** attach a Bus Controller to a base plate; the resulting X2X backplane is then an isolated segment wired from the Bus Controller.
- **Motor modules** are standalone entries connected to a specific drive channel connector (`MT1`, `MT2`, …).
- **Other modules** may have their own specific wiring and parameterization rules.

Connections are written on the **source** module pointing at the **target** module instance name:

```xml
<Module Name="[InstanceName]" Type="[ModuleType]" Version="[Version]">
  <Connection Connector="[SourceConnector]" TargetModule="[TargetInstanceName]" TargetConnector="[TargetConnector]" />
</Module>
```

Cabled fieldbus connections carry an additional `<Cable>` child element with type and length:

```xml
<Connection Connector="[SourceConnector]" TargetModule="[TargetInstanceName]" TargetConnector="[TargetConnector]">
  <Cable Type="[CableType]" Length="[LengthInMeters]" Version="[CableVersion]" />
</Connection>
```

### Node Numbers

Every module on a cabled fieldbus network (POWERLINK, CAN, …) needs a unique `NodeNumber`:

```xml
<Module Name="[InstanceName]" Type="[ModuleType]" Version="[Version]" NodeNumber="[N]">
```

- **Node numbers must be unique within each network segment.** Before assigning a number, scan all existing `<Module>` entries in `Hardware.hw` for already-used values.
- Modules on a local X2X backplane or mounted as terminal blocks do not carry a `NodeNumber`.
- Modules on a PLK network that do not have a `NodeNumber` attribute defined, will get one automatically assigned based on Connection order. You can verify using the `as-compile` skill that the assigned numbers are unique and do not conflict with other nodes on the same network segment.

### X2X Backplane Chaining

X2X is a serial backplane bus. Modules are chained strictly left-to-right:

- The first slice in a rack connects its `X2X1` input to the rack root interface.
- Each subsequent slice connects its `X2X1` input to the `X2X2` output of the left neighbor.

**Local rack** (slices attached directly to the CPU):

```xml
<!-- First slice: root connects to the CPU local bus interface -->
<Module Name="[FirstBusModule]" Type="[BusModuleType]" Version="[Version]">
  <Connection Connector="X2X1" TargetModule="[CpuInstance]" TargetConnector="IF6" />
</Module>

<!-- Each subsequent slice connects to the previous one -->
<Module Name="[NextBusModule]" Type="[BusModuleType]" Version="[Version]">
  <Connection Connector="X2X1" TargetModule="[PreviousBusModuleInstance]" TargetConnector="X2X2" />
</Module>
```

**Remote rack** (slices attached to a Bus Controller on a base plate):

```xml
<!-- Bus Controller mounts on the base plate slide connector -->
<Module Name="[BusControllerInstance]" Type="[BusControllerType]" Version="[Version]" NodeNumber="[N]">
  <Connection Connector="SL" TargetModule="[BasePlateInstance]" TargetConnector="SL1" />
</Module>

<!-- First remote slice: root connects to the base plate bus interface -->
<Module Name="[FirstRemoteBusModule]" Type="[BusModuleType]" Version="[Version]">
  <Connection Connector="X2X1" TargetModule="[BasePlateInstance]" TargetConnector="IF1" />
</Module>
```

**Never cross-wire local and remote chains.** Each rack segment is an isolated electrical network; a local bus module must never link to a remote bus module and vice versa.

### X20 Triple-Slice Assembly

An X20 I/O slice always consists of three entries in `Hardware.hw`: the Bus Module, the Electronics Module, and the Terminal Block. The Electronics Module declares both its backplane connection (`SL` → `SL1` on the Bus Module) and its terminal connection (`SS1` → `SS` on the Terminal Block):

```xml
<Module Name="[BusModuleInstance]" Type="[BusModuleType]" Version="[Version]">
  <Connection Connector="X2X1" TargetModule="[LeftNeighborOrRoot]" TargetConnector="X2X2" />
</Module>

<Module Name="[ElectronicsModuleInstance]" Type="[ElectronicsModuleType]" Version="[Version]">
  <Connection Connector="SL"  TargetModule="[BusModuleInstance]"      TargetConnector="SL1" />
  <Connection Connector="SS1" TargetModule="[TerminalBlockInstance]"   TargetConnector="SS"  />
</Module>

<Module Name="[TerminalBlockInstance]" Type="[TerminalBlockType]" Version="[Version]" />
```

**Use `GetPossibleHWConnections` (see skill `as-physical-hw-api`) on the electronics module type** to retrieve which Bus Module type and Terminal Block type it is compatible with. Different slices require different terminal block widths or bus module variants — do not assume `X20BM11` + `X20TB12` are always correct.

### Drives and Motors

Drives participate in a cabled fieldbus network (typically POWERLINK) and expose multiple independent motor channels. Each motor is a separate module entry wired to one drive channel.

**Fieldbus daisy-chaining:** Drives connect to the CPU or bus controller via `PLK1` and forward the bus to the next node via `PLK2`. Every drive in the chain requires a unique `NodeNumber`:

```xml
<!-- First drive connects to the CPU POWERLINK interface -->
<Module Name="[DriveInstance]" Type="[DriveType]" Version="[Version]" NodeNumber="[N]">
  <Connection Connector="PLK1" TargetModule="[CpuOrPreviousNode]" TargetConnector="[PLKInterface]">
    <Cable Type="PowerlinkCable" Length="[LengthInMeters]" Version="[CableVersion]" />
  </Connection>
  <!-- Forward the bus to the next POWERLINK node if one exists -->
  <Connection Connector="PLK2" TargetModule="[NextNodeInstance]" TargetConnector="PLK1">
    <Cable Type="PowerlinkCable" Length="[LengthInMeters]" Version="[CableVersion]" />
  </Connection>
</Module>
```

**Motor wiring:** Each physical motor is a standalone `<Module>` entry. Its `MT` connector is wired to the specific channel connector (`MT1`, `MT2`, …) on the drive. Use `GetPossibleHWConnections` on the drive type to confirm which channel connectors are available:

```xml
<Module Name="[MotorInstance]" Type="[MotorType]" Version="[Version]">
  <Connection Connector="MT" TargetModule="[DriveInstance]" TargetConnector="[MTn]" />
  <!-- Motor parameters require Location path — use GetModuleProperties to retrieve valid IDs and paths -->
  <Parameter ID="[MotorParamID]" Location="[M1/MotorParameters/...]" Value="[Value]" />
</Module>
```

**Axis assignment:** Each drive channel that controls a real axis must reference the corresponding axis object by name.

Rules for Axis assignment via `DriveConfiguration/Channel[n]/RealAxis`:
- `DriveConfiguration/Channel[1]/RealAxis` -> `gAxis_*`
- `DriveConfiguration/Channel[2]/RealAxis` -> `gAxis_*`

```xml
<Parameter ID="AxisReference" Location="DriveConfiguration/Channel[n]/RealAxis" Value="[AxisObjectName]" />
```

**Channel-level motion parameters** (encoder mode, gearbox, homing, digital inputs) live inside the `DriveConfiguration` namespace at deeper paths. Use `GetModuleProperties` with a `-ParentElement` drill-down to discover which parameters exist and what values are permitted before authoring them.

**Library prerequisite:** When axes are configured, ensure the required motion libraries (`McBase`, `MpBase`, `McAxis`, `McAcpAx`) are registered in `Cpu.sw` before building.

### Module Parametrization

Modules expose configurable properties as `<Parameter>` child elements. Parameter IDs, allowed values, and the `Location` attribute for technology-package namespaces are all module-specific metadata — never author them from memory.

**Use `GetModuleProperties` (skill `as-physical-hw-api`)** to retrieve the permitted parameters, their valid values, and whether a `Location` path is required:

```xml
<!-- Simple parameter, no namespace location -->
<Parameter ID="[ParamID]" Value="[Value]" />

<!-- Technology-package parameter with a namespace location (e.g. drive or motor config) -->
<Parameter ID="[ParamID]" Location="[NamespacePath]" Value="[Value]" />
```

Rules for parameter values:
- **List values:** the `Value` must be one of the allowed IDs returned by `GetModuleProperties` (`valueList[].id`), not a human-readable label.
- **Reference values:** the `Value` must be the exact name of an existing project object (e.g. an axis variable name).
- **Numeric values:** respect the range and unit documented in the parameter metadata.

### Finding and Validating Modules

**Never add a module to `Hardware.hw` without first verifying it exists in the AS installation and has the expected type and classification.** Use the `as-physical-hw-api` skill for all discovery and validation steps:

1. **Identify the module** — if the exact type string is unknown, use `SearchHWModule` (product number search - e.g. `X20DO8338` or `8BVI0014HWS0`) or `SearchByClassification` (keyword search - e.g. `X20 & Digital | Acopos6D`) to find candidates.
2. **Verify the module** — run `GetHWModuleInfo` on the confirmed type to read its description and classification. If the classification contains `"CPU"`, the module is a CPU module and **must never be added** with `AddHWModule`; adding a second CPU corrupts the hardware configuration.
3. **Discover connections** — run `GetPossibleHWConnections` on the module type to obtain the exact connector names, compatible connection types, and `autoConnect` routing recipes before writing any `<Connection>` nodes.
4. **Discover parameters** — run `GetModuleProperties` on the module type before writing any `<Parameter>` nodes.
5. **Add the module** — once the above steps confirm the type, version, connections, and parameters, use `AddHWModule` to register the module in `Hardware.hw`.

### Verification Information

#### Connector and Parameter Accuracy

- **Never guess connector names.** Always run `GetPossibleHWConnections` on the module type before writing any `<Connection>`. The wrong connector name (e.g. `SL` vs `SL1`, `IF3` vs `IF6`) will silently mismatch and the module will appear unwired in AS.
- **Never guess parameter IDs or Location paths.** Always run `GetModuleProperties` before writing any `<Parameter>`. An incorrect `ID` or `Location` is silently ignored by AS and the setting will have no effect.

#### Completeness Checks

Before finishing any topology change confirm:

- Every fieldbus module that needs a node address has a unique `NodeNumber`.
- Every X20 I/O slice has all three entries (Bus Module, Electronics Module, Terminal Block) present and wired to each other.
- Every motor is connected to exactly one drive channel, and that channel is not shared.
- Every axis referenced in `DriveConfiguration/Channel[n]/RealAxis` exists as a `.axis` file registered in `Cpu.sw`.
- Reference values (e.g. axis names) point to objects that actually exist in the project.
- Every `Version` attribute matches one of the installed versions of the modules in the AS installation. Use `GetHWModuleInfo` to confirm.
- Every `Type` attribute matches the exact type string of a module in the AS installation. Use `GetHWModuleInfo` to confirm.
- Every `Module` instance name is unique within the `Hardware.hw` file of the current Configuration.

## I/O Mapping (`IoMap.iom`)

- Connects hardware I/O channels to IEC 61131-3 variables. The `.iom` file lives in the CPU folder: `Physical/<Config>/<CPU>/IoMap.iom`. It is processed at build time; changes take effect after a transfer.
- Multiple `.iom` files can exist in a project, all of them are equally valid! Create one `.iom` file per high-level functional group (e.g. `IoConv.iom`, `IoLift.iom`) to keep the mapping organized.
 - `.iom` file names have a limit of <= 10 characters and have to be unique within the CPU folder. They are not allowed to share the same name as any `.vvm` file, task name (`<Task Name="ThisName">`), nor any mapp configuration file name. Therefore always use an `Io` prefix for `.iom` files!

### How to Discover Available Channels

Before writing any mapping, retrieve the available channels for a module using the `as-physical-hw-api` skill `GetIOMappingConfig`!

Each returned channel has:
- `channelName` — the exact identifier to use in the mapping line (e.g. `DigitalInput01`, `ModuleOk`)
- `dataType` — IEC data type (`BOOL`, `UINT`, `UDINT`, …)
- `direction` — `In` (hardware → PLC) or `Out` (PLC → hardware)
- `filter` — present on some channels; only the active channel variant is valid depending on module configuration (e.g. standard vs CANIO). Use the channel whose filter condition matches the module's `FunctionModel` parameter.

**Example output for `X20DI6553`:**
```
  ModuleOk       BOOL   In    Module status (1 = module present)
  DigitalInput01 BOOL   In    100 to 120 VAC, 47 to 63 Hz, <30 ms switching delay
  DigitalInput02 BOOL   In    100 to 120 VAC, 47 to 63 Hz, <30 ms switching delay
  ...
```

**Example output for `X20DO8322`:**
```
  ModuleOk          BOOL   In    Module status (1 = module present)
  DigitalOutput01   BOOL   Out   24 VDC / 0.5 A, source
  DigitalOutput02   BOOL   Out   24 VDC / 0.5 A, source
  StatusDigitalOutput01 BOOL In  Status digital output 01 (0 = OK)
  ...
```

### Rules

- File encoding: UTF-8 **without** BOM.
- Comments are only allowed using `(*…*)` at the end of a mapping line. Standalone comment lines are **PROHIBITED**.
- Syntax: `<variable> AT %<direction><type>."<ModuleInstanceName>".<ChannelName>;`
- **Module instance name** is the `Name` attribute from `Hardware.hw`, not the module type. Enclose it in double quotes.
- **Channel name** must be the exact `channelName` from `GetIOMappingConfig`. Never guess or invent channel names.
- **Address prefix** encodes direction and data type:
  - `%IX` / `%QX` — input / output **bit** (`BOOL`)
  - `%IB` / `%QB` — input / output **byte** (`USINT`)
  - `%IW` / `%QW` — input / output **word** (`UINT`)
  - `%ID` / `%QD` — input / output **double-word** (`UDINT`)
  - Use `%I…` for channels with `direction = In`; use `%Q…` for channels with `direction = Out`.
- **Variable paths** follow the following rules:
  - Task-local variable: `::<TaskName>:<variableName>` or `::<TaskName>:<struct>.<member>`
  - Global variable (no task): `::<VarName>` or `::<struct>.<member>`
- Only map channels that the task or global variable actually needs. Unmapped channels are ignored at runtime.
- Mapping the same channel to more than one variable is allowed for read-only (`In`) channels; never map an `Out` channel to multiple variables.
- **Filtered channels:** when a module has both a standard and a `_CANIO` variant of the same channel (e.g. `DigitalInput01` and `DigitalInput01_CANIO`), use only the variant that matches the module's `FunctionModel` parameter in `Hardware.hw`.
- *(optional)* **Inverted channels:** mappings can have an optional `INVERSE` parameter that flips the logic of a channel. Adding INVERSE to a mapping line will invert the value of the channel in the PLC program (In) or physical output (Out).
- *(optional)* **Task Class:** by adding a task class prefix to the variable path, you can adjust the cyclic timing of the mapping. For example, `Cyclic#8.::<TaskName>:<variableName>` will map the channel in the `Cyclic#8` phase instead of the default Cyclic#n that the associated Task (`<TaskName>`) is deployed in.

### Syntax

**Mandatory parameters:**
```
VAR_CONFIG
    <variable> AT %<dir><type>."<ModuleInstanceName>".<ChannelName>;
END_VAR
```

**With optional parameters:**
```
VAR_CONFIG
    Cyclic#n.<variable> AT %<dir><type>."<ModuleInstanceName>".<ChannelName> INVERSE; (*optional comment*)
END_VAR
```
### Example

Hardware.hw has an `X20DI6553` instance named `X20DI6553Inst` and an `X20DO8322` instance named `X20DO8322Inst`.

Run `GetIOMappingConfig` on each module to get the available channels, then map them:

```
VAR_CONFIG
    ::MyTask:diSensor1      AT %IX."X20DI6553Inst".DigitalInput01; (*Conveyor sensor 1*)
    ::MyTask:diSensor2      AT %IX."X20DI6553Inst".DigitalInput02; (*Conveyor sensor 2*)
    ::MyTask:diModuleOk     AT %IX."X20DI6553Inst".ModuleOk;       (*Module health check*)
    ::MyTask:doActuator1    AT %QX."X20DO8322Inst".DigitalOutput01; (*Lift up valve*)
    ::MyTask:doActuator2    AT %QX."X20DO8322Inst".DigitalOutput04; (*Position control*)
    ::MyTask:stOutput1Ok    AT %IX."X20DO8322Inst".StatusDigitalOutput01; (*Output 1 short-circuit status*)
END_VAR
```
