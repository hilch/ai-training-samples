---
description: 'Guidelines for B&R MappView HMI development'
applyTo: '**/*.content,**/*.page,**/*.widget,**/*.binding,**/*.eventbinding,**/*.eventscript,**/*.theme,**/*.dialog,**/*.tmx'
---

# B&R MappView Development Guidelines

You are an expert in B&R MappView development (Automation Studio 6.0+).
Follow these guidelines when generating code, bindings, and structures for MappView projects in B&R Automation Studio.

## General Instructions

- **Tool Version:** Use Automation Studio 6.0 or newer.
- **Goal:** Create maintainable, high-standard HMI projects with consistent structure and naming.
- **Documentation:** Always refer to the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) for mappView documentation and best practices.
- **Theme:** Use a consistent theme across all pages. Ensure all widgets use the default theme with additional styles defined in the theme file.

## Architecture & File Structure

### Package File Management (`Package.pkg`)
The `Package.pkg` file is the backbone of the Automation Studio project structure. It lists all files and sub-packages contained within a directory.
**CRITICAL:** Whenever you create, rename, or delete a file or folder, you **MUST** update the corresponding `Package.pkg` file.

**Example `Package.pkg`:**
```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
    <Object Type="File">PageName.page</Object>
    <Object Type="File">PageName.content</Object>
    <Object Type="File">PageName.tmx</Object>
  </Objects>
</Package>
```

### Logical View
Organize the `Logical View` to separate common resources from specific page content, respecting the resolution folder structure.

```text
Logical View
└── mappView
    ├── Resources
    │   ├── Snippets
    │   └── Texts
    └── 16by9                   # Resolution Folder (e.g., 16by9, 16by10)
        ├── Dialogs             # Dedicated folder for Dialogs
        │   └── DialogName
        │       ├── DialogName.dialog
        │       ├── DialogName.content
        │       └── DialogName.tmx
        ├── Pages               # Dedicated folder for Pages
        │   └── PageName
        │       ├── PageName.page
        │       ├── PageName.content
        │       └── PageName.tmx
        ├── Resources
        └── Variables
```

### Configuration View (Physical View)
Mirror the Logical View structure in the `Physical View` under the active configuration.

```text
Physical View
└── <Config>
    └── <Hardware>
        └── mappView
            ├── Visu.vis
            ├── Config.mappviewcfg
            └── MainVisu        # Visualization Folder
                ├── PageName    # All bindings for the page go here
                │   ├── PageName.binding
                │   ├── PageName.eventbinding
                │   └── PageName.eventscript
                └── DialogName
                    ├── DialogName.binding
                    └── DialogName.eventbinding
```

## File and XML Structure

### Visualization Hierarchy
The MappView project is structured hierarchically. The `Resolution Folder` (e.g., `16by9`) represents a specific visualization scope (e.g., "MainVisu", "ServiceVisu").

1.  **Visualization (`.vis`)**: The root file. Defines the start page, themes, and lists all pages, contents, and binding sets used.
2.  **Layout (`.layout`)**: Defines the geometric areas (`Area`) where contents can be placed.
3.  **Page (`.page`)**: Assigns `Content` files to `Layout Areas`.
4.  **Content (`.content`)**: Contains the actual UI elements (`Widgets`).
5.  **Bindings (`.binding`, `.eventbinding`, `.eventscript`)**: Connect widgets to variables (data) or actions (events).

### XML Structure Examples

#### 1. Visualization (`.vis`)
Registers all resources.
```xml
<vdef:Visualization id="vis_1" ...>
  <StartPage pageRefId="Main" />
  <Pages>
    <Page refId="Main" />
  </Pages>
  <Contents>
    <Content refId="commonHeader" />
    <Content refId="Main" />
  </Contents>
  <BindingsSets>
    <BindingsSet refId="Main_binding" />
  </BindingsSets>
  <EventBindingsSets>
    <EventBindingsSet refId="Main_eventbinding" />
  </EventBindingsSets>
  <EventScriptsSets>
    <EventScriptsSet refId="Main_eventscript" />
  </EventScriptsSets>
</vdef:Visualization>
```

