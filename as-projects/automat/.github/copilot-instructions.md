# Copilot Instructions

This repository is a B&R Automation Studio project. Use this file as the always-on project guide for coding agents working in GitHub Copilot Chat or other AGENTS.md-compatible tools.

## Project overview

- Target environment: Windows, B&R Automation Studio, PowerShell.
- Main project file: `*.apj`.
- Primary source areas:
  - `Logical/` for IEC 61131-3 code, global variables/types, Unity integration, libraries, and mappView assets.
  - `Physical/<config>/<cpu>/` for hardware, CPU software, I/O mapping, and PV mapping.
  - `Requirements/` for specifications.
- Generated or build output areas:
  - `Binaries/`
  - `Diagnosis/`
  - `Temp/`

## Working rules

- Prefer small, local changes over broad refactors.
- Fix the root cause in source files instead of patching generated outputs.
- Do never edit generated files in `Binaries/`, `Diagnosis/`, or `Temp/`
- Preserve the existing Automation Studio project structure.
- If you add, remove, or rename files in Automation Studio packages, update the corresponding `Package.pkg` file.
- If you add a new Structured Text task, use the task `AS API: Add ST Task`

## Source file guidance

### General — IEC 61131-3 Code, Types, Variables

**MUST** read the following before editing IEC code in /Logical:

- Collection: [as-project-base](.github/collections/as-project-base.collection.yml)
- Instructions: [as-project-code.instructions.md](.github/instructions/as-project-code.instructions.md)

### General — ANSI C Code and Headers

**MUST** read the following before editing ANSI C code in /Logical:

- Collection: [as-project-base](.github/collections/as-project-base.collection.yml)
- Instructions: [as-project-code-ansic.instructions.md](.github/instructions/as-project-code-ansic.instructions.md)

### mappView HMI — Visualization

**MUST** read the following before working on mappView visualization:

- Collection: [as-project-visu](.github/collections/as-project-visu.collection.yml)
- Instructions: [as-project-visu.instructions.md](.github/instructions/as-project-visu.instructions.md)

### Physical View — Hardware, CPU, I/O, PvMap, Access & Security

**MUST** read the following before working on physical configuration, user handling and variable mappings:

- Collection: [as-project-base](.github/collections/as-project-base.collection.yml)
- Instructions: [as-project-config.instructions.md](.github/instructions/as-project-config.instructions.md)

### Connectivity — OPC UA

**MUST** read the following before working on connectivity features like OPC UA:

- Collection: [as-project-connectivity](.github/collections/as-project-connectivity.collection.yml)
- Instructions: [as-project-opcua.instructions.md](.github/instructions/as-project-opcua.instructions.md)

### mapp Services — Alarms, AlarmX

**MUST** read the following before working on mapp Services features like Alarms and AlarmX:

- Collection: [as-project-mpservices](.github/collections/as-project-mpservices.collection.yml)
- Instructions: [as-project-mpalarmx.instructions.md](.github/instructions/as-project-mpalarmx.instructions.md)

### mapp Motion — Axis, Drive, Motion Control

**MUST** read the following before working on mapp Motion features like Axis, Drive, Motion Control and mappTrak:

- Collection: [as-project-mpmotion](.github/collections/as-project-mpmotion.collection.yml)
- Collection: [as-project-mapptrak](.github/collections/as-project-mapptrak.collection.yml)

## Preferred workflow

1. Read the relevant requirement or nearby implementation before editing.
2. Change the smallest source surface that controls the behavior.
3. If project structure changes are required, update package registration files in the same change.
4. Validate with the narrowest useful check before making more edits.

## Build and validation

Use the existing workspace tasks when possible.

- Preferred build task: `AS: Build`
- Full configuration build: `AS: Build All Configurations`
- Transfer flow when explicitly requested: `AS: Build and Transfer`

## Automation Studio API helpers

This repo already includes scripts for project-safe structural changes.

- Add libraries with `AS Logical API: Add Library`
- Add Structured Text tasks with `AS Logical API: Add ST Task`
- Add hardware modules with `AS Physical HW API: Add HW Module`
- Discover allowed technology-package config elements with `AS TP Config API: Get Config Elements` (and `AS TP Config API: List File Endings`) before hand-editing files under `Physical/<config>/<cpu>/mapp*`

Prefer those helpers over hand-editing project metadata when they match the task.

## Agent expectations

- Be explicit about assumptions when requirements are incomplete.
- Do not rewrite unrelated files to match a preferred style.
- Do not modify user or machine-specific settings files unless the task explicitly asks for it.
- Prefer repository tasks and existing scripts over inventing new build or setup flows.
