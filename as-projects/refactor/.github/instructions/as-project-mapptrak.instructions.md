---
description: Guidelines for configuring ACOPOStrak assembly and layout.
applyTo: '**/*.st,**/*.var,**/*.typ,**/*.assembly,**/*.assemblyfeature,**/*.processpoint,**/*.sector,**/*.shuttlestereotype'

---

# B&R Automation Studio mapptrak configuration file guidelines

Use this when creating or changing ACOPOStrak tasks and related mapptrak configuration in Automation Studio 6.  
**Goal:** build consistent mapptrak configuration.

## General
- Only generate objects and configuration elements that are required for the specific assembly and layout.

## Scope

- Applies to mappTrak configuration files under `Physical/<Configuration>/<CPU>/mappMotion/` and ST code that references them:

  - `Physical/<Configuration>/<CPU>/mappMotion/**/*.assembly`
  - `Physical/<Configuration>/<CPU>/mappMotion/**/*.assemblyfeature`
  - `Physical/<Configuration>/<CPU>/mappMotion/**/*.processpoint`
  - `Physical/<Configuration>/<CPU>/mappMotion/**/*.sector`
  - `Physical/<Configuration>/<CPU>/mappMotion/**/*.shuttlestereotype`
  - matching `Package.pkg` files

## Project Pattern to Follow for Configuration Files

mappTrak is structured like this:

```text
  Package.pkg
  *.assembly
  *.assemblyfeature
  *.processpoint
  *.sector
  *.shuttlestereotype
  ... optional subpackages with additional configuration files
```

If you add, remove, or rename files or folders, always update the related `Package.pkg`.

## Hardware Segments
- Each segment must be defined in the hardware configuration file (*.hw) and referenced in the assembly configuration file (*.assembly).
- Each segment must have a unique name and ID. The segment name is used in the assembly configuration file to reference the segment.
- Each segment type name is defined with A and B.
  - AA: straight in, straight out
  - AB: straight in, curved out
  - BA: curved in, straight out
  - BB: curved in, curved out
  - AA8C: compact 180° curve
- Segments must be connected in the correct order. Any other combination of segment types is invalid and will result in a configuration error. 
  - example:AA --> AB --> BB --> BA --> AA
  - example:AA --> AA8C --> AA --> AA8C
  - straight end to straight end, curve end to curve end
  - compact curve can be connected to a straight end
- Each segment type has a different length.
  - AA: 0.66m
  - AB and BA: 0.450642056m
  - BB: 0.24m
  - AA8C: 0.585m

## Package.pkg Rules

- Root `mappMotion/Package.pkg` must include:
  - `<Object Type="File">*.assembly</Object>` for each assembly file
  - `<Object Type="File">*.sector</Object>` for each sector file
  - `<Object Type="File">*.shuttlestereotype</Object>` for each shuttle stereotype file
  - `<Object Type="File">*.assemblyfeature</Object>` for each assembly feature file (optional)
  - `<Object Type="File">*.processpoint</Object>` for each process point file (optional)

Example:

```xml
<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<Package xmlns="http://br-automation.co.at/AS/Package">
  <Objects>
    <Object Type="File">Trak_Assem.assembly</Object>
    <Object Type="File">Trak_Shutt.shuttlestereotype</Object>
    <Object Type="File">Trak_Secto.sector</Object>
    <Object Type="File">Trak_Proce.processpoint</Object>
    <Object Type="File">Trak_Ass_1.assemblyfeature</Object>
  </Objects>
</Package>
```

# Assembly Configuration File Structure (*.assembly)

## File Naming
- Each `*.assembly` file must have a unique name, typically aligned with its configuration.  
  - Example: `Trak_Assem.assembly`

## Assembly Definition
- Each assembly file contains one or more assembly elements with unique IDs.
- Naming pattern: `gAssembly_1`, `gAssembly_2`, etc. or `gAssembly_<FunctionalName>`
- Keep these core Group IDs unchanged: `Tracks`, `CommonSegmentSettings`, `Shuttles`, `AssemblyFeatures`, `Visualization`

## Essential Configuration Elements

### Tracks (required, minimum 1)
- **TrackSeparation**: distance between parallel tracks (LREAL, metres; typical: 0.030)
- **Track[n]**: indexed groups defining each track
  - **Segments**: ordered list of segment references (e.g., `::AB_Seg_1_1_1`)
  - **Position**: `Absolute` | `RelativeToOne` | `RelativeToTwo`
    - `Absolute`: independent positioning with Translation (X, Y, Z) and Orientation (Angle1, Angle2, Angle3)
    - `RelativeToOne`: positioned relative to another track
      - <Selector ID="Position" Value="RelativeToOne">
          <Group ID="TrackSegmentPosition">
            <Property ID="SegmentRef" Value="AB_Seg_1_1_1" />
          </Group>
          <Group ID="Base">
            <Property ID="SegmentRef" Value="AB_Seg_1_1_1" />
          </Group>
        </Selector>
    - `RelativeToTwo`: positioned relative to two other tracks
      - <Selector ID="Position" Value="RelativeToTwo">
          <Group ID="AlignmentToFirst">
            <Property ID="SegmentRefNewFirst" Value="AA_Seg_1_2_1" />
            <Property ID="SegmentRefBaseFirst" Value="AA_Seg_1_2_1" />
          </Group>
          <Group ID="AlignmentToSecond">
            <Property ID="SegmentRefNewSecond" Value="AA_Seg_1_2_1" />
            <Property ID="SegmentRefBaseSecond" Value="AA_Seg_1_2_1" />
          </Group>
        </Selector>

```xml
<Group ID="Tracks">
  <Property ID="TrackSeparation" Value="0.030" />
  <Group ID="Track[1]">
    <Group ID="Segments">
      <Property ID="SegmentRef[1]" Value="::AB_Seg_1_1_1" />
      <Property ID="SegmentRef[2]" Value="::BB_Seg_1_1_2" />
    </Group>
    <Selector ID="Position" Value="Absolute">
      <Property ID="SegmentRef" Value="AB_Seg_1_1_1" />
      <Property ID="Position" Value="0.0" />
      <Property ID="PositionRelativeTo" Value="FromStart" />
      <Group ID="Translation">
        <Property ID="X" Value="0" />
        <Property ID="Y" Value="0" />
        <Property ID="Z" Value="0.1975" />
      </Group>
      <Group ID="Orientation">
        <Property ID="Angle1" Value="0.0" />
        <Property ID="Angle2" Value="0.0" />
        <Property ID="Angle3" Value="180" />
      </Group>
    </Selector>
  </Group>
</Group>
```

### Common Segment Settings (optional)

Each property can be omitted — the assembly uses B&R defaults for missing values.

#### Essential Properties

| Property | Type | Allowed Values | Default | Description |
|----------|------|-----------------|---------|-------------|
| `SegmentSimulationOnPLC` | STRING | `On`, `Off` | `Off` | Enable PLCopen simulation on PLC |
| `ScopeOfErrorReaction` | STRING | `Assembly`, `Segment`, `SegmentAdvanced` | `Assembly` | Error scope (reserved for future expansion) |
| `ShuttleIdentificationTime` | REAL | 0.0 - 100.0 | 0.0 | Identification timeout (seconds) |

#### Stop Reaction (Selector)

**Selector ID="StopReaction" Value=**
| Value | Fallback Required | Fallback Options | Behavior |
|-------|-------------------|------------------|----------|
| `InductionHalt` | No | — | ⚡ Coil short-circuit → **instant stop** (safest, recommended) |
| `CoastToStandstill` | No | — | 🛑 Motor off, passive deceleration (⚠️ collision risk, long stop distance) |
| `StandstillManeuver` | **Yes** | `InductionHalt`, `CoastToStandstill` | 📊 Controlled stop via standstill, fallback on failure |

**Example:**
```xml
<!-- Option 1: InductionHalt (fastest, safest) -->
<Selector ID="StopReaction" Value="InductionHalt" />

<!-- Option 2: Coast to standstill (⚠️ not recommended) -->
<Selector ID="StopReaction" Value="CoastToStandstill" />

<!-- Option 3: Controlled with fallback -->
<Selector ID="StopReaction" Value="StandstillManeuver">
  <Property ID="Fallback" Value="InductionHalt" />
</Selector>
```

#### Speed Filter (Selector)

**Selector ID="SpeedFilter" Value=** — Lowpass filter for velocity ramp smoothing

| Value | Properties | Behavior | Notes |
|-------|------------|----------|-------|
| `NotUsed` | — | No filtering | ✅ Default; sharp velocity transitions |
| `Lowpass` | `FilterTime` (REAL) | First-order lowpass (800 μs default) | Smoother acceleration/deceleration; adds latency |

**FilterTime Range & Default:**
- Range: 0.0001 - 1.0 seconds
- B&R Recommended: **0.0008** (800 microseconds)
- Typical value: 0.0016 (1.6 milliseconds)

**Example:**
```xml
<!-- No filtering (default) -->
<Selector ID="SpeedFilter" Value="NotUsed" />

<!-- Enable Lowpass filter with 800 μs (B&R recommended) -->
<Selector ID="SpeedFilter" Value="Lowpass">
  <Property ID="FilterTime" Value="0.0008" />
</Selector>

<!-- Custom: 1.6 ms filter -->
<Selector ID="SpeedFilter" Value="Lowpass">
  <Property ID="FilterTime" Value="0.0016" />
</Selector>
```

#### Controller Parameters (Group, Advanced)

**Default tuning values if omitted.** Only customize if motion is imprecise or oscillating.
- The property can be deleted to use the default B&R compensation.

Example:

```xml
<Group ID="ControllerParameters">
  <Group ID="DefaultParameter">
    <!-- Position Controller (P-control) -->
    <Group ID="Controller">
      <Group ID="Position">
        <Property ID="ProportionalGain" Value="100" />  <!-- Range: 0.1-10000 -->
      </Group>
      <!-- Speed Controller (PI-control) -->
      <Group ID="Speed">
        <Property ID="ProportionalGain" Value="10" />   <!-- Range: 0.01-1000 -->
        <Property ID="IntegrationTime" Value="1" />     <!-- Range: 0.001-100, seconds -->
      </Group>
      <!-- Feed-forward compensation -->
      <Group ID="FeedForward">
        <Property ID="TotalMass" Value="1" />           <!-- Virtual inertia, Range: 0.001-1000 -->
        <Property ID="ForcePositive" Value="1" />       <!-- +force bias, Range: -1000 to 1000 -->
        <Property ID="ForceNegative" Value="1" />       <!-- -force bias, Range: -1000 to 1000 -->
        <Property ID="SpeedForceFactor" Value="1" />    <!-- Velocity-dependent force, Range: 0-1000 -->
        <Property ID="ForceLoad" Value="1" />           <!-- Load-dependent force, Range: -1000 to 1000 -->
      </Group>
    </Group>
    <!-- Force Limits -->
    <Selector ID="ForceLimits" Value="ControlledForce">
      <Property ID="ForcePositive" Value="1" />   <!-- +force limit, Range: -1000 to 1000 -->
      <Property ID="ForceNegative" Value="1" />   <!-- -force limit, Range: -1000 to 1000 -->
    </Selector>
    <!-- Error Detection Thresholds -->
    <Group ID="MovementErrorLimits">
      <Property ID="PositionError" Value="0.005" /> <!-- Range: 0-1, metres; alarms if exceeded -->
      <Property ID="VelocityError" Value="1" />     <!-- Range: 0-1000, m/s; alarms if exceeded -->
    </Group>
    <!-- Diverter Override (if present) -->
    <Group ID="Diverter">
      <Property ID="ForceOverride" Value="2" />     <!-- Range: 0.5-10 (multiplier) -->
    </Group>
  </Group>
</Group>
```

**Common Adjustments:**
- **Oscillation/ringing**: Reduce `Position/ProportionalGain` or increase `Speed/IntegrationTime`
- **Too slow to target**: Increase `Position/ProportionalGain`
- **Jerky motion**: Increase `TotalMass` (virtual inertia)

#### Compensation (Group, Advanced)

- **Cogging Compensation**: reduces low-speed oscillation in motors with cogging torque.  
  - Only active below `UpperVelLimit` (m/s).  
  - The property can be set to Inactive to disable compensation entirely.
  - The property can be deleted to use the default B&R compensation.

```xml
<Group ID="Compensation">
  <Selector ID="CoggingCompensation" Value="Active">
    <Property ID="UpperVelLimit" Value="1.3" /> <!-- Range: 0.01-4.0 m/s; only active below this speed -->
  </Selector>
</Group>
```

#### Position Lag Monitor (Selector)

- **PositionControllerLagMonitor**: monitors the lag between commanded and actual position.  
  - If the lag exceeds `WarningLimit` (metres), a warning is raised.  
  - The property can be deleted to disable the monitor entirely.

```xml
<Selector ID="PositionControllerLagMonitor" Value="Active">
  <Property ID="WarningLimit" Value="0.005" /> <!-- Range: 0-1, metres; alarm if lag exceeds -->
</Selector>
```

