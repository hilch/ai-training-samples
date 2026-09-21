---
name: as-physical-hw-api
description: "Use when adding a hardware module to Hardware.hw, searching for available hardware modules by name pattern or by classification keyword/tag, listing permitted classification values from HardwareCatalogLayout.xml before classification search, reading a module's description/classification, listing a module's permittable I/O channel mappings, discovering how a module can be wired to other modules, or browsing the permitted configuration properties of a module — all from the AS installation's .hwx metadata. Triggers: add hardware module, search hardware module, search module by keyword, search module by tag, find module by classification, list permitted classifications, HardwareCatalogLayout.xml, CPU module, AutomationPC, get module info, get module description, IO mapping, I/O channels, channel name, data type, direction, possible connections, routing, connector, AutoConnect, AddHWModule, SearchHWModule, SearchByClassification, GetHWModuleInfo, GetIOMappingConfig, GetPossibleHWConnections, GetModuleProperties, module configuration, hardware properties, module config properties, Hardware.hw, IoMap.iom, .hwx file, ACOPOS, X20, module classification."
argument-hint: "AddHWModule <type> [version] | SearchHWModule <pattern> | SearchByClassification <keyword> | GetHWModuleInfo <type> [version] | GetIOMappingConfig <type> [version] | GetPossibleHWConnections <type> [version] | GetModuleProperties <type> [version] [-ParentElement <path>] [-MaxDepth <n>] | GetPermittedHwClassifications"
---

# AS Physical HW API

Automation Studio **Physical hardware** helper — adds, searches, and describes hardware modules using the AS installation's `.hwx` metadata so `Hardware.hw` is updated correctly without hand-editing XML.

> **IMPORTANT:** If the `GetHWModuleInfo` output contains `"CPU"` in the `HardwareModuleType` classification, it is **PROHIBITED** to add that module using `AddHWModule`. Adding a second CPU will corrupt the hardware configuration.

## When to Use

- **Add a hardware module** to `Hardware.hw` with exact `Type` and resolved `Version`
- **Search for hardware modules** by partial name to discover available types and versions
- **Search by classification keyword** to find modules by a human term (e.g. `PPC3100`, `AutomationPC`, `CPU`) matched against the `<Classification>` `Value` tags — useful when you don't know the module number
- **Get module description** to understand what a specific hardware module does
- **Get I/O mapping config** to list a module's permittable channels (name, data type, direction, description) for `IoMap.iom`
- **Get possible HW connections** to discover how a module can be wired (target module/connector, connection types) for `Hardware.hw`
- **Get module configuration properties** to browse the permitted `<Parameter>`/`<Selector>`/`<Group>` entries that go under the `<Module>` node in `Hardware.hw` (e.g. `FunctionModel`, `Supervision`, `StatusInfo`)
- **Get permitted HW classifications** — use before `SearchByClassification` to discover valid keywords

## Script

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
  -ProjectPath <project-path> -Action <action> [options]