#### 2. Layout (`.layout`)
Defines areas.
```xml
<ldef:Layout id="pageLayout" height="1080" width="1920" ...>
  <Areas>
    <Area id="AreaHeader" height="90" width="1920" left="0" top="0" />
    <Area id="AreaContent" height="990" width="1920" left="0" top="90" />
  </Areas>
</ldef:Layout>
```

#### 3. Page (`.page`)
Maps contents to layout areas.
```xml
<pdef:Page id="Main" layoutRefId="pageLayout" ...>
  <Assignments>
    <Assignment type="Content" baseContentRefId="commonHeader" areaRefId="AreaHeader" />
    <Assignment type="Content" baseContentRefId="Main" areaRefId="AreaContent" />
  </Assignments>
</pdef:Page>
```

#### 4. Content (`.content`)
Defines widgets.

**Required attributes:**
- The `<Content>` root element **must** have `width` and `height` attributes matching the layout area dimensions.
- Every `<Widget>` element **must** include a `zIndex` attribute (integer, e.g. `zIndex="0"`).
- Widgets nested inside container widgets (e.g. `GroupBox`) are written as child `<Widget>` elements inside the parent's `<Widgets>` element.

**Unit and format**
- Use `format="{'metric':{'decimalPlaces':2,'minimumIntegerDigits':3,'maximumIntegerDigits':4},'imperial':{'decimalPlaces':2,'minimumIntegerDigits':3,'maximumIntegerDigits':4},'imperial-us':{'decimalPlaces':2,'minimumIntegerDigits':3,'maximumIntegerDigits':4}}"` for formatting on numeric widgets. Adjust `decimalPlaces`, `minimumIntegerDigits`, and `maximumIntegerDigits` as needed.
- The default units are based on the "UNECE/CEFACT Trade Facilitation Recommendation No.20" standard. Use `unit="{'metric':'CEL','imperial':'FAH','imperial-us':'KEL'}"`. Look for `Available standard units` in the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools).

```xml
<Content id="Main" width="1920" height="990"
  xmlns="http://www.br-automation.com/iat2015/contentDefinition/v2"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Widgets>
    <Widget xsi:type="widgets.brease.Button" id="startBt" left="10" top="10" width="120" height="40" zIndex="0" />
    <Widget xsi:type="widgets.brease.NumericOutput" id="speedNumOut" left="10" top="60" width="120" height="40" zIndex="1" />
    <!-- GroupBox with nested widgets -->
    <Widget xsi:type="widgets.brease.GroupBox" id="settingsGrp" left="150" top="10" width="300" height="200" zIndex="2">
      <Widgets>
        <Widget xsi:type="widgets.brease.NumericInput" id="targetSpeedNumIn" left="10" top="30" width="120" height="40" zIndex="0" />
      </Widgets>
    </Widget>
  </Widgets>
</Content>
```

#### 5. Binding (`.binding`)
Links widget properties to OPC UA variables or MpLinks.

**Critical: `id` must match `.vis` `refId`**
- The `id` attribute on `<BindingsSet>`, `<EventBindingsSet>`, etc must exactly match the `refId` used in the `.vis` file under `<BindingsSets>`.
- Each `refId` in the `.vis` must be **unique**

**OPC UA variable address syntax:**
| Variable location | Syntax |
|---|---|
| Task-local variable | `::TaskName:variableName` |
| Task-local struct member | `::TaskName:structName.memberName` |
| Global variable (no task) | `::AsGlobalPV:GlobalVarName` |
| Global struct member | `::AsGlobalPV:GlobalStructName.memberName` |

```xml
<BindingsSet id="Main_binding"
  xmlns="http://www.br-automation.com/iat2015/binding/engineering/v2"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Bindings>
    <!-- OPC UA variable → widget (oneWay display) -->
    <Binding mode="oneWay">
      <Source xsi:type="opcUa" refId="::TaskName:actSpeed" attribute="value" />
      <Target xsi:type="brease" contentRefId="Main" widgetRefId="speedNumOut" attribute="value" /> <!-- Use attribute="node" if OpcUa Unit, Limit, etc Nodes should be applied -->
    </Binding>
    <!-- Widget → OPC UA variable and back (twoWay control) -->
    <Binding mode="twoWay">
      <Source xsi:type="opcUa" refId="::TaskName:setSpeed" attribute="value" />
      <Target xsi:type="brease" contentRefId="Main" widgetRefId="speedNumIn" attribute="value" />
    </Binding>
    <!-- MpLink binding -->
    <Binding mode="oneWay">
      <Source xsi:type="mapp" refId="gAlarmXCore" attribute="link" />
      <Target xsi:type="brease" contentRefId="Alarm" widgetRefId="AlarmList1" attribute="mpLink" />
    </Binding>
  </Bindings>
</BindingsSet>
```