#### Diverter Overrides (Group, Advanced)

- The property can be deleted to use the default B&R compensation.

```xml
<Group ID="Diverter">
  <Property ID="DiverterWeakEndOverrideFactor" Value="1.3" /> <!-- Range: 0.5-10 -->
  <Property ID="DiverterStrenEndOverrideFactor" Value="1.3" /> <!-- Range: 0.5-10 -->
</Group>
```

### Shuttles (required, minimum 1)

Each property can be omitted — the assembly uses B&R defaults for missing values.

#### Core Configuration

| Property | Type | Allowed Values | Default | Description |
|----------|------|-----------------|---------|-------------|
| `MaxShuttleCount` | UINT | 1 - 1000 | 100 | Maximum number of shuttles in assembly (must be > 0) |
| `MaxShuttleCommandCount` | UINT | 0 - 1000 | 0 | Reserved command buffer size (0 = unlimited) |

**⚠️ IMPORTANT:** When using `InitShuttleSim` assembly feature, `MaxShuttleCount` **MUST** be set explicitly to at least the number of shuttles defined in the simulation. If omitted or set to 0, shuttles will not be created. Example: If `InitShuttleSim` defines 3 shuttles, set `MaxShuttleCount` to at least `3`.

#### Convoy & Sector Switching

| Property | Type | Allowed Values | Default | Description |
|----------|------|-----------------|---------|-------------|
| `UseConvoys` | STRING | `Active`, `Inactive` | `Inactive` | Group shuttles for synchronized movement |
| `SwitchSectorAfterStop` | STRING | `SwitchOntoSegmentSector`, `Autoattach`, `RemainOnInternalSector` | `SwitchOntoSegmentSector` | Sector behavior after stop commands |

- For Autoattach following parameter can be set
  - A Fallback can be set, if not set, the default is used
  - <Selector ID="SwitchSectorAfterStop" Value="Autoattach">
      <Property ID="Fallback" Value="Is" />
    </Selector>
- For RemainOnInternalSector following parameter can be set
  - A Scope can be set, if not set, the default is used
  - <Selector ID="SwitchSectorAfterStop" Value="RemainOnInternalSector">
      <Property ID="Scope" Value="All" />
    </Selector>


#### Distance Reserve (Safety Margins)

These are the mandatory safety distances maintained between shuttles and obstacles.

```xml
<Group ID="DistanceReserve">
  <Property ID="Collision" Value="0.002" />    <!-- Range: 0.0-1.0 metres -->
  <Property ID="ErrorStop" Value="0.006" />    <!-- Range: 0.0-1.0 metres -->
  <Property ID="ConflictZone" Value="0.0" />   <!-- Range: 0.0-1.0 metres -->
</Group>
```

| Property | Type | Range | Typical | Purpose |
|----------|------|-------|---------|---------|
| `Collision` | LREAL | 0.0 - 1.0 m | 0.002 m | Buffer between shuttles during normal operation |
| `ErrorStop` | LREAL | 0.0 - 1.0 m | 0.006 m | Safety distance on emergency stop |
| `ConflictZone` | LREAL | 0.0 - 1.0 m | 0.0 m | Protective zone around obstacles |

#### Shuttle Stereotypes (required, minimum 1)

Reference to shuttle type definitions. Must match IDs in `*.shuttlestereotype` file.
An assembly may reference exactly one stereotype. The `ShuttleStRef[1]` index is required syntax; do not add `ShuttleStRef[2]` or other entries. Multiple stereotype elements may be defined in the `*.shuttlestereotype` file for use by different assemblies.

```xml
<Group ID="ShuttleStereotypes">
  <Property ID="ShuttleStRef[1]" Value="ShuttleStereotype_1" />
</Group>
```

#### Magnet Plate Configurations

Hardware shuttle model definitions.
Only one MagnetPlate can be configured.

Valid Values:
- `8F1Sx.100.xxxxxx-x` — 5 magnet poles on both sides of the shuttle (standard and legacy)
- `8F1Sx.102.xxxxxx-x` — 5 magnet poles on one side of the shuttle (standard and legacy)
- `8F1SM.102.xxxxxx-x` — 5 magnet poles on one side of the shuttle (Metal-to-Metal)
- `8F1SA.104.xxxxxx-x` — 5 magnet poles skewed on both sides of the shuttle (legacy)
- `8F1SA.106.xxxxxx-x` — 5 magnet poles skewed on one side of the shuttle (legacy)
- `8F1SA.201.xxxxxx-x` — 10 magnet poles on both sides of the shuttle (legacy)
- `8F1SC.201.xxxxxx-x` — 10 magnet poles on both sides of the shuttle (standard)
- `8F1SA.203.xxxxxx-x` — 10 magnet poles on one side of the shuttle (legacy)
- `8F1SC.203.xxxxxx-x` — 10 magnet poles on one side of the shuttle (standard)
- `8F1SB.308.xxxxxx-x` — 21 magnet poles on one side of the shuttle (standard)

```xml
<Group ID="MagnetPlateConfigurations">
  <Property ID="ShuttleType[1]" Value="8F1Sx.100.xxxxxx-x" />
</Group>
```

#### Collision Avoidance (optional)

Advanced safety settings for shuttle-to-shuttle and shuttle-to-obstacle avoidance.

```xml
<Group ID="CollisionAvoidance">
  <Selector ID="Strategy" Value="AdvancedVariable">
    <Property ID="Reserved" Value="1" />
  </Selector>
  <Selector ID="AdjustmentMode" Value="Dense" />
  <Group ID="MaximumModelDimensions">
    <Group ID="Length">
      <Property ID="ExtentToFront" Value="0.05" />
      <Property ID="ExtentToBack" Value="0.05" />
    </Group>
    <Group ID="Width">
      <Property ID="Width" Value="0.046" />
    </Group>
  </Group>
  <Property ID="VirtualShColAvoidanceScope" Value="AllShuttles" />
</Group>
```

**Collision Avoidance Parameters:**

- If not set, the assembly uses B&R defaults for missing values.
- In the default configuration, no property is set.

| Property | Type | Allowed Values | Description |
|----------|------|-----------------|---------|-------------|
| `Strategy` | STRING | `AdvancedVariable`, `Variable`, `AdvancedConstant` | |
| `AdjustmentMode` | STRING | `Dense` | `Dense` = tighter spacing during maneuvers; |
| `VirtualShColAvoidanceScope` | STRING | `NoShuttles`, `NonVirtualShuttles` | Apply CA to virtual shuttles in simulation |

**Maximum Model Dimensions (Shuttle Bounding Box):**

| Property | Type | Range | Unit | Purpose |
|----------|------|-------|------|---------|
| `Length/ExtentToFront` | LREAL | 0.001 - 1.0 | metres | Distance from shuttle center to front |
| `Length/ExtentToBack` | LREAL | 0.001 - 1.0 | metres | Distance from shuttle center to back |
| `Width/Width` | LREAL | 0.001 - 1.0 | metres | Shuttle lateral width |

**⚠️ Important:**
- These dimensions are used **only** for collision detection
- Must reflect actual shuttle footprint (including tolerance)
- Assembly defaults are typically sufficient; override only if custom shuttles used

#### Coupling (optional)

Shuttle-to-shuttle coupling settings for synchronized movement groups.
Without the group, the EliminateShuttleToShuttleDelay feature is disabled.

```xml
<Group ID="Coupling">
  <Group ID="CouplingFeature">
    <Property ID="EliminateShuttleToShuttleDelay" Value="TRUE" />
  </Group>
</Group>
```

### Assembly Features & Visualization
- **AssemblyFeatures**: reference to feature IDs (e.g., `Snapshot`, `Limits_1`)
- **Visualization**:
  - `MonitoringPv`: global variable reference (STRING)
  - `ProcessingTaskClass`: task class (USINT, typically 4)

## Minimal Assembly Example
```xml
<Element ID="gAssembly_1" Type="assembly">
  <Group ID="Tracks">
    <Property ID="TrackSeparation" Value="0.030" />
    <Group ID="Track[1]">
      <Group ID="Segments">
        <Property ID="SegmentRef[1]" Value="::AB_Seg_1_1_1" />
        <Property ID="SegmentRef[2]" Value="::BB_Seg_1_1_2" />
      </Group>
      <Selector ID="Position" Value="Absolute">
        <Property ID="SegmentRef" Value="AB_Seg_1_1_1" />
        <Property ID="Position" Value="0.0" />
        <Property ID="PositionRelativeTo" Value="FromStart" />
        <Group ID="Translation">
          <Property ID="X" Value="0" />
          <Property ID="Y" Value="0" />
          <Property ID="Z" Value="0.0" />
        </Group>
        <Group ID="Orientation">
          <Property ID="Angle1" Value="0.0" />
          <Property ID="Angle2" Value="0.0" />
          <Property ID="Angle3" Value="0.0" />
        </Group>
      </Selector>
    </Group>
  </Group>
  <Group ID="Shuttles">
    <Property ID="MaxShuttleCount" Value="100" />
    <Group ID="ShuttleStereotypes">
      <Property ID="ShuttleStRef[1]" Value="ShuttleStereotype_1" />
    </Group>
  </Group>
</Element>
```

## Guidelines
- Segment references must match hardware definition in `.hw` file
- Multiple tracks require different Track[n] groups with consistent positioning
- Always reference existing shuttle stereotypes and features by ID
- Use descriptive assembly IDs for clarity in multi-assembly projects


# Assembly Feature configuration file structure (*.assemblyfeature)

## File Naming
- Each `*.assemblyfeature` file must have a unique name, typically aligned with its assembly.  
  - Example: `Trak_Ass_1.assemblyfeature`

## Assembly Feature Definition
- Each assemblyfeature file contains one or more feature elements with unique IDs.
- Keep these core IDs unchanged: `Type`, and type-specific groups like `CouplingObject`, `AssemblySnapshot`, `SectorTraceCfg`, `SegmentGroup`

## Feature Types (Selector ID="Type")

### AssemblySnapshot — Record assembly state
- **Use case**: Capture shuttle positions and system state at runtime
- **Properties**:
  - `RecordsBufferSize`: max entries (UDINT, typical: 300)
  - `StopDelay`: delay before recording (UDINT ms, typical: 100)

```xml
<Element ID="Snapshot" Type="assemblyfeature">
  <Selector ID="Type" Value="AssemblySnapshot">
    <Group ID="AssemblySnapshot">
      <Property ID="RecordsBufferSize" Value="300" />
      <Property ID="StopDelay" Value="100" />
    </Group>
  </Selector>
</Element>
```

### SectorTraceCfg — Trace sector activity
- **Use case**: Monitor all shuttle activities within a specific sector
- **Properties**:
  - `Sector`: sector reference (STRING)
  - `SectorRestricted`: `on` | `off` — limit to this sector only
  - `ShuttleCount`: reserved capacity (UINT)

```xml
<Element ID="SectorTrace" Type="assemblyfeature">
  <Selector ID="Type" Value="SectorTraceCfg">
    <Group ID="SectorTraceCfg">
      <Property ID="ShuttleCount" Value="0" />
      <Property ID="Sector" Value="BaseSector_1_1" />
      <Property ID="SectorRestricted" Value="on" />
    </Group>
  </Selector>
</Element>
```

### InitShuttleSim — Initialize simulation shuttles
- **Use case**: Pre-populate shuttles for testing and simulation
- **Do not use when shuttles are generated by the PLC application at runtime (e.g., via MC_BR_SecAddShuttle_AcpTrak).**
- **Properties**:
  - `SectorRef`: sector where shuttles start (STRING)
  - `Orientation`: `Positive` | `Negative` | `KeepSegment`
  - `ShuttleDefinition`: `SingleShuttleDefinition` | `ShuttleSetDefinition`
    - Single: `Position[1]` (LREAL)
    - Set: `StartPosition`, `NumberOfShuttles`, `DistnaceBetweenShuttles`

```xml
<Element ID="InitShuttles" Type="assemblyfeature">
  <Selector ID="Type" Value="InitShuttleSim">
    <Group ID="InitShuttleSim[1]">
      <Property ID="SectorRef" Value="BaseSector_1_1" />
      <Property ID="Orientation" Value="Positive" />
      <Selector ID="ShuttleDefinition" Value="SingleShuttleDefinition">
        <Property ID="Position[1]" Value="0.0" />
      </Selector>
    </Group>
  </Selector>
</Element>
```

### ShtSectorAutoAttach — Auto-attach shuttle sector
- **Use case**: Define sector attachment priority for shuttles entering tracks
- **Properties**:
  - `Priority[n]`: ordered priority groups
    - `SectorRef`: target sector (STRING)
    - `Orientation`: `KeepSegment` | `Positive` | `Negative`

```xml
<Element ID="AutoAttach" Type="assemblyfeature">
  <Selector ID="Type" Value="ShtSectorAutoAttach">
    <Group ID="Priority[1]">
      <Property ID="SectorRef" Value="BaseSector_1_1" />
      <Property ID="Orientation" Value="KeepSegment" />
    </Group>
  </Selector>
</Element>
```

