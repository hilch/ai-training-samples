---
description: Guidelines for B&R Automation Studio software configuration (Cpu.sw, PvMap, Access & Security, connectivity, technology packages).
applyTo: '**/*.sw,**/*.vvm,**/*.role,**/*.user,**/*.per,Physical/**'
---

# B&R Automation Studio Software Configuration Guidelines

You are an expert in B&R Automation Studio (6.0+) software configuration.
Follow these guidelines when working with files in the `Physical/` view of an Automation Studio project.

## General Instructions

- The `Physical/` folder mirrors the hardware topology. One folder per configuration (`Config1/`), one subfolder per CPU module.
- Never edit generated outputs in `Binaries/`, `Temp/`, or `Diagnosis/`.
- Whenever you add, remove, or rename a file or folder, update the corresponding `Package.pkg`.
- Always refer to the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) if information is missing.

## Folder Structure

```text
Physical/
└── Config1/
    ├── Hardware.hw            # Hardware topology (module assignments)
    ├── Hardware.hwl           # Hardware layout (slot assignments)
    └── X20CP1686X/            # CPU module folder
        ├── Cpu.per            # CPU-specific parameters
        ├── Cpu.sw             # Software configuration (task scheduling)
        ├── IoMap.iom          # I/O channel mapping
        ├── PvMap.vvm          # Process Variable mapping between tasks
        ├── AccessAndSecurity/ # User, Role, OPC UA security
        ├── Connectivity/      # Network and fieldbus settings
        ├── mappView/          # mappView visualization configuration
        └── ...                # Technology package folders (mappMotion, etc.)
```

## CPU Software Configuration (`Cpu.sw`)

The `Cpu.sw` file defines which tasks run on the CPU and in which cyclic class.

### Task Classes
- Tasks are grouped into cyclic classes (e.g. `Cyclic#1` = 2ms, `Cyclic#4` = 10ms, etc.).
- Choose the class based on required response time. HMI and non-time-critical tasks belong in slower classes.
- `UserROM` memory is standard for application tasks. `UserRAM` is used for tasks requiring faster init.

### Adding a Task
Add a `<Task>` entry inside the appropriate `<TaskClass>` element:

```xml
<TaskClass Name="Cyclic#4">
  <Task Name="MyTask"
        Source="PackageName.ProgramName.prg"
        Memory="UserROM"
        Language="IEC"
        Debugging="true" />
</TaskClass>
```

- `Name`: display name in AS. Limit to <= 10 characters.
- `Source`: path in the form `<LogicalPackage>.<ProgramPackage>.prg`.
- `Language`: `IEC` for IEC 61131-3 tasks.
- `Debugging`: set `true` during development; consider `false` for production builds.

## Process Variable Mapping (`PvMap.vvm`)

Cyclically map variables between tasks without shared global variables. The `.vvm` file lives in the CPU folder: `Physical/<Config>/<CPU>/*.vvm`. Mapping is resolved at the end of the Cyclic#N phase, so mapped variables are available in the next cycle.

### Rules
- File encoding: UTF-8 **without** BOM.
- **Comments are not allowed** in `.vvm` files.
- Syntax: `<source> AT %Q.<destination>;` for both directions.
- Variable paths: `::<TaskName>:<variableName>` or `::<TaskName>:<struct>.<member>`.
- Global variables (no task): `::<VarName>`.

### Example

```
VAR_CONFIG
	::SimInputs.diLiftPositionInfeed1 AT %Q.::BoxLift:diLiftPositionInfeed1;
	::SimInputs.diLiftPositionInfeed2 AT %Q.::BoxLift:diLiftPositionInfeed2;
	::BoxLift:doLiftUp                AT %Q.::SimOutputs.doLiftUp;
	::BoxLift:doLiftDown              AT %Q.::SimOutputs.doLiftDown;
END_VAR
```

## Technology Packages

Each technology package installed on a CPU lives in its own named subfolder directly under the CPU folder (e.g., `mappMotion/`, `mappServices/`, `mapp6D/`, `mappCockpit/`, `mappVision/`). The folder name matches the technology.  
When working with technology packages, use the `as-tp-config-api` skill to list supported file endings for a technology and to validate allowed config file XML elements.  
These guidelines **DO NOT APPLY TO `mappView/`**!