#### 6. Event Binding (`.eventbinding`)
Links widget events to actions.

**Navigation action:** Use `pageId=` attribute to navigate.

**SetValue action:** Use `SetValueBool`, `SetValueReal`, etc. to write a PLC variable directly on a button click.

```xml
<EventBindingSet id="commonHeader"
  xmlns="http://www.br-automation.com/iat2014/eventbinding/v2"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Bindings>
    <EventBinding id="commonHeader.LoginInfo1.Click">
      <Source xsi:type="widgets.brease.LoginInfo.Event" contentRefId="commonHeader" widgetRefId="LoginInfo1" event="Click" />
      <EventHandler>
        <Action>
          <Target xsi:type="clientSystem.Action">
            <Method xsi:type="clientSystem.Action.OpenDialog" dialogId="login" autoClose="true" />
          </Target>
        </Action>
      </EventHandler>
    </EventBinding>
    <!-- Navigate to a page -->
    <EventBinding id="navMain.mainBt.Click">
      <Source xsi:type="widgets.brease.Button.Event" contentRefId="navMain" widgetRefId="mainBt" event="Click" />
      <EventHandler>
        <Action>
          <Target xsi:type="clientSystem.Action">
            <Method xsi:type="clientSystem.Action.Navigate" pageId="Main" />
          </Target>
        </Action>
      </EventHandler>
    </EventBinding>
    <!-- Write a BOOL TRUE to PLC on button click -->
    <EventBinding id="Main.resetBt.Click">
      <Source xsi:type="widgets.brease.Button.Event" contentRefId="Main" widgetRefId="resetBt" event="Click" />
      <EventHandler>
        <Action>
          <Target xsi:type="opcUa.Action">
            <Method xsi:type="opcUa.NodeAction.SetValueBool" refId="::TaskName:resetFlag" value="true" />
          </Target>
        </Action>
      </EventHandler>
    </EventBinding>
  </Bindings>
</EventBindingSet>
```

#### 7. Dialog (`.dialog`)
A dialog is a modal overlay. It requires:
1. Its **own dedicated layout file** (`.layout`) — separate from page layouts.
2. A **content file** (`.content`) with the dialog's widgets.
3. A **dialog definition file** (`.dialog`) that ties layout and content together.
4. Registration in the `.vis` file under `<Dialogs>`.

**Logical View location:** `<VisualizationPackage>/Dialogs/DialogName/`

```xml
<!-- DialogName.layout -->
<ldef:Layout id="DialogNameLayout" height="300" width="400"
  xmlns:ldef="http://www.br-automation.com/iat2015/layoutDefinition/v2">
  <Areas>
    <Area id="areaDialog" height="300" width="400" left="0" top="0" />
  </Areas>
</ldef:Layout>

<!-- DialogName.dialog -->
<ddef:Dialog id="DialogName" layoutRefId="DialogNameLayout"
  xmlns:ddef="http://www.br-automation.com/iat2015/dialogDefinition/v2">
  <Assignments>
    <Assignment type="Content" baseContentRefId="DialogNameContent" areaRefId="areaDialog" />
  </Assignments>
</ddef:Dialog>

<!-- DialogNameContent.content -->
<Content id="DialogNameContent" width="400" height="300"
  xmlns="http://www.br-automation.com/iat2015/contentDefinition/v2"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Widgets>
    <Widget xsi:type="widgets.brease.Login" id="loginWidget" left="10" top="10" width="370" height="270" zIndex="0" />
  </Widgets>
</Content>
```

**Registration in `.vis`:**
```xml
<Dialogs>
  <Dialog refId="DialogName" />
</Dialogs>
<Contents>
  <Content refId="DialogNameContent" />
</Contents>
```