```

Full script: [use-as-physical-hw-api.ps1](./scripts/use-as-physical-hw-api.ps1)

| Parameter          | Description                                                                                  | Default       |
| ------------------ | -------------------------------------------------------------------------------------------- | ------------- |
| `-ProjectPath`     | Path to AS project directory (required)                                                      | —             |
| `-Action`          | `AddHWModule`, `SearchHWModule`, `SearchByClassification`, `GetHWModuleInfo`, `GetIOMappingConfig`, `GetPossibleHWConnections`, `GetModuleProperties`, `GetPermittedHwClassifications` | `AddHWModule` |
| `-HwModuleType`    | Hardware module type name or search pattern (required for all actions except `SearchByClassification`)                       | —             |
| `-ClassificationKeyword` | Keyword matched (case-insensitive substring) against the `Value` attributes in each module's `<Classification>` block (required for `SearchByClassification`) | — |
| `-HwModuleVersion` | Optional module version for all read actions (highest installed if omitted)                  | none          |
| `-HwFileName`      | `.hw` file to register hardware module in                                                    | `Hardware.hw` |
| `-Target`          | I/O target generation (`SG3`/`SG4`/`SGC`) for `GetIOMappingConfig` and `GetModuleProperties`; derived from active CPU when omitted | none (auto)   |
| `-ResolveIncludes` | For `GetIOMappingConfig`: resolve shared X2X channel fragments. (`GetModuleProperties` always resolves includes.) | off           |
| `-ParentElement`   | For `GetModuleProperties`: `Root` or `/`-delimited node-ID path to drill into a sub-element | `Root`        |
| `-MaxDepth`        | For `GetModuleProperties`: levels to expand below the scope (`1` = shallow list, `-1` = unlimited) | `1`      |
| `-MaxElements`     | For `GetModuleProperties`: safety budget on total emitted nodes; over-budget containers collapse to stubs | `500` |

## Procedure

### Mandatory preconditions steps for adding hardware modules (MUST)

Before running `SearchHWModule`, `GetHWModuleInfo`, or `AddHWModule` for an unknown module type, choose the lookup style based on input quality:

1. **Keyword-first path (classification search):**
  - Use `GetPermittedHwClassifications` first to list valid classification values from `HardwareCatalogLayout.xml`.
  - Use `SearchByClassification` when the user provides meaningful keywords/tags preferably from that permitted list (for example `PPC3100`, `AutomationPC`, `CPU`, `Ethernet`, `Acopos6D`). If the search only using keywords returns too many results, you can fall back to also include technical keywords by then using the integrated description matching (e.g., `15.1 A`, `230 V`, `2.1GHz`, `15W`, `i3-7100U`, etc).
  - Use returned `moduleName` candidates with `GetHWModuleInfo` and/or `SearchHWModule` to narrow to exact type/version.
2. **Context-first path (Help Server MCP):**
  - Use `mcp_b_r_automatio_search_help` first when user input is vague, ambiguous, or lacks good module keywords.
  - Then use `mcp_b_r_automatio_get_page_by_id` to confirm product family naming, order key/module number, and variant rules.
  - Use the confirmed identifiers in `SearchHWModule` / `GetHWModuleInfo`.

Before `AddHWModule`, module identification must be verified by one of the two paths above. If confidence is low after classification results, escalate to Help Server MCP validation before adding.

### Search hardware modules by module type pattern

**REQUIRED Workflow choice:**
- Use **Help Server MCP first** when you need broad context discovery (the user did not provide reliable module keywords).
- Use **`SearchByClassification` first** when the user already provided good keywords/tags.
- Use this search path to narrow down to a specific module type and version, then validate with `GetHWModuleInfo` before adding with `AddHWModule`..

1. **(Context-first path) Use B&R Automation Studio Help Server (`mcp_b_r_automatio_search_help` and `mcp_b_r_automatio_get_page_by_id`) to research the product and identify module naming pattern:**
   - Search B&R documentation (e.g., "ACOPOSmulti 88A power supply", "ACOPOS P3 servo")
   - Document the base prefix (e.g., `8BVI` for ACOPOS multi, `8EI` for ACOPOS P3)
   - Understand the position-encoded naming scheme from "Order key" or "Model designation" sections
   - Note any specific suffixes or variants (e.g., `-3`, `-01`, `.C088`, etc.)

2. **Run progressive searches using the base prefix:**
   - Start broad with base prefix: `SearchHWModule -HwModuleType "8EI"` (find all ACOPOS P3 variants)
   - Narrow by size/features: `SearchHWModule -HwModuleType "8EI017"` (find 8EI017 variants)
   - Further narrow: `SearchHWModule -HwModuleType "8EI017HCS"` (specific configuration family)
   - Validate exact match: `GetHWModuleInfo -HwModuleType "8EI017HCS10.xxxx-1"` (confirm full specs)
   - If the output contains `"CPU"` in the `HardwareModuleType` classification, it is **PROHIBITED** to add that using `AddHWModule`. Adding a second CPU will cause a configuration error.

   ```powershell
   # Example: start broad, then narrow
   powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
     -ProjectPath . -Action SearchHWModule -HwModuleType "8EI"
   ```

3. **Example output from search:**
   ```json
   {"error":null,"basePath":"C:\\...\\Hardware\\Modules","matches":[{"moduleName":"X20DO8322","description":"8 Outputs 24 VDC / 0.5 A, Source","versions":["1.9.0.0"]},{"moduleName":"X20DO8332","description":"8 Outputs 24 VDC / 2 A, Source","versions":["1.7.0.0"]}],"success":true}
   ```
   Each match contains `moduleName`, `description` (from the highest-version `.hwx`), and all installed `versions`.

   > **Limit:** If the search pattern matches more than 100 modules the command returns an error (`success: false`). Use a more specific pattern (e.g. `X20DO8` instead of `X20`).

### Search modules by classification keyword

Use this when you have a **descriptive keyword** rather than a module number. It matches the keyword (case-insensitive substring) against the classification tags (`CpuType`, `HardwareModuleFamily`, `HardwareModuleType`, `IOType`, `NetworkType`, …). This is the preferred first step when the user gives usable keywords/tags from the permitted classification values list, but use Help Server MCP first when the request is too broad or lacks reliable terms.

#### Permitted classifications (MUST before `SearchByClassification`)

Before any classification search, you **MUST** list the permitted classification values first:

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/get-permitted-hw-classifications.ps1 `
  -ProjectPath .