### Coupling — Couple multiple shuttles
- **Use case**: Group shuttles together for synchronized movement
- **Properties**:
  - `Name`: coupling identifier (STRING)
  - `ShuttleCount`: capacity (UINT)
  - `CamAutomat`, `CamList`: optional automation data

```xml
<Element ID="Coupling_1" Type="assemblyfeature">
  <Selector ID="Type" Value="Coupling">
    <Group ID="CouplingObject[1]">
      <Property ID="Name" Value="myCoupling" />
      <Property ID="ShuttleCount" Value="0" />
      <Property ID="CamAutomat" />
      <Property ID="CamList" />
    </Group>
  </Selector>
</Element>
```

### LocalLimits — Restrict motion in zones
- **Use case**: Apply velocity/acceleration limits to specific zones or sectors
- **Properties**:
  - `Scope`: `AllShuttles` | `SingleShuttles`
  - `Location`: `Sector` | `CurvedArea` | `DualTrackArea`
  - `MotionLimits`: contains `Velocity` and `Acceleration` selectors
    - Each: `Basic` (with value), `NotUsed`, or `Inactive`

```xml
<Element ID="Limits_1" Type="assemblyfeature">
  <Selector ID="Type" Value="LocalLimits">
    <Group ID="LocalLimit[1]">
        <Selector ID="Location">
          <Property ID="SectorRef" Value="BaseSector_1_1" />
        </Selector>
        <Group ID="MotionLimits">
          <Selector ID="Velocity" Value="Basic" />
          <Selector ID="Acceleration" Value="Basic" />
        </Group>
      </Group>
      <Group ID="LocalLimit[2]">
        <Selector ID="Scope" Value="SingleShuttles" />
        <Selector ID="Location" Value="CurvedArea" />
      </Group>
      <Group ID="LocalLimit[3]">
        <Selector ID="Scope" Value="SingleShuttles">
          <Property ID="LocalLimitName" Value="myLocalLimit1" />
        </Selector>
        <Selector ID="Location" Value="DualTrackArea" />
      </Group>
  </Selector>
</Element>
```

### SegmentGroup — Logical segment grouping
- **Use case**: Bundle related segments for configuration and monitoring
- **Properties**:
  - `Name`: group identifier (STRING)
  - `SegmentRef[n]`: segment references within the group

```xml
<Element ID="SegGroup_1" Type="assemblyfeature">
  <Selector ID="Type" Value="SegmentGroup">
    <Group ID="SegmentGroup[1]">
      <Property ID="Name" Value="LoadingZone" />
      <Group ID="Segments">
        <Property ID="SegmentRef[1]" Value="::AB_Seg_1_1_1" />
        <Property ID="SegmentRef[2]" Value="::BB_Seg_1_1_2" />
      </Group>
    </Group>
  </Selector>
</Element>
```

### ExcludeFromAssembly — Exclude segments
- **Use case**: Deactivate specific segments during assembly power-on or operation
- **Properties**:
  - `Exclusion` type: `AsmPowerOn` (on assembly startup) or others
  - `SegmentRef[n]`: list of segments to exclude

```xml
<Element ID="Exclude_1" Type="assemblyfeature">
  <Selector ID="Type" Value="ExcludeFromAssembly">
    <Selector ID="Exclusion" Value="AsmPowerOn">
      <Property ID="SegmentRef[1]" Value="::BB_Seg_1_1_3" />
    </Selector>
  </Selector>
</Element>
```

### ShuttleShapeRegistration — Define shuttle dimensions
- **Use case**: Specify shuttle physical dimensions (overrides assembly defaults)
- **Properties**:
  - `Set[n]`: dimension set
    - `Length`: `ExtentToFront`, `ExtentToBack` (LREAL, metres)
    - `Width`: `Width` (LREAL, metres)

```xml
<Element ID="ShapeReg_1" Type="assemblyfeature">
  <Selector ID="Type" Value="ShuttleShapeRegistration">
    <Group ID="Set[1]">
      <Group ID="Length">
        <Property ID="ExtentToFront" Value="0.025" />
        <Property ID="ExtentToBack" Value="0.025" />
      </Group>
      <Group ID="Width">
        <Property ID="Width" Value="0.046" />
      </Group>
    </Group>
  </Selector>
</Element>
```

## Guidelines
- Reference features in the assembly configuration file `AssemblyFeatures` group by their ID.
- All feature IDs within a file must be unique.
- Use descriptive ID names that reflect the feature's purpose (e.g., `Snapshot`, `Limits_Loading`, `Group_Zone1`).
- When referencing sectors or segments, use the full path (e.g., `::BaseSector_1_1`, `::AB_Seg_1_1_1`).
- Multiple feature types can coexist in the same file.



# Sector configuration file structure (*.sector)

## Purpose
- Defines contiguous paths (sectors) composed of segments within an assembly.
- Shuttles can be attached to sectors for movement and control.

## File Naming
- Each `*.sector` file must have a unique name, typically aligned with its assembly.  
  - Example: `Trak_Secto.sector`

## Sector Definition
- Each sector file contains one or more sector elements with unique IDs.
- A sector defines a contiguous path composed of segments within an assembly.
- Keep these core IDs unchanged: `Type`, `StartSegment`, `IntermediateSegments`, `EndSegment`, `SegmentRef`, `Position`, `PositionRelativeTo`

## Structure (Selector ID="Type" = "Composed")

- **StartSegment**: first segment in the sector
  - `SegmentRef`: reference to starting segment (STRING)
  - `Position`: distance along segment start (LREAL, metres)
  - `PositionRelativeTo`: `FromStart` | `FromEnd`

- **IntermediateSegments**: zero or more mid-sector segments
  - `SegmentRef[n]`: ordered segment references
  - These segments are fully included (no position offset)

- **EndSegment**: final segment in the sector
  - `SegmentRef`: reference to ending segment (STRING)
  - `Position`: distance along segment end (LREAL, metres)
  - `PositionRelativeTo`: `FromStart` | `FromEnd`

## Examples

### Simple Sector (single segment)
```xml
<Element ID="BaseSector_1_1" Type="sector">
  <Selector ID="Type" Value="Composed">
    <Group ID="StartSegment">
      <Property ID="SegmentRef" Value="::AB_Seg_1_1_1" />
      <Property ID="Position" Value="0" />
      <Property ID="PositionRelativeTo" Value="FromStart" />
    </Group>
    <Group ID="EndSegment">
      <Property ID="SegmentRef" Value="::AB_Seg_1_1_1" />
			<Property ID="Position" Value="0.450642056" />
      <Property ID="PositionRelativeTo" Value="FromStart" />
    </Group>
  </Selector>
</Element>
```

### Composed Sector (with intermediates)
```xml
<Element ID="BaseSector_1_1" Type="sector">
  <Selector ID="Type" Value="Composed">
    <Group ID="StartSegment">
      <Property ID="SegmentRef" Value="::AB_Seg_1_1_1" />
      <Property ID="Position" Value="0" />
      <Property ID="PositionRelativeTo" Value="FromStart" />
    </Group>
    <Group ID="IntermediateSegments">
      <Property ID="SegmentRef[1]" Value="::BB_Seg_1_1_2" />
      <Property ID="SegmentRef[2]" Value="::BB_Seg_1_1_3" />
      <Property ID="SegmentRef[3]" Value="::BA_Seg_1_1_4" />
    </Group>
    <Group ID="EndSegment">
      <Property ID="SegmentRef" Value="::AB_Seg_1_1_5" />
      <Property ID="Position" Value="0" />
      <Property ID="PositionRelativeTo" Value="FromEnd" />
    </Group>
  </Selector>
</Element>
```

## Structure (Selector ID="Type" = "Subsector")

A **Subsector** defines a partial region within an existing (parent) Sector. Instead of defining segments, you reference a parent sector and specify the start position and length.

#### Core Parameters

| Parameter | Type | Range | Unit | Required | Description |
|-----------|------|-------|------|----------|-------------|
| `SectorRef` | STRING | — | — | **Yes** | Reference to parent sector ID (e.g., `BaseSector_1_1`) |
| `Position` (in `Start`) | LREAL | 0.0 - Parent Length | metres | **Yes** | Start position within parent sector (0 = beginning) |
| `Length` | LREAL | 0.001 - Parent Length | metres | **Yes** | Length of subsector; must fit within parent |

#### Validation Constraints

| Constraint | Validation | Error |
|-----------|-----------|-------|
| `Position + Length ≤ Parent Sector Length` | Subsector must fit within parent | Configuration rejected |
| `Position ≥ 0.0` | Start position must be ≥ 0 | Configuration rejected |
| `Length > 0.0` | Length must be positive (min: 0.001) | Configuration rejected |
| `SectorRef` valid | Parent sector must exist | Configuration rejected |

#### Syntax

```xml
<Element ID="SubSector_LoadingZone" Type="sector">
  <Selector ID="Type" Value="Subsector">
    <Property ID="SectorRef" Value="BaseSector_1_1" />
    <Group ID="Start">
      <Property ID="Position" Value="0.1" />
    </Group>
    <Property ID="Length" Value="0.8" />
  </Selector>
</Element>
```

**Interpretation:**
- `SectorRef="BaseSector_1_1"`: Parent is `BaseSector_1_1`
- `Position=0.1`: Subsector starts 0.1 m from parent start
- `Length=0.8`: Subsector is 0.8 m long
- **Effective range:** 0.1 m to 0.9 m within `BaseSector_1_1`

#### Guidelines

- **Parent must exist**: `SectorRef` must reference an existing `Composed` or `Subsector` sector
- **Bounds checking**: `Position + Length` must not exceed parent sector length
- **Nested subsectors**: A Subsector can reference another Subsector as parent (chains are allowed)
- **Overlapping allowed**: Multiple subsectors can cover the same physical area (useful for multi-zone logic)
- **Naming convention**: Use descriptive IDs (e.g., `SubSector_Loading_Zone`, `SubSector_Buffer_Area`)
- **Process points**: Reference subsector ID when defining process points within zones


# Shuttle stereotype configuration file structure (*.shuttlestereotype)

## File Naming
- Each `*.shuttlestereotype` file must have a unique name, typically aligned with its assembly.  
  - Example: `Trak_Shutt.shuttlestereotype`

## Shuttle Stereotype Definition
- Each stereotypefile contains one or more shuttle stereotype elements with unique IDs.
- Defines physical and operational characteristics of a shuttle type.
- Keep these core IDs unchanged: `MeasurementUnit`, `MeasurementResolution`, `MovementLimits`, `JerkFilter`, `ZeroVibrationFilter`, `UserData`, `CollisionAvoidance`, `StateTransitions`

## Essential Configuration Elements

### Measurement (required)
- **MeasurementUnit**: `5067858` (metres — do not change)
- **MeasurementResolution**: precision (LREAL; typical: 0.00001)

### Movement Limits (required)
- **MovementLimits** — Selector: `Internal` (hardwired limits)
- **Velocity**
  - Value (REAL, m/s; **max: 4.0**)
- **Acceleration**
  - Value (REAL, m/s²; **max: 50.0**, typical: 10-40)
- **Deceleration**
  - Value (REAL, m/s²; **max: 50.0**, typical: 10-40)
  - ⚠️ **Critical for collision avoidance** — determines braking distance
- **UpdateMode**: `Immediately` (accept limits now) | `AvoidError` (skip if causes error stop)

### Jerk Filter (optional)
- **JerkFilter** — Selector: `Used` (with `JerkTime`) | `NotUsed`
  - `JerkTime`: smoothing duration (REAL seconds; typical: 0.02)
  - Reduces acceleration/deceleration shock but adds latency

### Zero Vibration Filter (optional)
- **ZeroVibrationFilter** — Selector: `Used` | `NotUsed`
  - `ZeroVibrationCoefficient`: smoothing factor (REAL; typical: 0.5)
  - `ZeroVibrationTime`: filter time (REAL seconds; typical: 0.02)
  - Prevents oscillation and product sloshing during dynamic moves
  - Uses exponential filter with configurable coefficient (default: 0.5)
	- If both `JerkFilter` and `ZeroVibrationFilter` are present in the same `shuttlestereotype` element, the configuration will be rejected by Automation Studio. Only one may be defined per Element.

### Collision Avoidance (optional)
- **InitialModelDimensions** — Shuttle footprint for collision detection
  - `UseExplicitValues`: explicit dimensions (Length/Width)
    - `ExtentToFront`, `ExtentToBack`: (LREAL, metres)
    - `Width`: (LREAL, metres)
  - `UseMaximumDimensions`: use assembly max dimensions

### State Transitions (optional)
- **AutomaticRecontrol**: `Active` | `Inactive` (auto-recovery after error)

### User Data (optional)
- **Size**: custom data buffer (UINT bytes; typical: 16)