**Opening from an event binding:**
```xml
<Method xsi:type="clientSystem.Action.OpenDialog" dialogId="DialogName" autoClose="true" />
```
#### 8. Event Script XML Structure (`.eventscript`)
```xml
<?xml version="1.0" encoding="utf-8"?>
<EventScriptsSet id="MyPage"
  xmlns="http://www.br-automation.com/iat2015/eventscript/v2"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

  <EventScript id="MyPage.Script1"
    contentId="MyContentId"
    eventScriptSetId="MyPage">
    <![CDATA[
      // @eventScriptSetId  MyPage
      // @contentId  MyContentId

      // React to OPC UA variable changes
      opcua('::AsGlobalPV:gMyVar').valueChanged(function(e) {
          var val = e.detail.value;
          // Write result to a session variable
          variables.MySessionVar.setValueString(String(val));
      });
    ]]>
  </EventScript>
</EventScriptsSet>
```

## Workflow: Adding UI Elements

### Adding a Widget (e.g., Button)
1.  **Edit `.content`**: Add the `<Widget>` element with a unique ID (suffix rule).
2.  **Edit `.binding`**: If the widget displays data, add a `<Binding>` entry.
3.  **Edit `.eventbinding`**: If the widget triggers an action, add an `<EventBinding>` entry.

### Adding a New Content
1.  **Create `.content`**: Define the new content file and add widgets.
2.  **Update `.page`**: Add an `<Assignment>` to place the content in a specific Layout Area.
3.  **Update `.vis`**: Add the `<Content refId="..." />` to the `<Contents>` list.

### Adding a New Page
1.  **Create `.page`**: Define the page and assign a Layout.
2.  **Create `.content`s**: Create the necessary content files.
3.  **Update `.page`**: Assign the contents to the layout areas.
4.  **Update `.vis`**:
    - Add `<Page refId="..." />` to `<Pages>`.
    - Add all new `<Content refId="..." />` entries to `<Contents>`.
    - Add `<BindingsSet refId="..." />` and `<EventBindingsSet refId="..." />` if created.

### Adding a Dialog
1.  **Create a dedicated layout** (`.layout`): Define a single area covering the full dialog dimensions. Dialog layouts go in the Layouts folder.
2.  **Create the content** (`.content`): Add widgets. Root element **must** have `width` and `height`.
3.  **Create the dialog definition** (`.dialog`): Reference the layout and assign the content to the layout area.
4.  **Update `Package.pkg`** in the dialog's Logical View folder: register all three files.
5.  **Update `.vis`**:
    - Add `<Dialog refId="DialogName" />` to `<Dialogs>`.
    - Add `<Content refId="DialogNameContent" />` to `<Contents>`.
6.  **Trigger the dialog** via an event binding using `clientSystem.Action.OpenDialog` with `dialogId="DialogName"`.

### Using Event Scripts for Dynamic Behavior
Event scripts are the configuration of **events** with **executable actions** written in JavaScript. They are the primary tool for runtime logic that reacts to changes and updates the HMI accordingly. Common use cases include:
- **Changing widget properties** (e.g., enabling/disabling buttons, setting text, updating values)
- **Switching widget styles** for visual indicators (e.g., status ellipses, colored shapes)
- **Building dynamic data** (e.g., JSON transform strings for Paper widgets)
- **Conditional visibility** (e.g., showing/hiding widgets based on machine state)

**Available event sources** (not limited to OPC UA — consult the B&R Automation Studio Help Server via `mcp_b_r_automatio_*` tools for complete documentation on each):
- **OPC UA events** — `opcua('::AsGlobalPV:varName').valueChanged(...)` — react to PLC variable changes.
- **Widget events** — `widgets.widgetId.eventName(...)` — react to user interactions (click, valueChanged, focus, etc.).
- **clientSystem events** — `clientSystem.eventName(...)` — react to system-level events (page loaded, language changed, theme changed, etc.).
- **opcuaSystem events** — `opcuaSystem.eventName(...)` — react to OPC UA connection state changes.
- **Timer events** — `timers.%TimerName%.start(...)` / `timers.%TimerName%.elapsed(...)` — session timer control and elapsed events.

