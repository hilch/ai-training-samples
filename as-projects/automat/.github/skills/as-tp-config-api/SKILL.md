---
name: as-tp-config-api
description: "Use when you need to know which XML elements/subelements are allowed inside a B&R Automation Studio technology-package configuration file (the files under Physical/<config>/<cpu>/mapp*). Triggers: what can go in a .axis / .purevaxcfg / .mpalarmxcore / .layout6d / .assembly6d file, allowed config nodes, valid config elements, mapp config schema, editor markup, list file endings for a technology package, discover config properties, what properties does this config support."
argument-hint: "GetConfigElements -TechnologyPackage <tp> [-TPVersion <v>] -FileEnding <.ending> [-ParentElement <path>] | ListFileEndings -TechnologyPackage <tp> [-TPVersion <v>]"
---

# AS TP Config API

Automation Studio **technology-package configuration** helper — discovers, **live from the AS installation**, which XML nodes a deployed config file (e.g. under `Physical/<config>/<cpu>/mappMotion/`) is allowed to contain, and returns them as structured **JSON**.

> **Why this exists:** Each technology package (mappMotion, mapp6D, mappServices, mappControl, mappVision, ...) ships an `Editors/` folder of META markup that dictates the legal nodes of its config files. There is no single document listing these — the markup is spread across cross-referencing XML fragments. This skill assembles that markup on demand so you can author config files correctly.

## When to Use

- **Discover allowed elements** for a config file ending (e.g. "what can go inside a `.axis` file for mappMotion 6.6.1?").
- **Drill into a sub-element** to list just its applicable subelements (via `-ParentElement`).
- **List valid file endings** for a technology package + version, each mapped to its editor markup file.
- Before creating or editing any `Physical/<config>/<cpu>/mapp*` configuration file by hand.

## Script

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-tp-config-api/scripts/use-as-tp-config-api.ps1 `
  -TechnologyPackage <tp> [-TPVersion <version>] -Action <action> [options]
```

Full script: [use-as-tp-config-api.ps1](./scripts/use-as-tp-config-api.ps1)

| Parameter             | Description                                                                                                   | Default            |
| --------------------- | ------------------------------------------------------------------------------------------------------------- | ------------------ |
| `-TechnologyPackage`  | Package name: `mappMotion`, `mapp6D`, `mappServices`, `mappControl`, `mappVision`, ... (case-insensitive)     | — (required)       |
| `-TPVersion`          | Package version, e.g. `6.6.1`. Highest installed version is used when omitted.                                | highest installed  |
| `-FileEnding`         | Config file ending, with or without leading dot (e.g. `.axis` / `axis`). Required for `GetConfigElements`.    | —                  |
| `-ParentElement`      | `Root` (whole config) or a `/`-delimited node-ID path to return one element + its subelements.                | `Root`             |
| `-Action`             | `GetConfigElements` or `ListFileEndings`.                                                                      | `GetConfigElements`|
| `-ProjectPath`        | Optional AS project dir; only biases AS-install selection toward the project's version.                       | `.`                |
| `-MaxDepth`           | Levels to expand **below the requested scope**. Deeper nodes collapse to stubs. `-1` = unlimited.             | `1`                |
| `-MaxElements`        | Safety budget on total emitted nodes per call; over-budget containers collapse and `truncated` is set. `0` = no cap. | `500`        |

The package name is matched case-insensitively, with a lenient fallback for loose names (e.g. `6D` → `mapp6D`, `mappControls` → `mappControl`).

### Responses: Shallow-by-default | Use: drill-down approach

Responses are **shallow by default** (`-MaxDepth 1`) to stay small and token-cheap:

- A `Root` call lists the top-level nodes; container nodes that have content are collapsed to a stub carrying `hasChildren: true` plus `childCount` (Group/Struct/Selection) or `selectionCount` (Selector).
- To see inside a collapsed node, call again with a more specific `-ParentElement` path — that node becomes the scope and its direct children are expanded.
- Increase `-MaxDepth` (e.g. `2`, `3`, or `-1` for unlimited) only when you deliberately want the deeper tree in one call.

## Output (JSON)

