---
description: Guidelines for B&R Automation Studio Connectivity and OPC UA configuration (Connectivity, OPC UA, OpcUaCs).
applyTo: '**/*.uad,**/*.uadcfg'
---

# B&R Automation Studio Connectivity & OPC UA Configuration Guidelines

You are an expert in B&R Automation Studio (6.0+) connectivity and OPC UA configuration engineering.
Follow these guidelines when working with files in the `Physical/<Config>/<CPU>/Connectivity/` folder with OPC UA configuration of an Automation Studio project.

## General Instructions

- The `Physical/<Config>/<CPU>/Connectivity/` folder mirrors the Task configuration. One file per task and one for all Global variables.
- Whenever you add, remove, or rename a file or folder, update the corresponding `Package.pkg`.
- All OPC UA configuration files must be placed under `Physical/<Config>/<CPU>/Connectivity/OpcUaCs/` and registered in the corresponding `Package.pkg` with `SubType="OpcUaCs"`.
- Always refer to the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) for hardware module documentation, parameter ranges, and valid XML schema.


## Connectivity / OpcUaCs

OPC UA configuration lives in `Physical/<Config>/<CPU>/Connectivity/OpcUaCs/`.

### Folder Contents

```text
OpcUaCs/
├── Package.pkg        # Lists all files (SubType="OpcUaCs")
├── UaCsConfig.uacfg   # OPC UA server settings (port, security policies)
├── UaDvConfig.uadcfg  # OPC UA device configuration
└── MyVars.uad         # Variable exposure declaration (one per variable group)
```

### `Package.pkg` for OpcUaCs

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package SubType="OpcUaCs" PackageType="OpcUaCs"
  xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
    <Object Type="File">UaCsConfig.uacfg</Object>
    <Object Type="File">UaDvConfig.uadcfg</Object>
    <Object Type="File">MyVars.uad</Object>
  </Objects>
</Package>
```

### Opc UA Device Configuration (`UaDvConfig.uadcfg`)

Roles and their default permissions are defined in the `UaDvConfig.uadcfg` file. This is where you specify which roles have access to the OPC UA server and their default permissions (e.g. read/write, full access, no access). The ACL Aliases defined in `.uad` files reference these roles.

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Configuration>
  <Element ID="DefaultViewConfiguration" Type="uadcfg">
    <Group ID="InformationModels">
      <Property ID="ComplexTypeFacet" Value="0" />
      <Property ID="ExportTypeInformation" Value="0" />
      <Property ID="DedicatedTypeDefinitionsForStructures" Value="0" />
    </Group>
    <Group ID="DefaultRolePermissions">
      <Group ID="Role [1]">
        <Property ID="Name" Value="Admin" />
        <Group ID="RolePermissions">
          <Property ID="PermissionBrowse" Value="1" /> <!-- Enabled -->
          <Property ID="PermissionRead" Value="1" /> <!-- Enabled -->
          <Property ID="PermissionWrite" Value="1" /> <!-- Enabled -->
          <Property ID="PermissionCall" Value="1" /> <!-- Enabled -->
          <Property ID="PermissionReadRolePermissions" Value="0" /> <!-- Disabled -->
          <Property ID="PermissionWriteRolePermissions" Value="0" /> <!-- Disabled -->
          <Property ID="PermissionWriteAttribute" Value="1" /> <!-- Enabled -->
          <Property ID="PermissionReadHistory" Value="1" /> <!-- Enabled -->
        </Group>
      </Group>
      <Group ID="Role [n]">
        <Property ID="Name" Value="Operator" />
        <Group ID="RolePermissions">
          <!-- Set permissions for Operator role -->
        </Group>
      </Group>
    </Group>
  </Element>
</Configuration>
```

### OPC UA Variable Declaration (`.uad`)

A `.uad` file exposes PLC variables to OPC UA clients. Each file contains:
1. **ACL Aliases** — reusable access level definitions (role + permission bitmask).
2. **Module** — groups variables under a named module (use `<Default>` for the default module).
3. **Variables** — the variable tree, mirroring the PLC variable/struct hierarchy.

#### File skeleton