**General workflow:**
1.  **Identify the event source** — determine what triggers the behavior (PLC variable change, widget interaction, system event, timer, etc.). For OPC UA variables, enable them as OPC UA nodes in the `.uad` file.
2.  **Create an eventscript** that subscribes to the event and performs the desired action (set widget properties, update session variables, switch styles, navigate, etc.).
3.  **Register the eventscript** in the `.vis` file under `<EventScriptsSets>`.
4.  **Add bindings** if the eventscript writes to a session variable that a widget consumes.

**Example — Style switching (status indicator):**
If the target visual property (e.g., `backColor`) is a **styleable property**, you must define named styles in `Resources/Themes/<ThemeName>/brease/<WidgetType>.styles` and switch between them using `setStyle()`. Set the widget's initial `style` attribute in the `.content` file.

**Example — Generic value-based widget update:**
An eventscript can react to any PLC variable change and update widget properties directly (when `@contentId` is set) or write to session variables for binding. See the Event Script Examples section below.

## Naming Conventions

### Pages & Contents
- **Pages:** Use descriptive names (e.g., `MainOverview`, `Settings`).
- **Contents:** Must use the page name as a prefix if the page has multiple contents.
- **Folders:** Match the page/dialog name.

### Widgets
- **Suffix Rule:** Always append a suffix describing the widget type to facilitate debugging in binding files.
  - `startButton` (Button)
  - `speedNumericOutput` (NumericOutput)
  - `tempNumericInput` (NumericInput)
  - `statusTextOutput` (TextOutput)

### Bindings
- **Naming:** Bindings should automatically match the name of the content they are bound to.

## Best Practices & Features

### Event Scripts (Preferred)
- **Usage:** Use Event Scripts (`.eventscript`) instead of Event Bindings, Snippets, or Expressions whenever possible.
- **Reasoning:** They run on target (JavaScript), are easier to debug, and streamline the code.
- **Note:** Requires an extra license.
- **File Extension:** `.eventscript`
- **Location:** Physical View under the page/dialog folder alongside `.binding` and `.eventbinding`.
- **Registration:** Must be registered in `.vis` under `<EventScriptsSets>`.
- **Annotations:** Every event script requires `@eventScriptSetId` (matching the `.vis` refId preferably). The `@contentId` annotation is **optional** and determines scope (see below).
- **Session Variable Access (example: String type):** Use `variables.VarName.setValueString(value)` to write, `variables.VarName.getValueString()` to read. Other types have corresponding methods (e.g., `setValueBool`, `setValueInteger`, `setValueReal`). Refer to the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) and other instruction files for the full list of session variable accessor methods.
- **Widget Access:** When `@contentId` is set, widgets are accessed directly as `widgets.widgetId.actionName(value)` — the `@contentId` already scopes access to that content. Do **not** prefix with the content ID. This applies even when widgets are nested inside containers (e.g., GroupBox). When `@contentId` is **omitted**, widget access is not available — only `opcua()`, `variables.`, `clientSystem.`, `timers.`, and `opcuaSystem.` are accessible.
- **OPC UA Node Enablement:** For an eventscript to react to PLC variable changes via `opcua().valueChanged()`, the variable **must** be enabled as an OPC UA node in the `.uad` file under `Connectivity/OpcUaCs/` in the Physical View. Without this, the eventscript will not receive value change notifications. Load the `as-project-opcua.instructions.md` instruction file for more details on enabling OPC UA variables.

#### Event Script Scope Strategy

The `@contentId` annotation determines **scope** and reusability:

| Use Case | Include `@contentId`? | When to Use | Benefit |
|----------|----------------------|------------|---------|
| **Single-content logic** | ✅ **YES** | Form validation, dialog-specific behavior, widget direct manipulation | Scoped to one content. Prevents unintended side effects. Easier to debug. |
| **Multi-content reusable logic** | ❌ **NO** | Updating shared session variables used by multiple contents | Allows code reuse. One script, multiple bindings. Avoids duplication. |

**Rule:** If an event script updates a session variable shared across multiple contents → **omit `@contentId`**. If the script directly manipulates a single content's widgets → **include `@contentId`** for scope clarity.