### Structure Rules
- Every technology folder and every nested subfolder inside it must contain a `Package.pkg`.
- Technology config files are XML with technology-specific extensions (`.axis`, `.axesgroup`, `.axesgroupfeature`, `.mechanicalsystem`, `.assembly6d`, `.layout6d`, `.mpcomgroup`, `.visionapplication`, …).
- All share the same header:
  ```xml
  <?xml version="1.0" encoding="utf-8"?>
  <?AutomationStudio FileVersion="4.9"?>
  ```

### Naming Rules
- **File names** (the stem before the extension) are limited to **10 characters** and need to be unique within a configuration (this includes mapp technology package files, `.iom` files, `.vvm` files and deployed task names — from `<Task Name="MyTask" />` — in `Cpu.sw`).
- The `ID` attribute of every `<Element>` tag is the **mapp link name** — this is what application code assigns to an `MpLink` variable and what other config files reference by name. It is limited to **32 characters**.
- mapp link names must be unique across all technology configs on the same CPU — the mapp runtime resolves them globally by name.
- Cross-references between technology objects (e.g., an axis referenced by an axes group, a component group linking sub-components) must use the exact `ID` strings defined in the source config file.

### Editing Guidelines
- Automation Studio's graphical editors produce the correct XML structure. Prefer targeted hand-edits (changing a property value, adding a single element) over rewriting whole files from scratch.
- Never change `SubType`, `PackageType`, or the XML namespace on an existing `Package.pkg` in the root of a technology folder — Automation Studio uses these to associate the folder with the correct technology.
- Each technology has its own instruction file with schema details. Consult those instructions and the B&R Help Server (`mcp_b_r_automatio_*` tools) before editing unfamiliar config files.

## Access & Security

Files are located under `Physical/<Config>/<CPU>/AccessAndSecurity/UserRoleSystem/`.

### Roles (`Role.role`)

Defines roles that can be assigned to users and used for role based access in various systems. Add a `<Element ID="RoleName" Type="Role" />` element for each role:

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Configuration>
  <Element ID="Admin" Type="Role" />
  <Element ID="Operator" Type="Role" />
  <Element ID="Service" Type="Role" />
</Configuration>
```

- Role names are case-sensitive and referenced exactly in `User.user` and mappView role restrictions. They are always of `Type="Role"`.
- **CRITICAL**: DO NOT EDIT the existing `BRRole.brrole` file! It contains the built-in system roles and must not be modified. Always create your roles in `Role.role` and then assign them to a User in the `User.user` file.

### Users (`User.user`)

Defines user accounts.

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Configuration>
  <Element ID="admin" Type="User">
    <Property ID="Password" Value="$argon2id$v=19$m=512,t=70,p=1$..." />
    <Group ID="Roles"> <!-- User has one role -->
      <Property ID="Role[1]" Value="Admin" />
    </Group>
  </Element>
  <Element ID="operator" Type="User">
    <Property ID="Password" Value="" />
    <Group ID="Roles"> <!-- User has multiple roles -->
      <Property ID="Role[1]" Value="Admin" />
      <Property ID="Role[2]" Value="Service" />
      <Property ID="Role[3]" Value="Operator" />
    </Group>
  </Element>
  <Element ID="elevatedUser" Type="User">
    <Property ID="Password" Value="" />
    <Group ID="Roles"> <!-- User has special system role -->
      <Property ID="Role[1]" Value="BR_Engineer" />
    </Group>
  </Element>
</Configuration>
```

- **Password:** When generating a user leave the password empty `<Property ID="Password" Value="" />` and tell the user in the output to set the password manually in Automation Studio for security reasons.
- A user can have multiple roles
- **IMPORTANT:** Add `BR_Engineer` role for users with development and admin roles. **WARNING:** `BR_Engineer` role has full access to the Opc UA server. Only assign to trusted users. **CRITICAL:** Do not generate a role with the name `BR_Engineer` — it already exists as a built-in role with special permissions.
