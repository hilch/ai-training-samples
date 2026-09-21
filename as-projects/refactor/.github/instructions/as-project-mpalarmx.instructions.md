---
description: Guidelines for B&R Automation Studio mapp AlarmX technology package configuration (MpAlarmXCore, MpAlarmXList, MpAlarmXHistory, alarm management, alarm reactions, snippets).
applyTo: '**/*.mpalarmxcore,**/*.mpalarmxlist,**/*.mpalarmxhistory,**/*.mpalarmxcategory,**/*.mpcomgroup'
---

# B&R Automation Studio mapp AlarmX Instructions (Build + Code)

Use this when creating or changing AlarmX in Automation Studio 6.  
**Goal:** build valid XML in mappServices and wire it correctly in Structured Text.

## 1) Scope

- Applies to all AlarmX configuration files under `Physical/<Config>/<CPU>/mappServices/` and ST-Code that references them:
  - `Physical/<Configuration>/<CPU>/mappServices/**/*.mpalarmxcore`
  - `Physical/<Configuration>/<CPU>/mappServices/**/*.mpalarmxlist`
  - `Physical/<Configuration>/<CPU>/mappServices/**/*.mpalarmxhistory`
  - `Physical/<Configuration>/<CPU>/mappServices/**/*.mpcomgroup`
  - matching `Package.pkg` files
- Do not edit `Logical/Libraries/MpAlarmX/*`.

## 2) Project pattern to follow

AlarmX is structured like this:

```text
mappServices/
  Package.pkg (SubType="mappServices" PackageType="mappServices")
  MainGroup.mpcomgroup
  MachineMain/
    MainAlarm.mpalarmxcore
    MainAlarmL.mpalarmxlist
    MainHist.mpalarmxhistory
    ... optional subpackages with additional lists
  Auxiliary/
    ... local groups + local cores + local lists
```

If you add/remove/rename files or folders, always update the related `Package.pkg`.

## 3) Package.pkg rules

- Root `mappServices/Package.pkg` must include:
  - `SubType="mappServices"`
  - `PackageType="mappServices"`
- Subpackages use plain `<Package ...>` (no subtype).
- Keep `TransferedForTargetSourceCompare="true"` for `.mpalarmxlist` entries (project convention in both repos).

Example:

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
    <Object Type="File">MainAlarm.mpalarmxcore</Object>
    <Object Type="File" TransferedForTargetSourceCompare="true">MainAlarmL.mpalarmxlist</Object>
    <Object Type="File">MainHist.mpalarmxhistory</Object>
  </Objects>
</Package>
```

## 4) Build the hierarchy first (MpComGroup)

- Root group example: `gGroupMain`.
- Child groups use `mapp.Gen/Parent`.
- Core and History components reference a parent group with `mapp.Gen/Parent`.

Root example:

```xml
<Configuration>
  <Element ID="gGroupMain" Type="mpcomgroup">
    <Group ID="Linking">
      <Group ID="Subnodes">
        <Property ID="0" Value="gAlarmXCore" />
        <Property ID="1" Value="gAlarmXHistory" />
      </Group>
    </Group>
    <Selector ID="Alarms" Value="MpAlarmX" />
  </Element>
</Configuration>
```

Child example:

```xml
<Configuration>
  <Element ID="gUnitGroup" Type="mpcomgroup">
    <Group ID="mapp.Gen">
      <Property ID="Parent" Value="gGroupMain" />
    </Group>
  </Element>
</Configuration>
```

## 5) MpAlarmXCore minimal requirements

- `mapp.Gen`: `Enable`, `EnableCockpit`, `TaskClass`, `Parent`.
- `mapp.AlarmX.List`: at least one linked list.
- `mapp.AlarmX.Create`: usually `Disabled`.
- `mapp.AlarmX.Core.Retain/Memory`: usually `0` in this project.

Minimal core:

```xml
<Configuration>
  <Element ID="gAlarmXCore" Type="mpalarmxcore">
    <Group ID="mapp.Gen">
      <Property ID="Enable" Value="TRUE" />
      <Property ID="EnableCockpit" Value="TRUE" />
      <Property ID="TaskClass" Value="4" />
      <Property ID="Parent" Value="gGroupMain" />
    </Group>
    <Group ID="mapp.AlarmX.List">
      <Group ID="[0]">
        <Property ID="List" Value="gAlarmXCoreList" />
        <Property ID="Offset" Value="0" />
      </Group>
    </Group>
    <Selector ID="mapp.AlarmX.Create" Value="Disabled" />
    <Group ID="mapp.AlarmX.Core.Retain">
      <Selector ID="Memory" Value="0" />
    </Group>
  </Element>