**Multi-content widget manipulation:** If you need the same eventscript logic to manipulate widgets across multiple contents, you **cannot** use a single script without `@contentId` (widget access requires it). Instead, either:
- Create **separate eventscript files** for each content, each with its own `@contentId`. The docs explicitly allow multiple scripts referencing the same or different contents.
- Use a single script **without** `@contentId` that writes to a **session variable**, then **bind** that session variable to widget properties across contents in `.binding` files.

### Dialogs & Messages
- **Dialogs:** Use for wizards, configurations, or non-critical information. Avoid distracting pop-ups.
- **Messages:** Avoid general use. Only use for critical confirmations (e.g., Safety Acknowledge, Critical Alarms).

### Snippets & Expressions (Avoid)
- **Snippets:** Keep to a minimum. Use only for dynamic text updates or embedding OPC-UA variables in text if Event Scripts cannot be used.
- **Expressions:** Avoid. They run client-side and are difficult to debug. Logic should be handled in the PLC or Event Scripts.

### Session Variables
- **Usage:** Use only for small, internal HMI state (e.g., tab selection).
- **Warning:** Remember they are unique per client session. Shared state must be handled in the PLC.

### Styleable vs Regular Widget Properties

Widget properties fall into two categories. Check the widget documentation in the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) to determine which category a property belongs to.

| Category | Examples | Can change at runtime via | Notes |
|----------|----------|--------------------------|-------|
| **Regular properties** | `enable`, `visible`, `style`, `text`, `value` | `setEnable()`, `setVisible()`, `setStyle()`, `setText()` etc. | Actions listed under "Actions and events" in widget help |
| **Styleable properties** | `backColor`, `lineColor`, `borderWidth`, `opacity`, `borderStyle`, `fontSize`, `textColor` | Theme styles only (`setStyle()`) | Cannot be changed directly at runtime. Must define named styles in `.styles` files and switch between them. |

**Common mistake:** Attempting to use `setProperty('backColor', '#00FF00')` or similar on a styleable property — this has **no effect** at runtime. Widgets like `Ellipse`, `Rectangle`, `Line`, `Label`, `Button`, etc. all have styleable `backColor`.

**Correct approach for dynamic appearance:**
1. Define named styles in the widget's `.styles` file within each theme.
2. Use `setStyle('styleName')` in eventscripts or `SetStyle` in event bindings.

```javascript
// CORRECT — switch to a predefined theme style
widgets.StatusEllipse.setStyle('green');

// WRONG — backColor is styleable, this has no effect at runtime
widgets.StatusEllipse.setProperty('backColor', '#00FF00');
```

### Adding Widget Styles to Themes

When you need dynamic visual indicators (e.g., status ellipses, colored shapes), you must define styles in the `.styles` file for the corresponding widget type in **each theme** used by the visualization.

- **Location:** `Resources/Themes/<ThemeName>/brease/<WidgetType>.styles`
- **All themes must match:** If the visualization uses multiple themes (e.g., Light and Dark), add the style with the **same `id`** to every theme's `.styles` file.
- Style `id` values are **case-sensitive** and must match the string passed to `setStyle()`.
- Reuse existing theme keyword variables (e.g., `${theme_WidgetGreenStrokeColor}`) where possible for consistency.

```xml
<!-- Example: Adding a custom 'black' style to Ellipse.styles -->
<Style id="black" xsi:type="widgets.brease.Ellipse"
    borderWidth="2px"
    backColor="#000000"
    lineColor="${theme_WidgetStrokeColorCollection}"
    opacity="1" />
```

## Widget Selection

- Find the full list of widgets in the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools). Consider performance implications of widget classes (A, B, C) relative to the target hardware (e.g., avoid Class C on T50 panels).
- Widget source code is available in `<InstallationPath>\AS\TechnologyPackages\mappView\<version>\Widgets\brease` for more detailed information on how widgets are implemented. Run the **`AS: Get Installed Versions`** VS Code task (defined in `.vscode/tasks.json`, script at `.github/skills/as-utility/scripts/get-as-installations.ps1`) to list all installed AS versions and their paths. Use the one that closely matches the version in the `*.apj` file.