`GetConfigElements` returns a header plus an `elements` array. Each node carries: `kind` (`Property` | `Group` | `Selector` | `Selection` | `Struct`), `id`, `name`, `description`, and kind-specific fields:

- **Property**: `dataType`, `value`, `valueRange`, `unit`, `numberFormat`, `regularExpression`, `selectionType`/`selectionSubType` (cross-reference to other config objects, e.g. an `.axis`), and `valueList` (enumerated `Item`s).
- **Group** / **Struct**: `children[]`, plus `minCount`/`maxCount`/`dynamicMode` for dynamic/array nodes (`ID` like `[%I]`).
- **Selector**: `default` (the default selection) and `selections[]`, each a conditional branch with its own `children[]`.
- Boolean `Is*` attributes are collected under `flags`.

### Self-describing XML fields (so the JSON is unambiguous)

To prevent the common JSON→XML misreads, every response is **self-describing** — you do not need an external mapping table:

- The header carries an **`xmlAuthoring`** legend: the file `wrapper` (the `<Configuration><Element ID Type="…">` envelope the `elements` live inside, with `Type` already filled in) plus one-line `element` / `value` / `editable` / `arrays` / `selector` / `reference` rules. The `xmlAuthoring.wrapper` field in skill responses is the literal code structure to follow. It implicitly defines what's allowed by what it shows. Don't add attributes (like xmlns) that aren't in the template, even if they seem like standard XML practice. The template is the contract.
- Every value-bearing Property carries **`editable`** — `true` if you may author/override its `Value`, `false` if it is read-only (`isReadOnly`) or computed by AS. **Only emit nodes where `editable` is true.**
- A node whose `id` contains `[%I]` (a dynamic/array template) also carries **`idExample`** — the concrete first-instance id (e.g. `Segment[1]`). Emit one node per instance with a 1-based index; never write the literal `[%I]`.
- An enumerated Property carries **`valueFromList: true`** — its `Value` must be one of `valueList[].id` (the `id`, never the `name`).
- A cross-reference Property carries **`valueIsReference: true`** (with `selectionSubType`) — its `Value` is the `ID` of an existing object of that subtype defined elsewhere in the project.

When a node is collapsed by `-MaxDepth` (or the `-MaxElements` budget), its `children`/`selections` arrays are replaced by `hasChildren: true` + `childCount`/`selectionCount`. The header also reports `maxDepth`, `emittedCount`, and — when the budget was hit — `truncated: true` with a `hint` describing how to drill in or raise the limits.

## Translating the drilling JSON into project config XML

> The JSON returned by `GetConfigElements` is **schema metadata**, *not* the XML you write — it lists which nodes are legal and their constraints. Each response now embeds an **`xmlAuthoring`** legend plus the self-describing node fields above, so the transcription is mechanical. The skeleton and mapping below restate that embedded guidance for reference.

### The file skeleton the JSON lives inside

The JSON `elements` array describes the **children of the root config element** only (this is exactly what `xmlAuthoring.wrapper` shows). The file you write must wrap them:


```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Configuration>
  <Element ID="<your-unique-id>" Type="<type>">
    <!-- the JSON elements are transcribed here -->
  </Element>
</Configuration>
```

- `Type` = the **file ending without the dot** (`.layout6d` → `Type="layout6d"`, `.assembly6d` → `Type="assembly6d"`, `.axis` → `Type="axis"`).
- `ID` is **author-chosen** and must be unique across all files of that type in the project. The skill never reports this ID — it is yours to invent.

### Maps to the config file

Every response is self-describing (the `xmlAuthoring` legend plus the node fields above), so the transcription is mechanical — no external table needed:

- each `Property` → `<Property ID="..." Value="..." />`; for an enum (`valueFromList: true`) the `Value` is one of `valueList[].id`, **never** the `name`;
- each `Group`/`Struct` → `<Group ID="...">` wrapping its `children[]`;
- each `Selector` → `<Selector ID="..." Value="<selection-id>">` wrapping **only** the chosen `Selection`'s `children` (`default` is the default selection; a `Selection` is never its own tag);
- an array node (`id` contains `[%I]`) → one element per instance using its `idExample` (`[1]`, `[2]`, …), within `minCount`/`maxCount`;
- a cross-reference (`valueIsReference: true`) → `Value` is the `ID` of an existing object whose type is `selectionSubType`.