## Minimal Shuttle Stereotype
```xml
<Element ID="ShuttleStereotype_1" Type="shuttlestereotype">
  <Property ID="MeasurementUnit" Value="5067858" />
  <Property ID="MeasurementResolution" Value="0.00001" />
  <Selector ID="MovementLimits">
    <Selector ID="Velocity">
      <Property ID="Velocity" Value="4.0" />
    </Selector>
    <Selector ID="Acceleration">
      <Property ID="Acceleration" Value="40" />
    </Selector>
    <Selector ID="Deceleration">
      <Property ID="Deceleration" Value="40" />
    </Selector>
    <Property ID="UpdateMode" Value="AvoidError" />
  </Selector>
</Element>
```

## Full-Featured Shuttle Stereotype
```xml
<Element ID="ShuttleStereotype_1" Type="shuttlestereotype">
  <Property ID="MeasurementUnit" Value="5067858" />
  <Property ID="MeasurementResolution" Value="0.00001" />
  <Selector ID="MovementLimits" Value="Internal">
    <Property ID="VelocityIsReadOnly" Value="0" />
    <Selector ID="Velocity" Value="Basic">
      <Property ID="Velocity" Value="4.0" />
    </Selector>
    <Property ID="AccelerationIsReadOnly" Value="0" />
    <Selector ID="Acceleration" Value="Basic">
      <Property ID="Acceleration" Value="40" />
    </Selector>
    <Property ID="DecelerationIsReadOnly" Value="0" />
    <Selector ID="Deceleration" Value="Basic">
      <Property ID="Deceleration" Value="40" />
    </Selector>
    <Property ID="UpdateMode" Value="AvoidError" />
  </Selector>
  <Selector ID="JerkFilter" Value="Used">
    <Property ID="JerkTime" Value="0.02" />
  </Selector>
  <Selector ID="ZeroVibrationFilter" Value="NotUsed" />
  <Group ID="UserData">
    <Property ID="Size" Value="16" />
  </Group>
  <Group ID="CollisionAvoidance">
    <Selector ID="InitialModelDimensions" Value="UseExplicitValues">
      <Group ID="Length">
        <Property ID="ExtentToFront" Value="0.025" />
        <Property ID="ExtentToBack" Value="0.025" />
      </Group>
      <Group ID="Width">
        <Property ID="Width" Value="0.046" />
      </Group>
    </Selector>
  </Group>
  <Group ID="StateTransitions">
    <Property ID="AutomaticRecontrol" Value="Active" />
  </Group>
</Element>
```

## Guidelines
- Movement limits are typically matched to hardware shuttle capabilities
- Shuttle stereotypes are referenced in assembly configuration by ID
- Use consistent movement parameters across stereotypes for same hardware type
- Jerk filtering improves precision but adds latency
- Zero vibration filter helps with ultra-precision applications
- Collision avoidance dimensions should reflect actual shuttle footprint


# Process point configuration file structure (*.processpoint)

## Purpose
- Process points define specific locations along a sector where shuttles can trigger events, stop at a barrier, or monitor throughput.
- The event can be checked by the PLC application using MC_BR_TrgPointEnable_AcpTrak and MC_BR_TrgPointGet_AcpTrak function blocks. This outputs an axis reference that can be used to move the shuttle to a specific position or other shuttle related FBs.
- The barrier can be controlled using MC_BR_BarrierCommand_AcpTrak function block.

## File Naming
- Each `*.processpoint` file must have a unique name, typically aligned with its assembly.  
  - Example: `Trak_Proce.processpoint`

## Process Point Definition
- Each processpoint file contains one or more process point elements with unique IDs.
- Keep these core IDs unchanged in naming and hierarchy: `SectorRef`, `Position`, `PositionRelativeTo`, `Type`, `BarrierFunctionality`, `ThroughputMonitor`

## Essential Configuration Elements

### Location & Positioning (required)
- **SectorRef**: Reference to an existing sector in the assembly (e.g., `BaseSector_1_1`)
- **Position**: Distance along the sector (LREAL, in metres)
- **PositionRelativeTo**: `FromStart` | `FromEnd`

### Barrier (optional)
- **BarrierFunctionality**: `On` | `Off` (default: `Off`)
- When `On`:
  - **ShuttleStopPosition**: `Extent` | `Center` 
    - `Extent`: stop shuttle length + safety distance **before** barrier
    - `Center`: stop shuttle center point **at** barrier (⚠️ cannot reverse!)
  - **BarrierStopDistance**: additional safety distance (LREAL metres, 0.0-1.0, default: 0.0)
    - Additive distance maintained from barrier approach side
    - Cannot be passed if not authorized (ticket system compatible)

### Throughput Monitor (optional)
- **ThroughputMonitor**: `ShuttleTriggered` | `Off` (default: `Off`)
- When `ShuttleTriggered`:
  - **BatchSize**: shuttles per batch window (UINT, default: 10)
    - Calculates throughput from last N shuttle intervals
  - **PartsPerShuttle**: product count per shuttle (REAL, default: 1.0)
  - Output: products per minute (updates when shuttle passes)

### Event Handling (optional)
- **TriggerEventBufferSize**: buffered events (UINT, default: 10)

## Common Use Cases

### Minimal Process Point (location-only)
```xml
<Element ID="ProcessPoint_1" Type="processpoint">
  <Selector ID="Type" Value="AcpTrakPoint">
    <Property ID="SectorRef" Value="BaseSector_1_1" />
    <Property ID="Position" Value="0.0" />
    <Property ID="PositionRelativeTo" Value="FromStart" />
  </Selector>
</Element>
```

### Process Point with Barrier
```xml
<Element ID="ProcessPoint_Barrier" Type="processpoint">
  <Selector ID="Type" Value="AcpTrakPoint">
    <Property ID="SectorRef" Value="BaseSector_1_1" />
    <Property ID="Position" Value="0.33" />
    <Property ID="PositionRelativeTo" Value="FromStart" />
    <Selector ID="BarrierFunctionality" Value="On">
      <Property ID="ShuttleStopPosition" Value="Extent" />
      <Property ID="BarrierStopDistance" Value="0.01" />
    </Selector>
  </Selector>
</Element>
```

### Process Point with Throughput Monitoring
```xml
<Element ID="ProcessPoint_Monitor" Type="processpoint">
  <Selector ID="Type" Value="AcpTrakPoint">
    <Property ID="SectorRef" Value="BaseSector_1_1" />
    <Property ID="Position" Value="0.5" />
    <Property ID="PositionRelativeTo" Value="FromStart" />
    <Property ID="TriggerEventBufferSize" Value="20" />
    <Selector ID="ThroughputMonitor" Value="ShuttleTriggered">
      <Property ID="BatchSize" Value="10" />
      <Property ID="PartsPerShuttle" Value="1.0" />
    </Selector>
  </Selector>
</Element>
```

## Guidelines
- The `SectorRef` must reference a valid, existing sector from the assembly configuration.
- Position coordinates must be within the sector's valid range.
- Multiple process points can exist in the same file, each with a unique ID.
- When combining barrier and throughput monitoring, both selectors can coexist.
- Use descriptive IDs that reflect the process point's function (e.g., `ProcessPoint_Loading`, `ProcessPoint_Unloading`)
- **Important Enums**:
  - `PositionRelativeTo`: `FromStart` | `FromEnd`
  - `BarrierFunctionality`: `On` | `Off`
  - `ShuttleStopPosition`: `Extent` | `Center`
  - `ThroughputMonitor`: `ShuttleTriggered` | `Off`.
  
  
## Programming Guidelines

# B&R Automation Studio mapptrak programming guidelines

Use this when creating or changing ACOPOStrak tasks and related mapptrak programs in Automation Studio 6.  
**Goal:** build valid Structured Text tasks using mapptrak.

## General
- All tasks must run in Taskclass #1.
- **ACTION blocks must be in separate files** — do NOT place ACTION definitions inline in `Main.st`. Create a dedicated file (e.g., `ProcessStation.st`) for each ACTION and register it in both `IEC.prg` and `Package.pkg`.
- **Important** Always check that a FB exists by checking the McAcpTrak.fun file.

## Scope

- Applies to mappTrak ST code that resides under `Logical/*`:

  - `Logical/**/*.st`
  - `Logical/**/*.var`
  - `Logical/**/*.typ`

## Reference template

The following code block serves as the **canonical reference template** for new ACOPOStrak projects.
Use this as the basis when generating assembly control tasks, type definitions, and variable declarations from scratch.
The standard way to execute the assembly control task is to use the gTrakAsm.Cmd.Power variable to power on the assembly and to use the gTrakAsm.Cmd.AllShuttleMoveVelocity variable to start all shuttles moving at the same velocity. **Attention:** The Position for RoutedMoveVel must be considered for the next movement to avoid the shuttle being on an internal sector. Therefore the Position must be set before the first different movement command is executed. (e.g., before a barrier or ProcessPoint where a new movement command is executed.)

### Shuttle initialization behavior

Choose **one** of the following approaches for simulation—do not mix them:

**Option A: SimShuttle array (used in this template)**

1. **Before PowerOn** (`ASM_CLEAN_SHUTTLE`): All existing shuttles are deleted via `MC_BR_AsmDeleteShuttle_0` with `mcACPTRAK_SH_DELETE_ALL`.
2. **PowerOn** (`ASM_POWER_ON`): Assembly is powered on via `MC_BR_AsmPowerOn_0`.
3. **After Ready** (`ADD_SHUTTLE`): Shuttles are added from the `SimShuttle` array via `MC_BR_SecAddShuttle_0`:
   - `Sector` := `SimShuttle[i].SectorRef` (from array)
   - `Position` := `SimShuttle[i].Position` (from array)
   - `Velocity` := `0.0` (fixed)
   - `Orientation` := `mcDIR_POSITIVE` (fixed)
4. **Loop exit condition**: When `SimShuttle[i].SectorRef = 0` (unconfigured entry) or `CntShuttle > TRAK_MAX_SHUTTLE`.
5. **Get shuttles**: After adding, shuttles are read via `MC_BR_AsmGetShuttle_AcpTrak`.
6. Configure `SimShuttle` entries in `_INIT` with valid `SectorRef` (e.g., `ADR(BaseSector_1_1)`) and `Position`.

**Option B: InitShuttleSim assembly feature**

1. Shuttles are pre-configured in the assembly configuration file (`.assembly` → `InitShuttleSim`).
2. **No delete, no add**: Skip `ASM_CLEAN_SHUTTLE` and `ADD_SHUTTLE` states.
3. **After PowerOn/Ready**: Read shuttles via `MC_BR_AsmGetShuttle_AcpTrak`—same as real hardware.
4. Behaves like real hardware: shuttles exist on the assembly from startup.

**Real hardware** (non-simulated):

- Shuttles are **not** deleted or added.
- After PowerOn/Ready, shuttles are read from the physical assembly via `MC_BR_AsmGetShuttle_AcpTrak`.