```

Output (compact JSON, one line):

```json
{"success":true,"catalogFile":"C:\\Program Files (x86)\\BRAutomation\\AS6\\AS\\Hardware\\HardwareCatalogLayout.xml","count":1234,"values":["ACOPOS","AutomationPC","CPU","Digital","Ethernet"],"error":null}
```

Use these `values[]` as the allowed search vocabulary for `SearchByClassification`.

#### SearchByClassification usage

**Boolean operators** (case-insensitive substring matching, NOT binds to a term, AND binds tighter than OR):

| Operator | Syntax | Meaning |
|---|---|---|
| OR  | `\|` or `OR`        | match modules containing **any** group (e.g. `"6D \| Acopos6D \| mapp6D"`) |
| AND | `&`, `+` or `AND`   | a group matches only if **all** its terms are present (e.g. `"CPU & Ethernet"`) |
| NOT | leading `-`/`!`, or `NOT` | term must **not** be present; works infix too — `"CPU NOT X20"`, `"CPU -X20"` and `"CPU & -X20"` are equivalent |

Combine them freely, e.g. `"X20 & Digital | Acopos6D | ACOPOS NOT P3"` = `(X20 AND Digital) OR (Acopos6D) OR (ACOPOS AND NOT P3)`.

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
  -ProjectPath . -Action SearchByClassification -ClassificationKeyword "PPC3100"
```

Output (compact JSON, one line):

```json
{"success":true,"basePath":"C:\\...\\Hardware\\Modules","keyword":"PPC3100","matchCount":1,"matches":[{"moduleName":"5PPC3100.KBU1-000","description":"PPC3100 i3-7100U 2C 2.4/2.1GHz 3MB 15W","matchedValues":["PPC3100"]}],"success":true}
```

Each match contains `moduleName`, `description` (from the highest-version `.hwx`), and `matchedValues` (the classification `Value`s that matched). Combine boolean operators in one query for a broader or more targeted search:

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
  -ProjectPath . -Action SearchByClassification -ClassificationKeyword "6D | Acopos6D | mapp6D"
```

### Get hardware module description

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
  -ProjectPath . -Action GetHWModuleInfo -HwModuleType "X20DI8371"
```

Output example:

```
Module : X20DI8371 v1.6.1.0
HWX    : C:\Program Files (x86)\BRAutomation\AS6\AS\Hardware\Modules\X20DI8371\1.6.1.0\X20DI8371.hwx
Desc   : 8 Digital Inputs 24 VDC, Sink, IEC 61131-2, Type 1
Classification:
  HardwareModuleFamily: X20
  HardwareModuleType: IO
  IOType: Digital, In
```

The description is read from the `Description-en` attribute of the first `<Module>` element in the `.hwx` file. Classification lists all child nodes of the `<Classification>` element, grouping repeated tag names into comma-separated values.

### Add a hardware module

1. **Identify the exact module type** — if unknown, follow the Search workflow above first.
2. **Run the add command:**
   ```powershell
   powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
     -ProjectPath . -Action AddHWModule -HwModuleType "80VD100PD.C088-01"
   ```
3. **Optionally specify a version:**
   ```powershell
   ... -Action AddHWModule -HwModuleType "80VD100PD.C088-01" -HwModuleVersion "2.5.1.0"
   ```
4. **Verify** the module appears in `Physical/<ActiveConfig>/Hardware.hw`:
   ```xml
   <Module Name="80VD100PD.C088-01" Type="80VD100PD.C088-01" Version="2.5.1.0"></Module>
   ```

### Get a module's I/O mapping configuration

Lists the channels a module exposes for I/O mapping in `IoMap.iom` — the channel name, IEC data type, direction, and description.

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
  -ProjectPath . -Action GetIOMappingConfig -HwModuleType "X20DO2322"
```

Output (compact JSON, one line):

```json
{"success":true,"moduleType":"X20DO2322","moduleVersion":"1.4.1.0","resolvedTarget":"SG4","targetSource":"cpuIoMappingEditor","cpu":"X20CP1686X","resolveIncludes":false,"hwxFile":"...","channelCount":16,"channels":[{"channelName":"ModuleOk","dataType":"BOOL","direction":"In","description":"Module status (1 = module present)"},{"channelName":"DigitalOutput01","dataType":"BOOL","direction":"Out","description":"24 VDC / 0.5 A, source","filter":"NOT (DigitalOutputsPacked='on')"}]}
```

Each `channels[]` entry has:

| Field         | Meaning                                                                 |
| ------------- | ----------------------------------------------------------------------- |
| `channelName` | Channel ID used in `IoMap.iom` (e.g. `DigitalOutput01`)                 |
| `dataType`    | IEC data type (`BOOL`, `INT`, `UDINT`, …)                               |
| `direction`   | `In` or `Out`                                                           |
| `description` | English channel description                                             |
| `filter`      | *(only when present)* config condition under which the channel is valid (e.g. `NOT (DigitalOutputsPacked='on')`) |
| `connector`   | *(only when present)* the connector the channel belongs to (CPU interface channels) |

**Target generation:** the I/O channel set differs per AR target (`SG3`/`SG4`/`SGC`). The script auto-derives the target from the active configuration's CPU (`Cpu.pkg` → CPU `.hwx`); `targetSource` reports how it was resolved (`cpuIoMappingEditor`, `cpuChannelsTarget`, `cpuFirmwareTarget`, or `default`). Override with `-Target SG3|SG4|SGC` (then `targetSource` is `parameter`).

**Shared fragments:** advanced X2X bus-diagnostic channels are pulled from shared `<Include>` fragments and are **off by default**. Add `-ResolveIncludes` to merge them in.

**Maps to `IoMap.iom`:** an `Out` channel is mapped with `%QX`/`%QW`, an `In` channel with `%IX`/`%IW`, using the module instance name and the channel name, e.g.:

```
::Outputs.doLiftUp AT %QX."X20DO2322".DigitalOutput02;
```

### Get a module's possible hardware connections

Lists a module's connectors and how each can be routed — used to add `<Connection>` nodes under a `<Module>` in `Hardware.hw` (a freshly added module has no routing).

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
  -ProjectPath . -Action GetPossibleHWConnections -HwModuleType "X20DO2322"
```

