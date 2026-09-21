---
description: Guidelines for creating and configuring mapp Motion axis files from commit-based patterns (axis types, limits, scaling, and rule-of-thumb tuning).
applyTo: 'Physical/**/*.axis'
---

# B&R Automation Studio mapp Motion Axis Configuration Guidelines

Use these rules when creating or editing axis configuration files in `Physical/`.

## Scope

- Edit only Automation Studio project source files (typically `Physical/`, `Logical/`, and the project `*.apj`).
- Never edit generated files in `Binaries/`, `Diagnosis/`, or `Temp/`.
- If you add, remove, or rename axis files, update the matching `Package.pkg`.

## Adding axis configuration

Use this sequence when adding an axis based on the current project baseline.

1. If not done yet, register required motion libraries in logical package:
   - File: `Logical/Libraries/Package.pkg`
   - Use the `as-logical-api` skill to insert these libraries as `<Object Type="Library">` entries:
     - `McBase`
     - `MpBase`
     - `MpAxis`
     - `McAxis`
   - Use the `as-logical-api` skill to add the hardware-specific library matching the drive type (these are often not added automatically):
     - `McAcpAx` -> ACOPOS servo drives -> ACOPOS axis
     - `McStpAx` -> X20SM / ACOPOS Micro stepper -> B&R stepper motor axis
     - `McPuveAx` -> purely virtual axis
     - `McDS402Ax` -> Powerlink DS402 third-party drives and ACOPOSinverter -> DS402 axis

2. If not done yet, register mapp Motion in the project file:
   - File: `*.apj`
   - Ensure `<TechnologyPackages>` contains `mappMotion` with the project-aligned version.
   - The `McDriveLog` version has to be explicitly defined as well, matching the used mapp Motion version.
     - Example:
       ```xml
       <TechnologyPackages>
         <mappMotion Version="6.5.1" McDriveLog="6.5.1" />
       </TechnologyPackages>
       ```

3. Create the axis configuration file:
   - File: `Physical/<Configuration>/<CPU>/mappMotion/*.axis`
   - Register each added axis file in `Physical/<Configuration>/<CPU>/mappMotion/Package.pkg`.
   - (optional) Keep naming and placement aligned with the axis object name used in the axis file. As long as the axes belong to the same mechatronic unit, they can be configured inside one `*.axis` file - then use the name of the mechatronic unit for the file name.

## Axis Types

Configure the BaseType according to the specifications or the intended machine behavior. Configure movement limits with physically achievable velocity and acceleration values. Set measurement resolution according to the required positioning accuracy. For linear axes, define the rotary to linear transformation. If a gearbox is present, configure the gear ratio.  

Behavior note from MC_ReadActualPosition:

- Bounded types (linear bounded, rotary bounded) report absolute position within bounded range.
- Periodic types (linear periodic, rotary periodic) report position within [0, Period).
- Non-bounded, non-periodic types (linear, rotary) use absolute position with overflow behavior.

### Linear

- E.g., conveyor belt (endless)
- No defined bounds
- Direct homing is usually sufficient

### Linear Bounded

- e.g., ball screw
- Define position limits consistent with mechanical stroke.

### Rotary Periodic

- Use periodic behavior for cyclic angular motion.

### Linear periodic:

- Conveyor tracking position source in mapp Robotics/CNC.
- AS help example requires linear periodic for tracking-path axis source.
- Can be also used for endless conveyor belts, since the modulo position can make the application logic easier. For this, the period should be set to the actual physical length of one conveyor belt loop. If the loop length is not known/defined, use the linear (non-periodic, unbounded) type instead.

### Rotary:

- Continuous spindle, unwinder, or shaft where absolute overflow behavior is acceptable.

### Rotary bounded:

- Swivel axis, valve, or index axis with mechanical end stops and bounded angular travel.

## Measurement units

- Select measurement units based on machine specifications and the BaseType. Use millimeters/degrees when not specified.

## Count direction

- Define count direction based on the physical setup and the desired positive direction for motion commands. Standard = clockwise, negative = counterclockwise.