```iec-st

(*Assembly control interface*)

TYPE
	TrakAsmType : 	STRUCT  (*ACOPOStrak assembly main interface structure type*)
		AssemblyRef : REFERENCE TO McAssemblyType; (*Assembly reference - ADR(gAssembly_1)*)
		Cmd : TrakAsmCommandType; (*Assembly command structure*)
		Par : TrakAsmParameterType; (*Assembly parameters structure*)
		Status : TrakAsmStatusType; (*Assembly status structure*)
		Shuttle : ARRAY[1..TRAK_MAX_SHUTTLE]OF TrakAsmShuttleType; (*Shuttle information*)
	END_STRUCT;
	TrakAsmCommandType : 	STRUCT  (*Assembly commands*)
		Power : BOOL; (*Command power on/off*)
		ErrorReset : BOOL; (*Command error reset*)
		AllShuttleMoveStop : BOOL; (*Command stop all shuttle movements*)
		AllShuttleMovePosition : BOOL; (*Command start move all shuttles to position*)
		AllShuttleMoveVelocity : BOOL; (*Command start move all shuttles to velocity*)
	END_STRUCT;
	TrakAsmParameterType : 	STRUCT  (*Assembly parameters*)
		MoveSectorRef : REFERENCE TO McSectorType; (*Parameter shuttle move target sector - ADR(Sector_x)*)
		MovePosition : LREAL; (*[m] Parameter shuttle move target position*)
		MoveVelocity : REAL; (*[m/s] Parameter shuttle move target velocity*)
		MoveRouteVelocity : REAL; (*[m/s] Parameter shuttle move route velocity*)
		MoveAcceleration : REAL; (*[m/s^2] Parameter shuttle move acceleration*)
		MoveDeceleration : REAL; (*[m/s^2] Parameter shuttle move deceleration*)
	END_STRUCT;
	TrakAsmStatusType : 	STRUCT  (*Assembly status*)
		Simulated : BOOL; (*Status assembly is simulated*)
		Disabled : BOOL; (*Status assembly is disabled*)
		Ready : BOOL; (*Status assembly is ready*)
		Homing : BOOL; (*Status assembly is homing*)
		Stopping : BOOL; (*Status assembly is stopping*)
		ErrorStop : BOOL; (*Status assembly is in error-stop*)
		StationError : BOOL; (*One of the process stations is in error state*)
		CommunicationReady : BOOL; (*Status PLK communication to all segments in assembly is ready*)
		ReadyForPower : BOOL; (*Status assembly is ready for command power-on*)
		Power : BOOL; (*Status assembly is power-on*)
		ReadyForMovement : BOOL; (*Status assembly is ready to start shuttle movements*)
		ShuttlesPresent : UINT; (*Shuttles present on assembly*)
		ShuttlesInErrorStop : UINT; (*Shuttles in error-stop*)
	END_STRUCT;
	TrakAsmShuttleType : 	STRUCT  (*ACOPOStrak shuttle information structure type*)
		Valid : BOOL; (*Shuttledata valid*)
		Axis : McAxisType; (*McAxis of shuttle*)
		ID : UDINT; (*Shuttle ID as displayed in logger and SceneViewer*)
		ActSectorType : McAcpTrakSecTypeEnum; (*Shuttle current sector type*)
		ActSector : STRING[32]; (*Shuttle current sector*)
		ActPosition : LREAL; (*[m] Shuttle position on sector*)
		ActVelocity : REAL; (*[m/s] Shuttle velocity on sector*)
		TotalMoveDistance : LREAL; (*[m] Shuttle total moved distance*)
		StatusStandStill : BOOL; (*Shuttle is in state Standstill*)
		StatusStopping : BOOL; (*Shuttle is in state Stopping*)
		StatusErrorStop : BOOL; (*Shuttle is in state Errorstop*)
		StatusDiscreteMotion : BOOL; (*Shuttle is in state DiscreteMotion*)
		StatusContinuousMotion : BOOL; (*Shuttle is in state ContinuousMotion*)
		StatusSynchronizedMotion : BOOL; (*Shuttle is in state SynchronizedMotion*)
	END_STRUCT;
	TrakAsmVisType: STRUCT  (*ACOPOStrak shuttle color and product state information structure type*)
		Shuttle : ARRAY[1..TRAK_MAX_SHUTTLE]OF TrakAsmShuttleVisType;
	END_STRUCT;
	TrakAsmShuttleVisType: STRUCT  (*ACOPOStrak shuttle color and product state information structure type*)
		ColorRed: REAL;
		ColorGreen: REAL;
		ColorBlue: REAL;
		ProductState: REAL;
	END_STRUCT;
END_TYPE

(*Shuttle UserData structure - adapt to your application*)

TYPE
	TrakShuttleUserDataType : 	STRUCT  (*Shuttle user data structure type - to be adapted to application*)
		Color : ShuttleColorType; (*Do not change: SceneViewer shuttle color*)
		ProductValue : DINT; (*Product value used for routing decisions*)
	END_STRUCT;
	ShuttleColorType : 	STRUCT  (*Shuttle color type for digital twin. RGB format.*)
		Red : REAL; (*Red component of the color*)
		Green : REAL; (*Green component of the color*)
		Blue : REAL; (*Blue component of the color*)
	END_STRUCT;
END_TYPE

VAR CONSTANT
	TRAK_MAX_SHUTTLE : UINT := 10; (*Maximum number of shuttles handled in this application - should be set equal or higher than Shuttles-maximum-count setting in mappMotion assembly config file*)
END_VAR
(*Assembly control interface*)
VAR
	gTrakAsm : TrakAsmType; (*Trak assembly main interface structure*)
END_VAR
(*Assembly monitoring data for RS*)
VAR
	gAssemblyMon : McAcpTrakAssemblyMonData; (*Trak assembly monitoring data used for RS connection*)
END_VAR
VAR
	gVis : TrakAsmVisType; (*Trak assembly vis data used for RS connection*)
END_VAR

TYPE
	TrakSimShuttleType : 	STRUCT  (*Simulated shuttle structure*)
		SectorRef : REFERENCE TO McSectorType; (*add shuttle in simulation at sector - ADR(Sector_x)*)
		Position : LREAL; (*add shuttle in simulation at position*)
	END_STRUCT;
END_TYPE

(*enum*)

TYPE
	StepEnum : 
		( (*Main assembly control states enumeration*)
		COMMAND := 0,
		RESET,
		ASM_CLEAN_SHUTTLE,
		ASM_CLEAN_SHUTTLE_WAIT,
		ASM_POWER_ON := 20,
		ASM_POWER_ON_WAIT,
		ADD_SHUTTLE_INIT := 30,
		ADD_SHUTTLE,
		ADD_SHUTTLE_WAIT,
		GET_SHUTTLE_INIT := 40,
		GET_SHUTTLE,
		GET_SHUTTLE_WAIT,
		ERROR := 90,
		ERROR_RESET := 91,
		READY := 100,
		ASM_POWER_OFF := 110,
		ASM_POWER_OFF_WAIT,
		STOP_SHUTTLE := 120,
		STOP_SHUTTLE_WAIT,
		MOVE_SHUTTLE_VELOCITY := 130,
		MOVE_SHUTTLE_VELOCITY_WAIT,
		MOVE_SHUTTLE_POSITION := 140,
		MOVE_SHUTTLE_POSITION_WAIT,
		MOVING := 150
		);
END_TYPE


(*sequence control*)
VAR
	Step : StepEnum; (*Current state*)
	i : UINT; (*Indexing variable*)
	CntShuttle : UINT; (*Shuttle counter used for indexing*)
END_VAR
(*add shuttles for simulation*)
VAR
	SimShuttle : ARRAY[1..TRAK_MAX_SHUTTLE] OF TrakSimShuttleType; (*Simulated shuttles*)
END_VAR
(*initialize shuttle user data*)
VAR
	SizeofShuttleUserData : UDINT; (*Size of shuttle user data structure in bytes*)
	UserData : REFERENCE TO TrakShuttleUserDataType;
	ShuttleUserData : TrakShuttleUserDataType; (*Shuttle user data structure*)
	MC_BR_ShCopyUserData_0 : MC_BR_ShCopyUserData_AcpTrak;
END_VAR
(*assembly FUB*)
VAR
	MC_BR_AsmReset_0 : MC_BR_AsmReset_AcpTrak;
	MC_BR_AsmDeleteShuttle_0 : MC_BR_AsmDeleteShuttle_AcpTrak;
	MC_BR_AsmPowerOn_0 : MC_BR_AsmPowerOn_AcpTrak;
	MC_BR_AsmPowerOff_0 : MC_BR_AsmPowerOff_AcpTrak;
	MC_BR_AsmGetInfo_0 : MC_BR_AsmGetInfo_AcpTrak;
	MC_BR_AsmReadStatus_0 : MC_BR_AsmReadStatus_AcpTrak;
	MC_BR_AsmReadInfo_0 : MC_BR_AsmReadInfo_AcpTrak;
	MC_BR_SecAddShuttle_0 : MC_BR_SecAddShuttle_AcpTrak;
	MC_BR_AsmGetShuttle_0 : MC_BR_AsmGetShuttle_AcpTrak;
	MC_BR_AsmStop_0 : MC_BR_AsmStop_AcpTrak := (0);
END_VAR
(*segment FUB*)
(*shuttle FUB*)
VAR
	MC_BR_ShReadInfo_0 : ARRAY[1..TRAK_MAX_SHUTTLE] OF MC_BR_ShReadInfo_AcpTrak;
	MC_ReadStatus_0 : ARRAY[1..TRAK_MAX_SHUTTLE] OF MC_ReadStatus;
	MC_BR_RoutedMoveVel_0 : MC_BR_RoutedMoveVel_AcpTrak;
	MC_BR_RoutedMoveAbs_0 : MC_BR_RoutedMoveAbs_AcpTrak;
END_VAR
(*Initialize shuttle user state*)
VAR
	ShuttleUserState : UDINT; (*Shuttle user state as stored on a shuttle.*)
END_VAR

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Example trak assembly basic control task
// TC#1 => preferred to be attached to mappMotion task class
// 
// * assembly startup sequence
// * ErrorStop handling
// * basic shuttle handling
// * aggregated shuttle move commands
// 
// "gTrakAsm" - global interface structure
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
 
PROGRAM _INIT

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// shuttles to add for simulation - adapt sector and positions to your application
	SimShuttle[1].SectorRef := ADR(BaseSector_1_1);	SimShuttle[1].Position := 0.0;
	SimShuttle[2].SectorRef := ADR(BaseSector_1_1);	SimShuttle[2].Position := 0.1;
	SimShuttle[3].SectorRef := ADR(BaseSector_1_1);	SimShuttle[3].Position := 0.2;
	// add more shuttles as needed...


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// interface structure

	gTrakAsm.AssemblyRef := ADR(gAssembly_1);
	gTrakAsm.Cmd.Power := FALSE;
	gTrakAsm.Cmd.ErrorReset := FALSE;

	gTrakAsm.Cmd.AllShuttleMoveStop := FALSE;
	gTrakAsm.Cmd.AllShuttleMovePosition := FALSE;
	gTrakAsm.Cmd.AllShuttleMoveVelocity := FALSE;
	gTrakAsm.Par.MoveSectorRef := ADR(BaseSector_1_1);	//sector reference of the initial process station
	gTrakAsm.Par.MovePosition := 0.0;
	gTrakAsm.Par.MoveVelocity := 2;
	gTrakAsm.Par.MoveRouteVelocity := 0;
	gTrakAsm.Par.MoveAcceleration := 10;
	gTrakAsm.Par.MoveDeceleration := 10;
	
	Step := COMMAND;

	// size of ShuttleUserData structure - this value has to be entered in ShuttleStereotype -> UserData -> Size
	SizeofShuttleUserData := SIZEOF(ShuttleUserData);
	
	//assembly monitoring data for RS
	gAssemblyMon;
END_PROGRAM





PROGRAM _CYCLIC


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// global ErrorStop and ErrorReset
	IF(gTrakAsm.Status.ErrorStop)THEN
		gTrakAsm.Status.ReadyForMovement := FALSE;
		Step := ERROR;
	END_IF
	IF(gTrakAsm.Cmd.ErrorReset)THEN
		MC_BR_AsmReset_0.Execute := TRUE;
	END_IF

	IF gTrakAsm.Status.StationError AND NOT(Step = ERROR OR Step = ERROR_RESET) THEN
		Step := ERROR;
	END_IF;


	CASE Step OF
	
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// command
		COMMAND:
			IF(gTrakAsm.Cmd.Power)THEN
				Step := RESET;
			END_IF

			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// startup sequence

		// reset
		RESET:
			brsmemset(ADR(gTrakAsm.Shuttle), 0, SIZEOF(gTrakAsm.Shuttle));
			MC_BR_AsmGetShuttle_0.Enable := FALSE;
			Step := ASM_CLEAN_SHUTTLE;
//			Step := ASM_POWER_ON;	//with mappTrak version 6.1 the ASM_CLEAN_SHUTTLE step could be skipped (ticketing issue fixed with new mode, so no need to delete the shuttles and create them again)
			
		// clean existing shuttles (for simulation only)
		ASM_CLEAN_SHUTTLE:
			IF(gTrakAsm.Status.Simulated)THEN
				MC_BR_AsmDeleteShuttle_0.Command := mcACPTRAK_SH_DELETE_ALL;
				MC_BR_AsmDeleteShuttle_0.Execute := TRUE;
				Step := ASM_CLEAN_SHUTTLE_WAIT;
			ELSE
				Step := ASM_POWER_ON;
			END_IF
			
		ASM_CLEAN_SHUTTLE_WAIT:
			IF MC_BR_AsmDeleteShuttle_0.Done THEN
				Step := ASM_POWER_ON;
			END_IF
				
		// assembly power ON
		ASM_POWER_ON:
			IF(gTrakAsm.Status.Disabled)THEN
				MC_BR_AsmPowerOn_0.Execute := TRUE;
			END_IF
			Step := ASM_POWER_ON_WAIT;
			
		ASM_POWER_ON_WAIT:
			IF(gTrakAsm.Status.Ready)THEN
				Step := ADD_SHUTTLE_INIT;
			END_IF
			
			
		// add shuttles (for simulation only)
		ADD_SHUTTLE_INIT:
			IF(gTrakAsm.Status.Simulated)THEN
				CntShuttle := 0;
				Step := ADD_SHUTTLE;
			ELSE
				Step := GET_SHUTTLE_INIT;
			END_IF
		
		ADD_SHUTTLE:
			CntShuttle := CntShuttle + 1;
			IF(CntShuttle > TRAK_MAX_SHUTTLE)THEN
				Step := GET_SHUTTLE_INIT;
			ELSIF(SimShuttle[CntShuttle].SectorRef = 0)THEN
				Step := GET_SHUTTLE_INIT;
			ELSE
				MC_BR_SecAddShuttle_0.Sector := SimShuttle[CntShuttle].SectorRef;
				MC_BR_SecAddShuttle_0.Execute := TRUE;
				MC_BR_SecAddShuttle_0.Position := SimShuttle[CntShuttle].Position;
				MC_BR_SecAddShuttle_0.Velocity := 0.0;
				MC_BR_SecAddShuttle_0.Orientation := mcDIR_POSITIVE;
				Step := ADD_SHUTTLE_WAIT;
			END_IF
			
		ADD_SHUTTLE_WAIT:
			IF(MC_BR_SecAddShuttle_0.Error)THEN
				Step := ERROR;

			ELSIF(MC_BR_SecAddShuttle_0.Done)THEN
				Step := ADD_SHUTTLE;
			END_IF
		
		
		// get all shuttles present on assembly
		GET_SHUTTLE_INIT:
			MC_BR_AsmGetShuttle_0.Enable := TRUE;
			MC_BR_AsmGetShuttle_0.Next := FALSE;
			CntShuttle := 0;
			Step := GET_SHUTTLE_WAIT;
		
		GET_SHUTTLE:
			MC_BR_AsmGetShuttle_0.Next := TRUE;
			Step := GET_SHUTTLE_WAIT;
		
		GET_SHUTTLE_WAIT:
			IF(MC_BR_AsmGetShuttle_0.Error)THEN
				MC_BR_AsmGetShuttle_0.Enable := FALSE;
				Step := ERROR;
			
			ELSIF(MC_BR_AsmGetShuttle_0.Valid)THEN
				IF(MC_BR_AsmGetShuttle_0.TotalCount = 0)THEN			// no shuttles present => ok, proceed
					MC_BR_AsmGetShuttle_0.Enable := FALSE;
					Step := READY;
				
				ELSE
					CntShuttle := CntShuttle + 1;
					gTrakAsm.Shuttle[CntShuttle].Axis := MC_BR_AsmGetShuttle_0.Axis;
					gTrakAsm.Shuttle[CntShuttle].ID := MC_BR_AsmGetShuttle_0.AdditionalInfo.ShuttleID;

					//// initialize shuttle UserData - not used in version 1
					//ShuttleUserData.Color.Red := 255;
					//ShuttleUserData.Color.Green := 136;
					//ShuttleUserData.Color.Blue := 0;
					
					MC_BR_ShCopyUserData_0.Axis := ADR(gTrakAsm.Shuttle[CntShuttle].Axis);
					MC_BR_ShCopyUserData_0.Execute := TRUE;
					MC_BR_ShCopyUserData_0.DataAddress := ADR(ShuttleUserData);
					MC_BR_ShCopyUserData_0.DataSize := SIZEOF(ShuttleUserData);
					MC_BR_ShCopyUserData_0.Mode := mcACPTRAK_USERDATA_SET;
					
					//initialize shuttle user state
					ShuttleUserState := 0;	//set all the shuttles as not initialized
					MC_BR_ShCopyUserData_0.UserState := ShuttleUserState;

					IF(MC_BR_AsmGetShuttle_0.RemainingCount = 0)THEN	// all present shuttles read => ok, proceed
						MC_BR_AsmGetShuttle_0.Enable := FALSE;
						Step := READY;
						
					ELSIF(CntShuttle >= TRAK_MAX_SHUTTLE)THEN					// more shuttles present than max configured => error
						MC_BR_AsmGetShuttle_0.Enable := FALSE;
						Step := ERROR;
				
					ELSE														// read next => positive edge on .Next flag
						MC_BR_AsmGetShuttle_0.Next := FALSE;
						Step := GET_SHUTTLE;
						
					END_IF
				END_IF
			END_IF
		
	
	
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// assembly ErrorStop => ErrorReset required
		ERROR:
			gTrakAsm.Cmd.Power := FALSE;

			IF gTrakAsm.Cmd.ErrorReset THEN
				Step := ERROR_RESET;
			END_IF;
			
						
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// ErrorReset
		ERROR_RESET:
			gTrakAsm.Status.StationError := FALSE;

			IF(gTrakAsm.Status.Disabled)THEN
				gTrakAsm.Cmd.ErrorReset := FALSE;
				Step := COMMAND;
			ELSIF(gTrakAsm.Status.Ready)THEN
				gTrakAsm.Cmd.ErrorReset := FALSE;
				Step := ASM_POWER_OFF;
			END_IF
			
			
			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// assembly Active and ready for movement
		READY:
			gTrakAsm.Status.ReadyForMovement := TRUE;
			IF(gTrakAsm.Cmd.Power = 0)THEN
				gTrakAsm.Status.ReadyForMovement := FALSE;
				Step := ASM_POWER_OFF;
				
			ELSIF(gTrakAsm.Cmd.AllShuttleMoveStop)THEN
				gTrakAsm.Cmd.AllShuttleMoveStop := 0;
				Step := STOP_SHUTTLE;

			ELSIF(gTrakAsm.Cmd.AllShuttleMoveVelocity)THEN
				gTrakAsm.Cmd.AllShuttleMoveVelocity := 0;
				CntShuttle := 0;
				Step := MOVE_SHUTTLE_VELOCITY;

			ELSIF(gTrakAsm.Cmd.AllShuttleMovePosition)THEN
				gTrakAsm.Cmd.AllShuttleMovePosition := 0;
				CntShuttle := 0;
				Step := MOVE_SHUTTLE_POSITION;

			END_IF
	
			
			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// assembly power OFF
		ASM_POWER_OFF:
			MC_BR_AsmPowerOff_0.Execute := TRUE;
			Step := ASM_POWER_OFF_WAIT;
			
		ASM_POWER_OFF_WAIT:
			IF(gTrakAsm.Status.Disabled)THEN
				Step := COMMAND;
			END_IF
	
			
			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// stop all shuttles in assembly
		STOP_SHUTTLE:
			MC_BR_AsmStop_0.Execute		:= TRUE;
			MC_BR_AsmStop_0.AdvancedParameters.StopMode	:= mcSTOPMODE_JERK_LIMIT;
			Step := STOP_SHUTTLE_WAIT;
		
		STOP_SHUTTLE_WAIT:
			IF(MC_BR_AsmStop_0.Busy = FALSE)THEN
				Step := READY;
			END_IF
	
			
			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// move all shuttles
		
		// accelerate all shuttles in assembly via the same target position to the same velocity
		// used for startup, to accelerate all shuttles to an initial velocity in order to pass the first process-point
		MOVE_SHUTTLE_VELOCITY:
			CntShuttle := CntShuttle + 1;
			IF(CntShuttle > TRAK_MAX_SHUTTLE)THEN
				Step := MOVING;
			ELSIF(gTrakAsm.Shuttle[CntShuttle].Valid)THEN
				MC_BR_RoutedMoveVel_0.Axis := ADR(gTrakAsm.Shuttle[CntShuttle].Axis);
				MC_BR_RoutedMoveVel_0.Execute := TRUE;
				MC_BR_RoutedMoveVel_0.Sector := gTrakAsm.Par.MoveSectorRef;
				MC_BR_RoutedMoveVel_0.Position := gTrakAsm.Par.MovePosition;
				MC_BR_RoutedMoveVel_0.Velocity := gTrakAsm.Par.MoveVelocity;
				MC_BR_RoutedMoveVel_0.RouteVelocity := gTrakAsm.Par.MoveRouteVelocity;
				MC_BR_RoutedMoveVel_0.Acceleration := gTrakAsm.Par.MoveAcceleration;
				MC_BR_RoutedMoveVel_0.Deceleration := gTrakAsm.Par.MoveDeceleration;
				MC_BR_RoutedMoveVel_0.Jerk := 0.0;
				MC_BR_RoutedMoveVel_0.BufferMode := mcABORTING;
				MC_BR_RoutedMoveVel_0.AdvancedParameters.StartDirection := mcDIR_UNDEFINED;
				MC_BR_RoutedMoveVel_0.AdvancedParameters.EndDirection := mcDIR_POSITIVE;
				MC_BR_RoutedMoveVel_0.AdvancedParameters.ShuttleOrientation := mcDIR_UNDEFINED;
				Step := MOVE_SHUTTLE_VELOCITY_WAIT;
			END_IF

		MOVE_SHUTTLE_VELOCITY_WAIT:
			Step := MOVE_SHUTTLE_VELOCITY;
			
			
		// move all shuttles in assembly to the same target position
		// used to move all shuttles to a start position - one shuttle will reach this position, all the others are queuing up
		MOVE_SHUTTLE_POSITION:
			CntShuttle := CntShuttle + 1;
			IF(CntShuttle > TRAK_MAX_SHUTTLE)THEN
				Step := READY;
			ELSIF(gTrakAsm.Shuttle[CntShuttle].Valid)THEN
				MC_BR_RoutedMoveAbs_0.Axis := ADR(gTrakAsm.Shuttle[CntShuttle].Axis);
				MC_BR_RoutedMoveAbs_0.Execute := TRUE;
				MC_BR_RoutedMoveAbs_0.Sector := gTrakAsm.Par.MoveSectorRef;
				MC_BR_RoutedMoveAbs_0.Position := gTrakAsm.Par.MovePosition;
				MC_BR_RoutedMoveAbs_0.Velocity := gTrakAsm.Par.MoveVelocity;
				MC_BR_RoutedMoveAbs_0.Acceleration := gTrakAsm.Par.MoveAcceleration;
				MC_BR_RoutedMoveAbs_0.Deceleration := gTrakAsm.Par.MoveDeceleration;
				MC_BR_RoutedMoveAbs_0.Jerk := 0.0;
				MC_BR_RoutedMoveAbs_0.BufferMode := mcABORTING;
				MC_BR_RoutedMoveAbs_0.AdvancedParameters.StartDirection := mcDIR_UNDEFINED;
				MC_BR_RoutedMoveAbs_0.AdvancedParameters.EndDirection := mcDIR_POSITIVE;
				MC_BR_RoutedMoveAbs_0.AdvancedParameters.ShuttleOrientation := mcDIR_UNDEFINED;
				Step := MOVE_SHUTTLE_POSITION_WAIT;
			END_IF

		MOVE_SHUTTLE_POSITION_WAIT:
			Step := MOVE_SHUTTLE_POSITION;

		
		MOVING:
			IF(gTrakAsm.Cmd.Power = 0)THEN
				gTrakAsm.Status.ReadyForMovement := FALSE;
				Step := ASM_POWER_OFF;
				
			ELSIF(gTrakAsm.Cmd.AllShuttleMoveStop)THEN
				gTrakAsm.Cmd.AllShuttleMoveStop := 0;
				Step := STOP_SHUTTLE;
			END_IF;
		
			
	END_CASE
	
	
	
	
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// assembly FUBs
		
	MC_BR_AsmReset_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmReset_0();
	MC_BR_AsmReset_0.Execute := FALSE;
	
	MC_BR_AsmDeleteShuttle_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmDeleteShuttle_0();
	MC_BR_AsmDeleteShuttle_0.Execute := FALSE;

	MC_BR_AsmPowerOn_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmPowerOn_0();
	MC_BR_AsmPowerOn_0.Execute := FALSE;

	MC_BR_AsmPowerOff_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmPowerOff_0();
	MC_BR_AsmPowerOff_0.Execute := FALSE;

	MC_BR_AsmGetInfo_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmGetInfo_0.Execute := TRUE;
	MC_BR_AsmGetInfo_0();

	gTrakAsm.Status.Simulated := (DiagCpuIsARsim() OR DiagCpuIsSimulated() OR (MC_BR_AsmGetInfo_0.AssemblyInfo.SimulationOnPlcMode = mcACPTRAK_SIM_STATE_ON));

	MC_BR_AsmReadStatus_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmReadStatus_0.Enable := TRUE;
	MC_BR_AsmReadStatus_0();

	gTrakAsm.Status.Disabled := MC_BR_AsmReadStatus_0.AssemblyDisabled;
	gTrakAsm.Status.Ready := MC_BR_AsmReadStatus_0.AssemblyReady;
	gTrakAsm.Status.Homing := MC_BR_AsmReadStatus_0.AssemblyHoming;
	gTrakAsm.Status.Stopping := MC_BR_AsmReadStatus_0.AssemblyStopping;
	gTrakAsm.Status.ErrorStop := MC_BR_AsmReadStatus_0.AssemblyErrorStop;

	MC_BR_AsmReadInfo_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmReadInfo_0.Enable := TRUE;
	MC_BR_AsmReadInfo_0();

	gTrakAsm.Status.CommunicationReady := MC_BR_AsmReadInfo_0.AssemblyInfo.CommunicationReady;
	gTrakAsm.Status.ReadyForPower := MC_BR_AsmReadInfo_0.AssemblyInfo.ReadyForPowerOn;
	gTrakAsm.Status.Power := MC_BR_AsmReadInfo_0.AssemblyInfo.PowerOn;
	gTrakAsm.Status.ShuttlesPresent := MC_BR_AsmReadInfo_0.AssemblyInfo.ShuttleCount.Count;
	gTrakAsm.Status.ShuttlesInErrorStop := MC_BR_AsmReadInfo_0.AssemblyInfo.ShuttleCount.InErrorStop;
	
	MC_BR_SecAddShuttle_0();
	MC_BR_SecAddShuttle_0.Execute := FALSE;
	
	MC_BR_AsmGetShuttle_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmGetShuttle_0();

	MC_BR_AsmStop_0.Assembly := gTrakAsm.AssemblyRef;
	MC_BR_AsmStop_0();
	MC_BR_AsmStop_0.Execute := FALSE;

	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// shuttle FUBs

	FOR i:=1 TO TRAK_MAX_SHUTTLE DO
	
		MC_BR_ShReadInfo_0[i].Axis := ADR(gTrakAsm.Shuttle[i].Axis);
		MC_BR_ShReadInfo_0[i].Enable := (gTrakAsm.Shuttle[i].Axis.controlif <> 0);
		MC_BR_ShReadInfo_0[i]();

		MC_ReadStatus_0[i].Axis := ADR(gTrakAsm.Shuttle[i].Axis);
		MC_ReadStatus_0[i].Enable := (gTrakAsm.Shuttle[i].Axis.controlif <> 0);
		MC_ReadStatus_0[i]();

		gTrakAsm.Shuttle[i].Valid := (MC_BR_ShReadInfo_0[i].Valid AND MC_ReadStatus_0[i].Valid);
		IF(gTrakAsm.Shuttle[i].Valid)THEN
			gTrakAsm.Shuttle[i].ActSectorType := MC_BR_ShReadInfo_0[i].ShuttleInfo.CurrentValues.SectorType;
			gTrakAsm.Shuttle[i].ActSector := MC_BR_ShReadInfo_0[i].ShuttleInfo.CurrentValues.SectorName;
			gTrakAsm.Shuttle[i].ActPosition := MC_BR_ShReadInfo_0[i].ShuttleInfo.CurrentValues.Position;
			gTrakAsm.Shuttle[i].ActVelocity := MC_BR_ShReadInfo_0[i].ShuttleInfo.CurrentValues.Velocity;
			gTrakAsm.Shuttle[i].TotalMoveDistance := MC_BR_ShReadInfo_0[i].ShuttleInfo.LifeCycleInfo.AbsoluteMovementDistance;
			gTrakAsm.Shuttle[i].StatusStandStill := MC_ReadStatus_0[i].StandStill;
			gTrakAsm.Shuttle[i].StatusStopping := MC_ReadStatus_0[i].Stopping;
			gTrakAsm.Shuttle[i].StatusErrorStop := MC_ReadStatus_0[i].ErrorStop;
			gTrakAsm.Shuttle[i].StatusDiscreteMotion := MC_ReadStatus_0[i].DiscreteMotion;
			gTrakAsm.Shuttle[i].StatusContinuousMotion := MC_ReadStatus_0[i].ContinuousMotion;
			gTrakAsm.Shuttle[i].StatusSynchronizedMotion := MC_ReadStatus_0[i].SynchronizedMotion;
		END_IF		
	END_FOR

	FOR i:=1 TO TRAK_MAX_SHUTTLE DO
		IF gAssemblyMon.Shuttle[i-1].Available THEN
			UserData ACCESS gAssemblyMon.Shuttle[i-1].UserData;
			gVis.Shuttle[i].ColorRed := UserData.Color.Red;
			gVis.Shuttle[i].ColorGreen := UserData.Color.Green;
			gVis.Shuttle[i].ColorBlue := UserData.Color.Blue;
			gVis.Shuttle[i].ProductState := DINT_TO_REAL(UserData.ProductValue);
		END_IF
	END_FOR

	MC_BR_ShCopyUserData_0();
	MC_BR_ShCopyUserData_0.Execute := FALSE;
	
	MC_BR_RoutedMoveVel_0();
	MC_BR_RoutedMoveVel_0.Execute := FALSE;
	
	MC_BR_RoutedMoveAbs_0();
	MC_BR_RoutedMoveAbs_0.Execute := FALSE;

END_PROGRAM





PROGRAM _EXIT

END_PROGRAM
```