Output (compact JSON, one line):

```json
{"success":true,"moduleType":"X20DO2322","moduleVersion":"1.4.1.0","hwxFile":"...","connectorCount":2,"connectors":[{"name":"SS1","type":"Plug","male":true,"connectionTypes":["X20Terminal"],"autoConnect":[{"targetModule":"X20TB12","targetConnector":"SS"}]},{"name":"SL","type":"Plug","male":true,"connectionTypes":["X20Electronic","X20ElectronicX2X+"],"autoConnect":[{"targetModule":"X20BM11","targetConnector":"SL1","preferredConnectionName":"X20Bus"}]}]}
```

Each `connectors[]` entry has:

| Field             | Meaning                                                                       |
| ----------------- | ----------------------------------------------------------------------------- |
| `name`            | Connector name on this module (used as `Connector="…"` in `Hardware.hw`)      |
| `type`            | `Plug`, `Cable`, or `Virtual`                                                 |
| `male`            | *(when present)* connector gender                                             |
| `networkType`     | *(when present)* network/bus type (`X2X`, `Ethernet`, `Powerlink`, …)         |
| `label`           | *(when present)* human-readable label (e.g. `ETH`, `PLK`)                     |
| `connectionTypes` | compatible connection types the connector accepts                             |
| `autoConnect`     | recommended routing recipes: `targetModule` (module **type**), `targetConnector`, optional `preferredConnectionName`/`required` |

**Maps to `Hardware.hw`:** an `autoConnect` recipe becomes a `<Connection>` under this module, where `TargetModule` is the **instance** `Name` of a present module of that type:

```xml
<Module Name="X20DO2322" Type="X20DO2322" Version="1.4.1.0">
  <Connection Connector="SS1" TargetModule="X20TB12_c" TargetConnector="SS" />
  <Connection Connector="SL"  TargetModule="X20BM11_b" TargetConnector="SL1" />
</Module>
```

### Get a module's configuration properties

Returns the permitted configuration entries for a module — the nodes you may author under `<Module>` in `Hardware.hw`.

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-physical-hw-api/scripts/use-as-physical-hw-api.ps1 `
  -ProjectPath . -Action GetModuleProperties -HwModuleType "8BVI0014HWS0.000-1"
```

**Drill-down approach.** Responses are shallow by default (`-MaxDepth 1`). Container nodes with content are collapsed to stubs (`hasChildren: true`, `childCount`). Call again with `-ParentElement` to go deeper; use `-MaxDepth -1` only when you want the full sub-tree in one call.

```powershell
... -ParentElement "DriveConfiguration/Channel[1]/RealAxis" -MaxDepth 2
```

**`flags` to watch:**

| Flag | Meaning |
|---|---|
| `isInvisible: true` / `visible: false` | Internal node — skip |
| `isReadOnly: true` / `isCalculated: true` | Read-only — do not author |
| `arAsHwAc: true` | Namespace root group (drive `DriveConfiguration`, motor `M1`, …) |

**Only author nodes where `editable: true`.**

#### JSON → Hardware.hw translation

| JSON node | Written to .hw as |
|---|---|
| `Parameter` or `Property` leaf | `<Parameter ID="{id}" Value="{value}"/>` |
| Same, with `location` field | `<Parameter ID="{id}" Location="{location}" Value="{value}"/>` |
| `valueFromList: true` | `Value` must be one of `valueList[].id` (use `id`, not `name`) |
| `valueIsReference: true` | `Value` must be the ID of an existing project object (e.g. axis name) |
| `Selector` | `<Parameter ID="{selectorId}" Value="{chosenSelectionId}"/>`, then its chosen Selection's children as flat siblings |
| `Group` / `Struct` wrapper | **Nothing** — every leaf lands directly under `<Module>` |
| Dynamic array Group (`%R` / `[%I]` in `id`) | `<Group ID="FileDevice1"/>` marker + flat `<Parameter>` siblings; replace placeholder with 1-based index, respect `minCount`/`maxCount` |