## Rule of Thumb: Accuracy and Resolution

- Required positioning accuracy comes from machine specification.
- Measurement resolution should be at least 10x finer than positioning accuracy when feasible by the encoder.
- Example relationship from commit patterns:
  - Positioning accuracy 0.1 mm => resolution around 0.01 mm or finer.
  - Positioning accuracy 0.01 mm => resolution around 0.001 mm or finer.
  - Positioning accuracy 0.01 deg => resolution around 0.001 deg or finer.
- If hardware limits prevent 10x, use the finest stable resolution supported and document the deviation.

## Rule of Thumb: Velocity and Acceleration

- Velocity, acceleration, and deceleration must be mechanically plausible.
- Start with acceleration approximately 10x velocity as an initial engineering estimate.
- Reduce acceleration ratio for low dynamic applications.
- Increase only when motor torque, mechanics, and safety margins are verified.
- Keep acceleration and deceleration symmetric by default unless process behavior requires otherwise.

## Drive and Assignment

- Add and configure a valid drive/channel so the axis can be assigned in hardware configuration, or in case of a purely virtual axis, add the `*.purevaxcfg` file and assign the axis to it.
- Do not hardcode one drive family as universal; select drive and channel setup to match the application.

## Mechanical elements

- For linear axes, configure the rotary to linear transformation when specified.
- Configure the gear ratio when specified.
- For DS402 third-party drives, do the gear ratio on the B&R side, not on the drive; configure drive units so encoder increments are passed per motor revolution.

## Homing and Limits

- Homing mode must match available sensors/encoder references and axis type.
- Bounded axes should have homing and limit logic aligned with end-stop/reference hardware.
- Periodic axes should use homing/reference strategy that provides repeatable modulo position.
- For limitless axes, direct homing is usually sufficient, but consider mechanical and safety implications.

## Movement error limits

- If specified, set movement error limits.

## Jerk and zero vibration filter

- If specified, set jerk and zero vibration filter parameters.
- Limit jerk to protect mechanics and avoid exciting resonances. Typical accel rise time: low ~10 s, normal 0.1-1 s, high ~0.01 s.
- The jerk filter smooths the movement curve; max is 0.4 s for ACOPOS P3 and 0.2 s for ACOPOSmulti/ACOPOSmicro.

## Minimal Creation Workflow

1. Add axis file in mapp Motion package and set axis object name.
2. Update Package.pkg if file structure changed.
3. Choose correct BaseType for machine kinematics.
4. Set units and measurement resolution.
5. Set velocity, acceleration, deceleration using rule-of-thumb as a start.
6. Set position limits or period according to bounded/periodic behavior.
7. Assign the axis to a drive channel or pure virtual axis configuration.
8. Configure gear ratio and rotary to linear transformation if applicable.
9. Configure homing to match physical reference method.
10. Configure movement error limits if specified.
11. Configure jerk and zero vibration filter if specified.


# B&R Automation Studio mapp Motion Application Program Guidelines

By default use the function blocks from the `MpAxis` technology library. If user specifies to use the PLCOpen function blocks, use the `McAxis` core library instead. It is also possible to combine function blocks from both - e.g. use `MpAxisBasic` function block for the general axis control and function blocks from `McAxis` for specific functionality and advanced features. If needed, drive-specific function blocks can be found in the drive-specific library (e.g. `McAcpAx`, `McStpAx`, `McPuveAx`, `McDS402Ax`, depending on the used drive type).  

 The list of available function blocks and their inputs/outputs with basic description can be checked directly in the libraries. For further information refer to the Automation Studio help using the B&R Automation Studio Help Server (via `mcp_b_r_automatio_*` tools), e.g. *Motion control > mapp Motion > mapp Axis > Programming* and *... > MpAxis > Function blocks > MpAxisBasic*. The help is the source of truth.

## Project conventions

- Each `*.axis` element `ID` is auto-generated as a global variable of type `McAxisType`
  (e.g. `gAxis_BallScrew`, `gAxis_Conveyor`, `gAxis_RotaryPeriodic`, `gAxis_1`). Use reference (address) to this global variable as the `MpLink` (called as `Axis` in the function blocks from `McAxis` library) target — do not declare it yourself.