</Configuration>
```

Mapping rules (optional but common):

- Alarm filter can be `AlarmName` or `[Severity]`.
- Used in this project: `Reaction`, `EscalateReaction`, `Escalate`, `Aggregate`.
- Typical pattern for severity-based escalation:
  - `[99] -> EscalateReaction`
  - `[99] -> Reaction`
  - `[99] or [1] -> Escalate`

Reuse rules:

- You can link one list multiple times with different `Offset` and `Argument1..9`.
- Token replacement is done with `{1}`..`{9}` inside list strings.

## 6) MpAlarmXList minimal requirements

- Add alarms under `mapp.AlarmX.Core.Configuration`.
- Each alarm should define at least:
  - `Name`, `Message`, `Code`, `Severity`, `Behavior`
- Keep behavior fields explicit (`Confirm`, `ReactionWhilePending`, `Retain`, `Async`, `Recording`).
- Keep `Disable`, `InhibitPV`, `AdditionalInformation1/2` fields present.

Minimal alarm entry:

```xml
<Group ID="[0]">
  <Property ID="Name" Value="DefaultAlarm" />
  <Property ID="Message" Value="{$Framework/DefaultAlarm}" />
  <Property ID="Code" Value="0" />
  <Property ID="Severity" Value="1" />
  <Selector ID="Behavior" Value="EdgeAlarm">
    <Property ID="Confirm" Value="0" />
    <Property ID="ReactionWhilePending" Value="TRUE" />
    <Property ID="Retain" Value="FALSE" />
    <Property ID="Async" Value="FALSE" />
    <Group ID="Recording">
      <Property ID="InactiveToActive" Value="TRUE" />
      <Property ID="UnacknowledgedToAcknowledged" Value="TRUE" />
      <Property ID="UnconfirmedToConfirmed" Value="TRUE" />
    </Group>
  </Selector>
  <Property ID="Disable" Value="FALSE" />
</Group>
```

Snippets:

- Use `mapp.AlarmX.Snippets` + `AutoDetect`.
- Snippet key in message: `{&Key}`.
- PV snippets use global path format like `::gAlarmInfoFramework.TaskName`.

## 7) MpAlarmXHistory (when history/export is needed)

- Use one history component for the branch you want to record.
- Typical project settings:
  - `Enable=TRUE`, `EnableCockpit=TRUE`, `Audit=TRUE`
  - `Memory=0` (UserROM+DRAM)
  - `Scope=1` (Branch)

## 8) Structured Text wiring (required)

Declaration:

```iec-st
VAR
    fbMpAlarmXCore : MpAlarmXCore;
    fbMpAlarmXHistory : MpAlarmXHistory;
END_VAR
```

Initialization and cyclic call:

```iec-st
PROGRAM _INIT
    fbMpAlarmXCore.MpLink := ADR(gAlarmXCore);
    fbMpAlarmXHistory.MpLink := ADR(gAlarmXHistory);
END_PROGRAM

PROGRAM _CYCLIC
    fbMpAlarmXCore(Enable := TRUE);
    fbMpAlarmXHistory(Enable := TRUE);
END_PROGRAM
```

Set/reset/check:

```iec-st
IF faultCondition THEN
    MpAlarmXSet(gAlarmXCore, 'MyAlarm');
ELSE
    MpAlarmXReset(gAlarmXCore, 'MyAlarm');
END_IF;

IF MpAlarmXCheckReaction(gAlarmXCore, 'ErrorStopHardware') THEN
    (* execute reaction in machine logic *)
END_IF;
```

Notes:

- `MpAlarmXSet` returns `InstanceID`.
- `MpAlarmXReset` returns a result code.
- Use the same core MpLink for set/reset/check.

## 9) Behavior guidance

- Use `EdgeAlarm` for event-like alarms.
- Use `PersistentAlarm` for state/condition alarms.
- Confirmation order: alarm must be acknowledged before confirmed.
- Alarm reaches `None` after reset + acknowledge + confirm (if confirm is required).
- Use asynchronous alarms (`Async=TRUE`) for non-time-critical alarms to improve performance.

## 10) If XML details are uncertain

Do not invent unknown XML groups/properties.

Use one of these safe approaches:

1. Verify XMl structure by asking the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) for the relevant file type and comparing with already existing mappAlarmX XMLs in the project.
2. Verify by building with the as-compile skill and checking the compiler output for configuration errors.

## 11) B&R Automation Studio Help Server pages to consult

Use the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools) for parameter-level confirmation:

- MpAlarmXCore configuration
- MpAlarmXHistory configuration
- MpComGroup configuration
- AlarmX configuration package

Useful pages by title:

- `MpAlarmXList configuration`
- `Application alarm`
- `Adding and configuring function blocks`
- `Reaction`
- `Escalate reaction`

## 12) Final validation checklist

- Every new object is listed in the correct `Package.pkg`.
- All MpLinks (`Element ID` values) are unique and referenced correctly.
- Core list links are valid (`List`, `Offset`, `Argument`).
- ST code assigns MpLink with `ADR(...)` and calls function blocks cyclically.
- Build succeeds without configuration/link errors.