# Overview of ACOPOStrak function blocks

ACOPOStrak is used to control shuttles on a linear motor track system. Programming is done via function blocks in Structured Text.

## Execute vs Enable pattern

- **Execute FBs:** Trigger with rising edge on `Execute`, wait for `Done`/`Error`, reset `Execute` after call in same scan.
- **Enable FBs:** Keep `Enable := TRUE` continuously, check `Valid` for active state, `Error` for failures.

## Division of function blocks

Function blocks are divided into different categories based on their function:
- **Administrative:** Managing system state, initialization, configuration, or getting references of objects
- **Motion:** Controlling shuttle movement
- **Informative:** Reading status, information, or diagnostics from the system
- **Process Point / Barrier:** Controlling shuttle flow at process stations

## RoutedMove function blocks
- MC_BR_RoutedMove(Abs,Vel)_AcpTrak: Shuttles move to a target position on a target sector via routing. The shuttle will change to an internal sector until the Position input is reached. The shuttle will switch to the target sector after the position is reached.
- All other function blocks require the shuttle to be on the target sector before the move command is executed.

## Common mistakes with RoutedMove function blocks
- A new Movement command (e.g., ModuloMoveAbs) is executed before the previous RoutedMove command reaches the target position. This results in an error for the new command.

## Assembly function blocks