### Preferred Widgets
- All Base Widgets
- `Paper`
- `ProgressBar`
- `TabControl`
- `Table`
- `ToggleSwitch`

### Not Recommended
- `ContentCarousel`
- `FlyOut`

### Widget Classes (Performance)
- Be mindful of Widget Classes (A, B, C) relative to the target hardware (e.g., avoid Class C on T50 panels).

## Security & Configuration

### `config.mappview` Settings
- **Protocol:** `HTTP`
- **Max Clients:** At least `2` (1 local + 1 remote).
- **Startup User:** `Anonymous` token.
- **Diagnostics:** Enable the diagnostic page for debugging (set appropriate role).
- **Default Visualization:** Must be set (Default ID: `Visu`).

## Examples

### Widget Naming Example
```xml
<!-- Good: Suffix indicates type -->
<Widget xsi:type="widgets.brease.Button" id="startBt" ... />
<Widget xsi:type="widgets.brease.NumericOutput" id="currentSpeedNumOut" ... />

<!-- Bad: Ambiguous names -->
<Widget xsi:type="widgets.brease.Button" id="Button1" ... />
<Widget xsi:type="widgets.brease.NumericOutput" id="Value" ... />
```

### Event Script Examples

> **Note on `e.detail` properties:** The `valueChanged` callback receives an event object. The available properties (e.g., `e.detail.value`, `e.detail.newValue`, `e.detail.oldValue`) depend on the widget or OPC UA event type. Always consult the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) documentation for the specific event payload structure of the widget or OPC UA subscription being used.

**Generic Example — React to PLC variable and update widget** (WITH contentId):
```javascript
// @eventScriptSetId ControlScript
// @contentId Content_Controls_Operator

// Goal: Enable/disable the Start button based on machine state.
// When gMachineReady is TRUE, enable the button; otherwise disable it.
opcua('::AsGlobalPV:gMachineReady').valueChanged(function(e) {
    // e.detail.newValue contains the new BOOL value of gMachineReady
    widgets.StartPushButton.setEnable(e.detail.newValue);
});

// Goal: Update a TextOutput widget with the current mode string from PLC.
opcua('::AsGlobalPV:gMachineStateText').valueChanged(function(e) {
    widgets.MachineStateTextOutput.setValue(e.detail.newValue);
});
```

**Multi-Content Pattern — Build JSON for shared session variable** (WITHOUT contentId):
```javascript
// @eventScriptSetId eventscript1
// (NO @contentId — intentional; script runs globally, cannot access widgets directly)

// Goal: Build a JSON transform string from multiple PLC state values.
// Multiple contents (Content_Overview, Content_Details, etc.) bind to the
// session variable 'TransformString' to update their Paper widgets.

var Batchstate = [0, 0];

// Helper: builds a JSON array string for the Paper widget 'transform' property.
// This is a user-defined function — eventscripts support standard JavaScript.
// No imports are needed; all standard JS functions (JSON.stringify, Array methods,
// Math, String, etc.) are available out of the box.
function buildJson(stateArray) {
    var arr = [];
    for (var i = 0; i < stateArray.length; i++) {
        arr.push('{"select":"#Elem' + i + '","fill":' + stateArray[i] + '}');
    }
    return '[' + arr.join(',') + ']';
}

// Each OPC UA variable needs its own valueChanged subscription
opcua('::AsGlobalPV:gProductState[0]').valueChanged(function(e) {
    Batchstate[0] = e.detail.newValue;
    variables.TransformString.setValueString(buildJson(Batchstate));
});

opcua('::AsGlobalPV:gProductState[1]').valueChanged(function(e) {
    Batchstate[1] = e.detail.newValue;
    variables.TransformString.setValueString(buildJson(Batchstate));
});

// Now Content_Overview, Content_Details, Content_Diagnostics can all bind to TransformString
```

> **JavaScript in eventscripts:** Eventscripts run standard JavaScript on the client. All built-in JS functions (`JSON.stringify`, `parseInt`, `Math.*`, string/array methods, etc.) are available without imports. You define your own helper functions directly in the script. There is no module system or import mechanism — everything is self-contained within the `<![CDATA[...]]>` block.