**`location` field.** Present only on nodes inside a Technology Package namespace (`flags.arAsHwAc: true` group). Copy it verbatim as `Location="{location}"`. Nodes without `location` flatten with no `Location` attribute. The `locationConfig.locationRoots[]` in the response header lists the available namespace root IDs (e.g. `DriveConfiguration`); `locationConfig` is `null` for modules with no namespace (e.g. `X20CP1686X`).

**Drive example** — `8BVI0014HWS0.000-1` uses `Property` leaves and all nodes carry a `location` field. Step-by-step drill-down to configure homing:

**Step 1** — `-ParentElement "DriveConfiguration/Channel[1]/RealAxis" -MaxDepth 1`:
```json
{ "kind": "Property", "id": "AxisReference", "location": "DriveConfiguration/Channel[1]/RealAxis",
  "editable": true, "selectionType": "ConfigurationElement", "valueIsReference": true }
{ "kind": "Selector", "id": "JerkFilter",    "location": "DriveConfiguration/Channel[1]/RealAxis",
  "default": "NotUsed", "selectionCount": 2, "hasChildren": true }
{ "kind": "Group",    "id": "Homing",        "location": "DriveConfiguration/Channel[1]/RealAxis",
  "childCount": 4, "hasChildren": true }
```

**Step 2** — `-ParentElement "DriveConfiguration/Channel[1]/RealAxis/Homing" -MaxDepth 1`:
```json
{ "kind": "Selector", "id": "Mode", "description": "Homing mode",
  "location": "DriveConfiguration/Channel[1]/RealAxis/Homing",
  "default": "Direct", "selectionCount": 12, "hasChildren": true }
{ "kind": "Property", "id": "RestorePositionVariable",
  "location": "DriveConfiguration/Channel[1]/RealAxis/Homing",
  "editable": true, "selectionType": "Variable", "selectionSubType": "McAcpAxRestorePosType" }
```

**Step 3** — `-ParentElement "DriveConfiguration/Channel[1]/RealAxis/Homing/Mode" -MaxDepth 1`:
```json
{ "kind": "Selector", "id": "Mode", "location": "DriveConfiguration/Channel[1]/RealAxis/Homing",
  "default": "Direct",
  "selections": [
    { "kind": "Selection", "id": "Direct",         "childCount": 2, "hasChildren": true },
    { "kind": "Selection", "id": "AbsoluteSwitch", "childCount": 8, "hasChildren": true },
    { "kind": "Selection", "id": "LimitSwitch",    "childCount": 8, "hasChildren": true },
    { "kind": "Selection", "id": "NotUsed" }
  ] }
```

**Step 4** — `-ParentElement "DriveConfiguration/Channel[1]/RealAxis/Homing/Mode/AbsoluteSwitch" -MaxDepth 1`:
```json
{ "kind": "Property", "id": "Position",     "location": "DriveConfiguration/Channel[1]/RealAxis/Homing/Mode",
  "dataType": "LREAL", "value": "0.0", "editable": true, "unit": "measurement units" }
{ "kind": "Property", "id": "Acceleration", "location": "DriveConfiguration/Channel[1]/RealAxis/Homing/Mode",
  "dataType": "REAL",  "value": "50.0", "editable": true, "unit": "measurement units/s²" }
{ "kind": "Property", "id": "SwitchEdge",   "location": "DriveConfiguration/Channel[1]/RealAxis/Homing/Mode",
  "value": "Positive", "editable": true, "valueFromList": true,
  "valueList": [{"id":"Positive"},{"id":"Negative"}] }
```