- Add the control logic as a Structured Text task in the Logical View using the
  `as-logical-api` skill.
- One `MpAxisBasic` instance and one `MpAxisBasicParType` structure control exactly one axis.
  For multiple axes, create one instance/structure pair per axis. If using the `McAxis` library, create one instance of each needed function block per axis as well.

## Engineering rules

- Assign `MpLink` and `Parameters` in the **cyclic** program section, never in `_INIT` (see
  INIT/EXIT and transfer rules below).
- Set motion parameters before triggering a command. Make sure the values are valid and mechanically plausible, keeping in mind the axis configuration (limits, resolution). Reuse the rule-of-thumb for velocity-acceleration ratio from the axis configuration section above.
- Only start a movement after the axis is powered (`PowerOn`) and, when the axis type requires
  it, homed (`IsHomed`).
- Perform the axis preparation (power-on, homing) in a state machine.

## INIT/EXIT subroutines and transfer settings

- **Do not assign pointers in `_INIT`.** Pointer inputs such as `MpLink` and `Parameters`
  (and any address inputs) must be assigned in `_CYCLIC`.
- `_INIT` may be used for non-pointer, value initialization (e.g. `Enable`, default parameter
  values).
- A dedicated `_EXIT` is normally not required for mapp Motion tasks.

## Minimal program example

Example code skeleton (adapt as needed for your application):

```iecst
VAR
    MpAxisBasic_0 : MpAxisBasic;
    AxisPar       : MpAxisBasicParType;
END_VAR
```

```iecst
// _INIT
MpAxisBasic_0.Enable := TRUE;
AxisPar.Velocity     := 100.0;
AxisPar.Acceleration := 1000.0;
AxisPar.Deceleration := 1000.0;
AxisPar.Position     := 50.0;
```

```iecst
// _CYCLIC
MpAxisBasic_0.MpLink := ADR(gAxis_BallScrew);   // axis object ID from the *.axis file
MpAxisBasic_0.Parameters := ADR(AxisPar);

CASE State OF
    STATE_POWER_OFF:
        MpAxisBasic_0.Power := FALSE;
        IF MpAxisBasic_0.Info.ReadyToPowerOn AND CmdPowerOn THEN
            State := STATE_POWER_ON;
        END_IF

    STATE_POWER_ON:
        MpAxisBasic_0.Power := TRUE;
        IF MpAxisBasic_0.PowerOn AND NOT MpAxisBasic_0.IsHomed THEN
            State := STATE_HOME;
        END_IF

    STATE_HOME:
        MpAxisBasic_0.Home := TRUE;
        IF MpAxisBasic_0.IsHomed THEN
            MpAxisBasic_0.Home := FALSE;
            State := STATE_READY;
        END_IF

    STATE_READY:
        // Wait for motion command, set parameters, trigger move, etc.

    STATE_ERROR:
        // Handle error, reset, etc.
END_CASE

MpAxisBasic_0();                                     // call at the end of the cycle
```


## Minimal application workflow

1. Add (or confirm) the axis configuration and its drive/pure-virtual assignment.
2. Add an ST task with the `as-logical-api` skill.
3. Declare one `MpAxisBasic` instance and one `MpAxisBasicParType` structure (or an instance of the corresponding function blocks from the `McAxis` library) per axis.
4. Wire `MpLink` (`Axis` in the `McAxis` library) to the axis object and `Parameters` to the parameter structure.
5. Enable the FB and wait until it reports communication ready.
6. Create a state machine for axis preparation
    - If `ReadyToPowerOn` is TRUE, axis can be powered on.
    - If `PowerOn` is TRUE, and `IsHomed` is FALSE, axis can be homed (if required by the axis type).
    - If `IsHomed` is TRUE, axis can be moved and is waiting for motion command.
7. Set motion parameters and trigger the desired move command; wait for its done/active output.
8. Continuously monitor `Error`/`StatusID`.