- `MC_BR_AsmPowerOn_AcpTrak`(`Execute`) - Power on assembly and identify shuttles
- `MC_BR_AsmPowerOff_AcpTrak`(`Execute`) - Power off assembly
- `MC_BR_AsmEnable_AcpTrak`(`Execute`) - Transition from Disabled to Ready
- `MC_BR_AsmDisable_AcpTrak`(`Execute`) - Transition to Disabled
- `MC_BR_AsmReset_AcpTrak`(`Execute`) - Reset assembly from error state to Disabled
- `MC_BR_AsmStop_AcpTrak`(`Execute`) - Stop all shuttles on assembly
- `MC_BR_AsmReadStatus_AcpTrak`(`Enable`) - Read assembly status (Disabled, Ready, Homing, Stopping, ErrorStop, Startup, InvalidConfig)
- `MC_BR_AsmReadInfo_AcpTrak`(`Enable`) - Read assembly info (CommunicationReady, PowerOn, ShuttleCount)
- `MC_BR_AsmReadErrorInfo_AcpTrak`(`Enable`) - Read assembly error information
- `MC_BR_AsmGetInfo_AcpTrak`(`Execute`) - Get assembly information including simulation mode
- `MC_BR_AsmGetShuttle_AcpTrak`(`Enable`) - Get shuttle references. Use `Next` to iterate.
- `MC_BR_AsmGetSector_AcpTrak`(`Enable`) - Get sector references from assembly
- `MC_BR_AsmGetSegment_AcpTrak`(`Enable`) - Get segment references from assembly
- `MC_BR_AsmGetDualTrack_AcpTrak`(`Enable`) - Get dual-track area references
- `MC_BR_AsmGetMinShDist_AcpTrak`(`Execute`) - Get minimum shuttle distance
- `MC_BR_AsmDeleteShuttle_AcpTrak`(`Execute`) - Delete shuttles (simulation). Commands: `mcACPTRAK_SH_DELETE_ALL`, single shuttle
- `MC_BR_AsmSetOverride_AcpTrak`(`Enable`) - Set velocity/acceleration override factor
- `MC_BR_AsmCamPrepare_AcpTrak`(`Execute`) - Prepare cam for coupling
- `MC_BR_AsmCopyShuttleData_AcpTrak`(`Execute`) - Copy shuttle data snapshot
- `MC_BR_AsmCopySegmentData_AcpTrak`(`Execute`) - Copy segment data snapshot
- `MC_BR_AsmCommandError_AcpTrak`(`Execute`) - Generate assembly error

## Sector function blocks

- `MC_BR_SecAddShuttle_AcpTrak`(`Execute`) - Add shuttle at position (simulation)
- `MC_BR_SecGetInfo_AcpTrak`(`Execute`) - Get sector information
- `MC_BR_SecReadInfo_AcpTrak`(`Enable`) - Read sector information cyclically
- `MC_BR_SecGetShuttle_AcpTrak`(`Enable`) - Get shuttles on sector
- `MC_BR_SecStop_AcpTrak`(`Execute`) - Stop all shuttles on sector

## Segment function blocks

- `MC_BR_SegPowerOn_AcpTrak`(`Execute`) - Power on segment
- `MC_BR_SegPowerOff_AcpTrak`(`Execute`) - Power off segment
- `MC_BR_SegReset_AcpTrak`(`Execute`) - Reset segment from error state
- `MC_BR_SegReadStatus_AcpTrak`(`Enable`) - Read segment status (Disabled, Ready, Stopping, ErrorStop, Startup, InvalidConfig)
- `MC_BR_SegReadInfo_AcpTrak`(`Enable`) - Read segment information
- `MC_BR_SegGetInfo_AcpTrak`(`Execute`) - Get segment information
- `MC_BR_SegReadErrorInfo_AcpTrak`(`Enable`) - Read segment error information
- `MC_BR_SegLimitErrorScope_AcpTrak`(`Execute`) - Limit error response scope
- `MC_BR_SegProcessParID_AcpTrak`(`Execute`) - Read/write segment ParIDs
- `MC_BR_SegCommandError_AcpTrak`(`Execute`) - Generate segment error

## Shuttle motion function blocks

- `MC_BR_RoutedMoveAbs_AcpTrak`(`Execute`) - Move to absolute position on target sector via routing
- `MC_BR_RoutedMoveVel_AcpTrak`(`Execute`) - Move via routing, then continue at velocity. Outputs: `InVelocity`, `PositionReached`
- `MC_BR_ModuloMoveAbs_AcpTrak`(`Execute`) - Move to modulo position on closed sector
- `MC_BR_ElasticMoveAbs_AcpTrak`(`Execute`) - Elastic move to absolute position
- `MC_BR_ElasticMoveAdd_AcpTrak`(`Execute`) - Elastic move over distance
- `MC_BR_ElasticMoveVel_AcpTrak`(`Execute`) - Elastic move at velocity
- `MC_BR_ElasticMoveCycPos_AcpTrak`(`Enable`) - Cyclic position setpoint (elastic)
- `MC_BR_MoveCyclicTcpPos_AcpTrak`(`Enable`) - Cyclic TCP position setpoint
- `MC_BR_MoveCyclicTcpVel_AcpTrak`(`Enable`) - Cyclic TCP velocity
- `MC_Halt`(`Execute`) - Halt movement (resumable)
- `MC_Stop`(`Execute`) - Stop movement
- `MC_BR_GetRouteInfo_AcpTrak`(`Execute`) - Get routing information before movement
- `MC_BR_GetPosition_AcpTrak`(`Execute`) - Translate position between components

## Shuttle information function blocks

- `MC_BR_ShReadInfo_AcpTrak`(`Enable`) - Read shuttle info (position, velocity, sector, lifecycle)
- `MC_ReadStatus`(`Enable`) - Read PLCopen status (StandStill, Stopping, ErrorStop, DiscreteMotion, ContinuousMotion, SynchronizedMotion)
- `MC_BR_ShReadErrorInfo_AcpTrak`(`Enable`) - Read shuttle error information
- `MC_BR_ShReadFrame_AcpTrak`(`Enable`) - Read shuttle frame (position/orientation in coordinate system)
- `MC_BR_ShReadSectorPos_AcpTrak`(`Enable`) - Read position on specific sector
- `MC_BR_ShReadSegmentInfo_AcpTrak`(`Enable`) - Read segments shuttle is on
- `MC_BR_ShReadSize_AcpTrak`(`Execute`) - Read shuttle dimensions
- `MC_BR_ShReadRecoveryInfo_AcpTrak`(`Execute`) - Read position at last control time

## Shuttle control function blocks

- `MC_BR_ShPowerOn_AcpTrak`(`Execute`) - Power on shuttle (Disabled → Standstill)
- `MC_BR_ShPowerOff_AcpTrak`(`Execute`) - Power off shuttle (→ Disabled)
- `MC_BR_ShCopyUserData_AcpTrak`(`Execute`) - Copy user data to/from shuttle. Modes: `mcACPTRAK_USERDATA_GET`, `mcACPTRAK_USERDATA_SET`
- `MC_BR_ShSetUserID_AcpTrak`(`Execute`) - Set user-defined shuttle ID string
- `MC_BR_ShResize_AcpTrak`(`Execute`) - Change shuttle dimensions at runtime
- `MC_BR_ShSelCtrlParSet_AcpTrak`(`Execute`) - Select controller parameter set
- `MC_BR_ShSetMotionFilter_AcpTrak`(`Execute`) - Set motion filter parameters
- `MC_BR_ShSwitchSector_AcpTrak`(`Execute`) - Switch shuttle's reference sector
- `MC_BR_ShLocalLimitCmd_AcpTrak`(`Execute`) - Enable/disable local motion limit
- `MC_BR_ShCouplingObjCmd_AcpTrak`(`Execute`) - Set/remove coupling object
- `MC_BR_ShInteractCmd_AcpTrak`(`Execute`) - Shuttle interaction commands
- `MC_BR_ShCamAutCmd_AcpTrak`(`Enable`) - Cam automat commands