Each node's `location` is copied verbatim as `Location=` in the XML. `Mode` (a `Selector`) contributes a path segment, so its selection's children have `location` = `.../Homing/Mode` not `.../Homing`:

```xml
<Module Name="8BVI0014HWS0.000-1" Type="8BVI0014HWS0.000-1" Version="2.3.0.2" NodeNumber="1">
  <Parameter ID="AxisReference" Location="DriveConfiguration/Channel[1]/RealAxis"             Value="LiftAxis" />
  <Parameter ID="JerkFilter"    Location="DriveConfiguration/Channel[1]/RealAxis"             Value="Used" />
  <Parameter ID="Mode"          Location="DriveConfiguration/Channel[1]/RealAxis/Homing"      Value="AbsoluteSwitch" />
  <Parameter ID="Position"      Location="DriveConfiguration/Channel[1]/RealAxis/Homing/Mode" Value="5" />
  <Parameter ID="Acceleration"  Location="DriveConfiguration/Channel[1]/RealAxis/Homing/Mode" Value="100" />
</Module>
```

**CPU example** — `X20CP1686X` has no namespace; nested groups (`AR000`), a `Selector` (`ActivateWebServer`), and a dynamic array (`FileDevice%R`) all flatten:

```json
{ "kind": "Parameter", "id": "Simulation", "name": "Activate simulation", "dataType": "UDINT",
  "value": "0", "editable": true, "valueFromList": true,
  "valueList": [{"id":"0","name":"off"},{"id":"1","name":"on"}],
  "flags": { "visible": false, "arConfigSkip": true } }

{ "kind": "Selector", "id": "ActivateWebServer", "name": "Activate Web Server",
  "default": "0",
  "selections": [
    { "kind": "Selection", "id": "0", "name": "off" },
    { "kind": "Selection", "id": "1", "name": "on", "childCount": 2, "hasChildren": true }
  ],
  "flags": { "validated": true } }

{ "kind": "Group", "id": "FileDevice%R", "name": "File device %R", "minCount": 1,
  "children": [
    { "kind": "Parameter", "id": "FileDeviceName%../R", "name": "Name",
      "dataType": "STRING", "value": "USER", "editable": true },
    { "kind": "Parameter", "id": "FileDevicePath%../R", "name": "Path",
      "dataType": "STRING", "value": "USER_PATH", "editable": true }
  ] }
```

```xml
<Module Name="X20CP1686X" Type="X20CP1686X" Version="1.10.0.0">
  <Parameter ID="Simulation"        Value="1" />        <!-- leaf inside group "AR000" -->
  <Parameter ID="ActivateWebServer" Value="1" />        <!-- Selector: Value = chosen selection id -->
  <Parameter ID="ActivateSdm"       Value="1" />
  <Group     ID="FileDevice1" />                        <!-- dynamic array marker (%R → 1) -->
  <Parameter ID="FileDeviceName1"   Value="USER" />     <!-- %../R → 1 -->
  <Parameter ID="FileDevicePath1"   Value="USER_PATH" />
</Module>
```


## Notes

- Use **`SearchByClassification`** for keyword/tag-driven discovery, and use **Help Server MCP** (`mcp_b_r_automatio_search_help` and `mcp_b_r_automatio_get_page_by_id`) for broad context discovery when user keywords are weak or missing
- Auto-detects installed AS versions from the Windows registry; project version matching is handled automatically
- Hardware module lookup path: `AS/Hardware/Modules/<Type>/<Version>/`
- `SearchHWModule`, `SearchByClassification`, `GetHWModuleInfo`, `GetIOMappingConfig`, `GetPossibleHWConnections`, and `GetModuleProperties` do **not** write any project files — the `-ProjectPath` parameter is still required by the script but no files are modified
- Output JSON is compact (single line, no insignificant whitespace) and structured for agent parsing
- Windows only. Requires B&R Automation Studio 6.x and PowerShell 5.1+