Variables are grouped by their origin:
- **Task variables** (declared inside a task's `.var`): placed under `<Task Name="TaskName">`.
- **Global variables** (declared in `Global.var` outside any task): placed under `<GlobalVariables Name="Global Variables">`.

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="6.0"?>
<OpcUaSource FileVersion="10">
  <Aliases>
    <ACLAliases>
      <!-- Define reusable access levels here -->
    </ACLAliases>
    <EURangeViolationAliases />
    <EURangesAliases />
    <EngineeringUnitAliases />
  </Aliases>
  <Module Name="&lt;Default&gt;">
    <!-- Task-local variables -->
    <Task Name="MyTask">
      <Variable Name="MyStructVar" />
        <Variable Name="NormalVar" />
        <Variable Name="ArrayVar" RecursiveEnable="2">
          <Variable AclId="3" AclInherit="2" Name="ArrayVar[1]" /> <!-- Special permissions -->
          <Variable AclInherit="1" Name="ArrayVar[4]" /> <!-- Read-only-->
        </Variable>
      </Variable>
      <!-- Variable nodes go here -->
    </Task>
    <!-- Global variables (if needed) -->
    <GlobalVariables Name="Global Variables">
      <!-- Variable nodes go here -->
    </GlobalVariables>
  </Module>
</OpcUaSource>
```

### ACL Aliases — Permission Bitmasks

Permissions follow OPC 10000-3 Table 36 (`PermissionType`). The relevant bits are:

| Bit | Hex    | Meaning                  |
|-----|--------|--------------------------|
| 0   | 0x0001 | Browse                   |
| 1   | 0x0002 | ReadRolePermissions      |
| 2   | 0x0004 | WriteAttribute           |
| 3   | 0x0008 | WriteRolePermissions     |
| 5   | 0x0020 | Read                     |
| 6   | 0x0040 | Write                    |
| 7   | 0x0080 | ReadHistory              |
| 12  | 0x1000 | Call                     |

#### Standard access level bitmasks

| Level name  | Bits enabled                                | Hex value |
|-------------|---------------------------------------------|-----------|
| Full        | Browse+ReadRP+WriteAttr+WriteRP+Read+Write+ReadHist+Call | `0x10EF`  |
| Read/Write  | Browse+ReadRP+WriteAttr+Read+Write+ReadHist+Call | `0x10E7`  |
| Read-only   | Browse+ReadRP+Read+ReadHistory              | `0x00A3`  |
| Write-only  | Browse+ReadRP+WriteAttr+Write+Call          | `0x1047`  |
| No access   | (none)                                      | `0x0000`  |

#### ACL Alias XML

Each `<ACL>` defines a reusable access level combination (referenced on nodes via `AclId`), one `<ACE>` per role. Only Roles in `DefaultRolePermissions` of the `*.uadcfg` file can be used. Define default inherited permissions there.

```xml
<ACLAliases>
  <!-- Id=1: Full read/write for both roles -->
  <ACL Id="1">
    <ACE RoleName="Admin"    Access="0x10EF" />
    <ACE RoleName="Operator" Access="0x10E7" />
  </ACL>
  <!-- Id=2: Admin full, Operator no access -->
  <ACL Id="2">
    <ACE RoleName="Admin"    Access="0x10EF" />
    <ACE RoleName="Operator" Access="0x0000" />
  </ACL>
  <!-- Id=3: Admin full, Operator read-only -->
  <ACL Id="3">
    <ACE RoleName="Admin"    Access="0x10EF" />
    <ACE RoleName="Operator" Access="0x00A3" />
  </ACL>
  <!-- Id=4: Admin full, Operator write-only -->
  <ACL Id="4">
    <ACE RoleName="Admin"    Access="0x10EF" />
    <ACE RoleName="Operator" Access="0x1047" />
  </ACL>
</ACLAliases>
```

### Variable Nodes — `AclInherit` and `AclId`

Every `<Variable>` leaf controls its OPC UA access via these attributes:

| `AclInherit` | `AclId`    | Meaning |
|---|---|---|
| (none)       | (none)     | **Default server access** — fully accessible based on server config per role. |
| `"1"`        | (none)     | Inherit from parent container |
| `"2"`        | `"1"`      | Use ACL alias Id=1 — full read/write for all roles. |
| `"2"`        | `"2"`      | Use ACL alias Id=2 — e.g. Admin-only or restricted access. |
| `"2"`        | `"3"`      | Use ACL alias Id=3 — e.g. Operator read-only. |
| `"2"`        | `"4"`      | Use ACL alias Id=4 — e.g. Operator write-only. |

**Key rule:** Omitting both `AclInherit` and `AclId` gives the variable full default access. Only add attributes when you need to restrict or differentiate access per role.

#### `RecursiveEnable`

Use `RecursiveEnable="2"` on an array container or a container whose children all share the same default access, so AS applies the ACL recursively without requiring it on every child.

#### `ChangedCommonProperties`

Required on leaves inside a `RecursiveEnable` container when the leaf overrides the container's default:

| Value | Meaning |
|-------|---------|
| `"16"` (0x10) | AccessControl/RolePermissions changed |
| `"48"` (0x30) | AccessControl + AccessLevel changed (node hidden from role) |

### Workflow: Enabling a Variable on OPC UA

1. **Create or open** the `.uad` file in `Physical/<Config>/<CPU>/Connectivity/OpcUaCs/`.
2. **Register** the file in `OpcUaCs/Package.pkg` as `Type="File"`.
3. **Define ACL Aliases** matching the roles in `Role.role` and the required access levels.
4. **Add a `<Variable>` node** under `<GlobalVariables>` or `<Task>` matching the PLC variable path (struct hierarchy = XML hierarchy).
5. **Set `AclInherit`** on each leaf node in case it should not inherit the default server access:
   - `"1"` for outputs (read-only inheritance from container).
   - `"2"` for inputs (explicit ACL, optionally with `AclId` to select a specific alias).
6. **Rebuild** the project to validate. Errors will reference the `.uad` file and line number.