## Process point / Barrier function blocks

- `MC_BR_TrgPointEnable_AcpTrak`(`Enable`) - Enable trigger point. Check `EventCount > 0` for events.
- `MC_BR_TrgPointReadInfo_AcpTrak`(`Execute`) - Read trigger info. Returns `TrgPointInfo.Axis`.
- `MC_BR_ProcPointGetInfo_AcpTrak`(`Execute`) - Get process point information
- `MC_BR_ProcPointReadInfo_AcpTrak`(`Enable`) - Read process point information cyclically
- `MC_BR_BarrierCommand_AcpTrak`(`Execute`) - Control barrier:
  - `mcACPTRAK_BARRIER_OPEN` - Open (all pass)
  - `mcACPTRAK_BARRIER_CLOSE` - Close (none pass)
  - `mcACPTRAK_BARRIER_ADD_TICKETS` - Add N tickets. Set `AdvancedParameters.TicketCount`
- `MC_BR_BarrierReadInfo_AcpTrak`(`Enable`) - Read barrier state/ticket count
- `MC_BR_BarrierGetShuttle_AcpTrak`(`Enable`) - Get shuttles waiting at barrier

## Convoy function blocks

- `MC_BR_ShCreateConvoy_AcpTrak`(`Execute`) - Create convoy with shuttle as master
- `MC_BR_ShMoveInConvoy_AcpTrak`(`Execute`) - Add shuttle/convoy to convoy
- `MC_BR_ShMoveCycInConvoy_AcpTrak`(`Enable`) - Cyclic target distance in convoy
- `MC_BR_ShRemoveFromConvoy_AcpTrak`(`Execute`) - Remove shuttle from convoy
- `MC_BR_ConDelete_AcpTrak`(`Execute`) - Delete convoy
- `MC_BR_ConGetShuttle_AcpTrak`(`Enable`) - Get shuttles in convoy
- `MC_BR_ConReadInfo_AcpTrak`(`Enable`) - Read convoy information


## Barrier ticketing + non-blocking single-shuttle control pattern

- Station objective:
	- Hold incoming shuttles at a barrier.
	- Control shuttles one by one.
	- Keep station logic cyclic and non-blocking.

- Required sequence:
	- Initialize barrier ticketing when `gTrakAsm.Status.ReadyForMovement = TRUE`.
	- Set `MC_BR_BarrierCommand_AcpTrak` command to ticket mode (`mcACPTRAK_BARRIER_ADD_TICKETS`) and configure `TicketCount` explicitly.
	- Enable trigger point (`MC_BR_TrgPointEnable_AcpTrak`) and detect incoming events via `EventCount > 0` (NOT `EventCount > OldValue`).
	- Read shuttle axis references using `MC_BR_TrgPointReadInfo_AcpTrak` and store them in station-managed storage (array or queue).
	- **IMPORTANT:** `MC_BR_TrgPointReadInfo_AcpTrak` reduces `EventCount` when executed. Therefore, always check `EventCount > 0` instead of comparing to a stored old value.
	- Start shuttle moves individually (`MC_BR_ModuloMoveAbs_AcpTrak`, `MC_BR_RoutedMoveVel_AcpTrak`) with pulsed `Execute` signals. Set an information flag (e.g., LastShuttleMoveDone) to indicate when the shuttle has finished moving and to trigger other actions.
	- Advance station state on edges/status (`Active` edge, `Acknowledge`, or equivalent), not by blocking for all moves to finish.

- Function block execution rules:
	- Assign all FB inputs before the call.
	- Call FBs cyclically at the end of the ACTION.
	- Reset `Execute` after call evaluation in the same scan.
	- Keep explicit old-value edge detection variables for `Active`/`Done` transitions when needed.

- Edge detection for sequential shuttle commands:
	- When sending multiple shuttles sequentially using one FB instance, detect the rising edge of `Active` to trigger the next command.
	- Use an `ActiveOld` variable (e.g., `RoutedMoveVelActiveOld`) to detect the transition from `Active = FALSE` to `Active = TRUE`.
	- Pattern: `IF FB.Active AND NOT ActiveOld THEN (* trigger next *) END_IF`
	- IMPORTANT: Set `Execute := FALSE` immediately after detecting the edge, so the FB sees a positive edge when `Execute` is set `TRUE` again for the next shuttle.
	- Update `ActiveOld := FB.Active` at the end of the ACTION, after the FB call.

- Error handling:
	- For each station FB, evaluate `Error` and route to a defined recovery state.
	- Set `gTrakAsm.Status.StationError := TRUE` on station-level errors.
	- Re-enter the ticketing initialization state after recoverable errors.

- Safety and robustness:
	- Protect shuttle index counters against array bounds.
	- Do not issue parallel movement commands through one FB instance unless that behavior is explicitly intended and validated.
	- Keep command generation deterministic: one state transition reason per cycle.

- State machine structure:
	- Do NOT create separate states for each shuttle (e.g., `PS_MOVE_SHUTTLE_1`, `PS_MOVE_SHUTTLE_2`). Use a single state with an index/counter.
	- Calculate position dynamically based on shuttle index (e.g., `Position := 1.0 - 0.2 * ShuttleCount`), or use given values in an array.
	- After triggering a move, check if more shuttles need processing: if yes, loop back to trigger state; if no, advance to the next phase.
	- Store shuttle axes in an array indexed by the shuttle counter.

## Common mistakes to avoid (MUST NOT)

### Code Structure Errors
- **DO NOT** place ACTION definitions inline in `Main.st`. Create a separate `.st` file for each ACTION (e.g., `ProcStation.st`) and register it in `IEC.prg`.
- **DO NOT** register ACTION files in `Cpu.sw` as separate tasks. ACTIONs are part of the parent task, not standalone tasks.
- **DO NOT** forget to call the ACTION in `Main.st` CYCLIC section after the assembly FB calls.

### State Machine Errors
- **DO NOT** combine `PS_SEND_OUT` and `PS_SEND_OUT_WAIT` into a single state. Always use two separate states.
- **DO NOT** create separate states for each shuttle (e.g., `PS_MOVE_SHUTTLE_1`, `PS_MOVE_SHUTTLE_2`). Use a single state with an index/counter.
- **DO NOT** skip `PS_SEND_OUT_WAIT` state — it ensures clean cycle restart.

### Function Block Errors
- **DO NOT** use `MC_BR_BarrierReadInfo_AcpTrak` for the standard ticketing pattern. Use only `MC_BR_BarrierCommand_AcpTrak` with `mcACPTRAK_BARRIER_ADD_TICKETS`.
- **DO NOT** forget the edge detection variable (e.g., `RoutedMoveVelActiveOld`). It must be declared in Variables.var and updated after the FB call.
- **DO NOT** check `EventCount > EventCountOld` for trigger point events. `MC_BR_TrgPointReadInfo_AcpTrak` decrements `EventCount` when executed. Always use `EventCount > 0` instead.
- **DO NOT** invent new function blocks or patterns. Copy from the Golden Example as closely as possible.

### Configuration Errors
- **DO NOT** forget to set `MaxShuttleCount` in the assembly when using `InitShuttleSim`. Must be >= number of shuttles defined.
- **DO NOT** forget to reference `InitShuttles` in the assembly's `AssemblyFeatures` group.
- **DO NOT** use wrong ProcessPoint ID in code — must match exactly the ID in `*.processpoint` file.

## Golden example: ProcessStation with barrier ticketing + non-blocking sequence

```iec-st
ACTION ProcessStation:

	CASE ProcStation_Step OF

		PS_INIT_TICKETING:
			IF gTrakAsm.Status.ReadyForMovement THEN
				MC_BR_BarrierCommand_0.Command := mcACPTRAK_BARRIER_ADD_TICKETS;
				MC_BR_BarrierCommand_0.AdvancedParameters.TicketCount := 2;
				MC_BR_BarrierCommand_0.Execute := TRUE;
				ProcStation_Step := PS_ENABLE_TRIGGER;
			END_IF

		PS_ENABLE_TRIGGER:
			MC_BR_TrgPointEnable_0.Enable := TRUE;
			IF MC_BR_TrgPointEnable_0.EventCount > 0 THEN
				MC_BR_TrgPointReadInfo_0.Execute := TRUE;
				ProcStation_Step := PS_MOVE_TO_POSITION;
			END_IF

		PS_MOVE_TO_POSITION:
			IF MC_BR_TrgPointReadInfo_0.Done THEN
				ProcStation_Axis[ShuttleIndex] := MC_BR_TrgPointReadInfo_0.TrgPointInfo.Axis;
				MC_BR_ModuloMoveAbs_0.Axis := ADR(ProcStation_Axis[ShuttleIndex]);
				MC_BR_ModuloMoveAbs_0.Position := 1.0 - 0.2 * ShuttleIndex; (* dynamic position per shuttle *)
				MC_BR_ModuloMoveAbs_0.Velocity := 2.0;
				MC_BR_ModuloMoveAbs_0.Execute := TRUE;
				ShuttleIndex := ShuttleIndex + 1;
				IF ShuttleIndex < TICKET_COUNT THEN
					ProcStation_Step := PS_ENABLE_TRIGGER; (* loop back for next shuttle *)
				ELSE
					ProcStation_Step := PS_SEND_OUT; (* all shuttles collected *)
					ShuttleIndex := 0;
				END_IF
			ELSIF MC_BR_TrgPointReadInfo_0.Error THEN
				gTrakAsm.Status.StationError := TRUE;
				ProcStation_Step := PS_INIT_TICKETING;
			END_IF

		(* Send out shuttles one by one using edge detection on Active *)
		PS_SEND_OUT:
			MC_BR_RoutedMoveVel_ProcStation.Axis := ADR(ProcStation_Axis[ShuttleCount]);
			MC_BR_RoutedMoveVel_ProcStation.Sector := ADR(BaseSector_1_1);
			MC_BR_RoutedMoveVel_ProcStation.Position := 0.0;
			MC_BR_RoutedMoveVel_ProcStation.Velocity := 2.0;
			MC_BR_RoutedMoveVel_ProcStation.RouteVelocity := 0.0;
			MC_BR_RoutedMoveVel_ProcStation.Acceleration := 10.0;
			MC_BR_RoutedMoveVel_ProcStation.Deceleration := 10.0;
			MC_BR_RoutedMoveVel_ProcStation.Jerk := 0.0;
			MC_BR_RoutedMoveVel_ProcStation.BufferMode := mcABORTING;
			MC_BR_RoutedMoveVel_ProcStation.AdvancedParameters.StartDirection := mcDIR_UNDEFINED;
			MC_BR_RoutedMoveVel_ProcStation.AdvancedParameters.EndDirection := mcDIR_POSITIVE;
			MC_BR_RoutedMoveVel_ProcStation.AdvancedParameters.ShuttleOrientation := mcDIR_UNDEFINED;
			MC_BR_RoutedMoveVel_ProcStation.Execute := TRUE;

			(* IMPORTANT: Detect rising edge of Active to trigger next shuttle.
			   Execute must be reset to FALSE after FB call so the next iteration
			   sees a positive edge when Execute is set TRUE again. *)
			IF MC_BR_RoutedMoveVel_ProcStation.Active AND NOT RoutedMoveVelActiveOld THEN
				ShuttleCount := ShuttleCount + 1;
				MC_BR_RoutedMoveVel_ProcStation.Execute := FALSE;
			END_IF

			(* All shuttles sent: transition to wait state *)
			IF ShuttleCount = TICKET_COUNT THEN
				ProcStation_Step := PS_SEND_OUT_WAIT;
				ShuttleCount := 0;
			END_IF

			IF MC_BR_RoutedMoveVel_ProcStation.Error THEN
				gTrakAsm.Status.StationError := TRUE;
				ProcStation_Step := PS_INIT_TICKETING;
			END_IF

		PS_SEND_OUT_WAIT:
			(* Wait until all shuttles have left, then restart ticketing *)
			ProcStation_Step := PS_INIT_TICKETING;

	END_CASE

	RoutedMoveVelActiveOld := MC_BR_RoutedMoveVel_ProcStation.Active;

	MC_BR_TrgPointEnable_0.ProcessPoint := ADR(ProcessPoint_Station);
	MC_BR_TrgPointEnable_0();

	MC_BR_TrgPointReadInfo_0.ProcessPoint := ADR(ProcessPoint_Station);
	MC_BR_TrgPointReadInfo_0();
	MC_BR_TrgPointReadInfo_0.Execute := FALSE;

	MC_BR_BarrierCommand_0.ProcessPoint := ADR(ProcessPoint_Station);
	MC_BR_BarrierCommand_0();
	MC_BR_BarrierCommand_0.Execute := FALSE;

	MC_BR_ModuloMoveAbs_0();
	MC_BR_ModuloMoveAbs_0.Execute := FALSE;

	MC_BR_RoutedMoveVel_ProcStation();
	MC_BR_RoutedMoveVel_ProcStation.Execute := FALSE;

END_ACTION
```
