# UE-AssetClosureCollector

[中文文档 / Chinese README](./README.zh-CN.md)

An Unreal Engine editor plugin for collecting the full dependency closure of one or more assets, previewing the copy plan, and optionally copying the assets into a target content folder with cleanup support.

## Project Overview

UE-AssetClosureCollector is an Unreal Engine editor plugin focused on dependency-aware asset collection workflows. It helps technical artists, tool developers, and content teams identify the full dependency closure of selected assets, preview the resulting collection plan, and copy those assets into a controlled target content root with optional cleanup.

The plugin is designed for asset packaging, content extraction, internal handoff, outsourcing delivery, and reuse preparation. It provides safer execution through Dry Run previews, execution confirmation dialogs, target path validation, and bilingual editor UI support.

## AI Assistance Disclosure

This plugin's implementation plan, source scaffolding, iterative code changes, and repository documentation were produced through a human-in-the-loop collaboration workflow with OpenAI Codex, a GPT-5-based coding agent. Final integration, validation, and repository management remain the responsibility of the project owner.

## Features

- Analyze one or more source assets at once
- Supports `/Game/...` asset paths
- Supports local `.uasset` file paths under the project's `Content` folder
- Recursively collect dependency closures
- Supports:
  - Hard dependencies
  - Soft dependencies
  - Manage dependencies
- Preview the copy plan before execution
- Optional target-folder cleanup
- Dry Run mode
- Execution confirmation before real copy/cleanup
- Bilingual UI
- Multiple asset input
- Fast source selection from local file picker or Unreal Content Browser
- Built-in usage guide window inside the tool

## Typical Use Cases

- Extract a reusable asset pack from an existing project
- Collect all dependencies required by a character, prop, effect, or blueprint
- Build a clean delivery folder for outsourcing or handoff
- Rebuild a target content folder so it only contains assets required by the current selection

## Current Workflow

1. Enter one or more source assets, one per line
2. Choose a target root path under `/Game/...`
3. Select dependency options
4. Run `Analyze Closure`
5. Review:
   - dependency analysis result
   - copy plan
   - cleanup plan
6. Choose one of:
   - `Copy Assets`
   - `Copy + Clean Target`

If `Dry Run` is enabled, execution only previews the result and does not modify assets.

## Installation

This repository contains the plugin source only.

### Option A: Install as a Project Plugin

Recommended for most users.

1. Open your Unreal project folder.
2. Create this folder if it does not exist:

```text
<YourProject>/Plugins/
```

3. Copy the entire `AssetClosureCollector` plugin folder into:

```text
<YourProject>/Plugins/AssetClosureCollector/
```

4. Your final layout should look like:

```text
<YourProject>/
  Plugins/
    AssetClosureCollector/
      AssetClosureCollector.uplugin
      Source/
```

5. Open the `.uproject`.
6. If Unreal asks to rebuild modules, choose `Yes`.
7. After compilation, open the editor.
8. Go to:

```text
Window -> Asset Closure Collector
```

### Option B: Install as an Engine Plugin

Use this if you want the plugin to be available to multiple projects using the same engine build.

1. Locate your Unreal Engine installation or source-built engine directory.
2. Copy the plugin folder into:

```text
<UE_ROOT>/Engine/Plugins/AssetClosureCollector/
```

3. Final layout:

```text
<UE_ROOT>/
  Engine/
    Plugins/
      AssetClosureCollector/
        AssetClosureCollector.uplugin
        Source/
```

4. Regenerate project files if needed.
5. Build the engine or let Unreal rebuild the plugin the next time a project starts.

## Supported Engine Usage

This plugin was developed against Unreal Engine 5.6 editor workflow.

It is an **Editor-only plugin** and is not intended for runtime or shipping gameplay use.

## How to Use

### 1. Open the Tool

In Unreal Editor:

```text
Window -> Asset Closure Collector
```

### 2. Add Source Assets

You can add source assets in three ways:

- Type `/Game/...` asset paths directly
- Paste local `.uasset` file paths from the current project's `Content` directory
- Use:
  - `Add File`
  - `Add Asset`

Each line represents one root asset.

### 3. Set Target Root Path

The target root path must:

- start with `/Game/`
- not be exactly `/Game`

Example:

```text
/Game/CollectedAssets
```

### 4. Choose Dependency Options

- `Include Hard`
- `Include Soft`
- `Include Manage`
- `Recursive`
- `Dry Run`

### 5. Analyze

Click:

```text
Analyze Closure
```

The tool will generate:

- asset analysis result
- copy plan
- target cleanup plan

### 6. Execute

- `Copy Assets`
  - Copies assets in the current closure into the target root
- `Copy + Clean Target`
  - Copies closure assets and removes target assets not included in the current closure

When `Dry Run` is off, the plugin shows a confirmation dialog before executing.

## Deployment to Your Own Engine or Team Environment

### For One Project

Use project plugin deployment:

```text
<Project>/Plugins/AssetClosureCollector/
```

This is easier to manage per project and keeps the plugin version tied to that project.

### For Many Projects on the Same Engine

Use engine plugin deployment:

```text
<UE_ROOT>/Engine/Plugins/AssetClosureCollector/
```

This is useful when:

- multiple projects use the same engine build
- your team wants one centrally installed editor tool

After copying the plugin:

- source-built engine users should rebuild
- installed-engine users may be prompted to rebuild plugin modules on startup

## Source Layout

```text
AssetClosureCollector/
  AssetClosureCollector.uplugin
  Source/
    AssetClosureCollectorEditor/
      AssetClosureCollectorEditor.Build.cs
      Public/
      Private/
```

## Notes and Limitations

- The plugin currently targets editor workflows, not runtime use.
- Cleanup only affects the **target root folder**, not the original source asset location.
- The target cleanup count only includes assets under the target root that are **not part of the current closure**.
- Empty folders are not treated as cleanup assets.
- Behavior depends on Unreal's asset registry and editor asset APIs.
- Always verify results with `Dry Run` before large cleanup operations.

## Recommended Safety Workflow

1. Enable `Dry Run`
2. Analyze the closure
3. Review copy and cleanup plans
4. Disable `Dry Run`
5. Execute after confirmation

## Repository Contents

This repository is intended to store source files only.

Do not commit generated folders such as:

- `Binaries/`
- `Intermediate/`
- `DerivedDataCache/`
- `Saved/`

## License

See the repository `LICENSE` file.