Author **only** nodes where `editable` is true; `name` and `description` are documentation, never written into the XML.

### End-to-end example

Drilling `mapp6D` `.layout6d` at `Segments` returns (abridged — note the self-describing fields):

```jsonc
{ "kind": "Group", "id": "Segments", "children": [
  { "kind": "Property", "id": "SegRow", "value": "1", "editable": true },
  { "kind": "Property", "id": "SegCol", "value": "2", "editable": true },
  { "kind": "Group", "id": "Segment[%I]", "idExample": "Segment[1]", "dynamicMode": "ShowNew", "children": [
    { "kind": "Property", "id": "TypeGrid240", "value": "0", "editable": true,
      "valueList": [ {"id":"0","name":"S3-AS"} ], "valueFromList": true },
    { "kind": "Property", "id": "Name", "editable": true },
    { "kind": "Property", "id": "IsMaster", "value": "false", "editable": true },
    { "kind": "Group", "id": "Gaps", "children": [
      { "kind": "Property", "id": "Left", "value": "0", "editable": true,
        "valueList": [ {"id":"0","name":"0"}, {"id":"3","name":"60"} ], "valueFromList": true } ] } ] } ] }
```

Transcribe it into the file (two segments shown) inside the `xmlAuthoring.wrapper` envelope — emitting only `editable: true` nodes, the array node's `idExample`, and enum `id`s for `valueFromList` properties:

```xml
<Configuration>
  <Element ID="Layout6D_1" Type="layout6d">
    <Group ID="Segments">
      <Property ID="SegRow" Value="2" />
      <Property ID="SegCol" Value="2" />
      <Group ID="Segment[1]">                   <!-- idExample, not the literal [%I] -->
        <Property ID="TypeGrid240" Value="0" />  <!-- valueFromList: the id "0", not "S3-AS" -->
        <Property ID="Name" Value="SegA" />
        <Property ID="IsMaster" Value="True" />
        <Group ID="Gaps">
          <Property ID="Left" Value="3" />       <!-- id "3" == 60 mm gap -->
        </Group>
      </Group>
      <Group ID="Segment[2]">
        <Property ID="TypeGrid240" Value="0" />
        <Property ID="Name" Value="SegB" />
        <Property ID="IsMaster" Value="False" />
      </Group>
    </Group>
  </Element>
</Configuration>
```

## Procedure

### Discover the root config for an ending

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-tp-config-api/scripts/use-as-tp-config-api.ps1 `
  -TechnologyPackage mappMotion -TPVersion 6.6.1 -FileEnding .axis
```

### Drill into a sub-element

`-ParentElement` takes a `/`-delimited path of node IDs. Because IDs are not globally unique (dynamic `[%i]` groups; repeated IDs under different `Selector` branches), a **full path** is required. When the current node is a `Selector`, the next segment must be one of its `Selection` IDs.

```powershell
# Just the pure-virtual-axis sub-tree of a .purevaxcfg
powershell -ExecutionPolicy Bypass -File .github/skills/as-tp-config-api/scripts/use-as-tp-config-api.ps1 `
  -TechnologyPackage mappMotion -TPVersion 6.6.1 -FileEnding .purevaxcfg -ParentElement PureVAxis

# Descend through a Selector into one of its selections
powershell -ExecutionPolicy Bypass -File .github/skills/as-tp-config-api/scripts/use-as-tp-config-api.ps1 `
  -TechnologyPackage mappMotion -TPVersion 6.6.1 -FileEnding .axis -ParentElement "BaseType/RotaryPeriodic"
```

If a path segment is wrong, the error lists the available IDs at that level.

### List valid file endings for a package

```powershell
powershell -ExecutionPolicy Bypass -File .github/skills/as-tp-config-api/scripts/use-as-tp-config-api.ps1 `
  -TechnologyPackage mappServices -Action ListFileEndings
```

Each entry is the file ending string (e.g. `.axis`).

## Notes

- Windows only. Requires PowerShell 5.1+ and an installed Automation Studio with the requested technology package.
- Read-only: the script never modifies the AS installation or the project.
- Output is JSON on stdout
