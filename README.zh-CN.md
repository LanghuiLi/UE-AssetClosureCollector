# UE-AssetClosureCollector

[English README](./README.md)

一个用于 Unreal Engine 编辑器的资源依赖收集插件，可分析一个或多个资源的完整依赖闭包，预览复制计划，并可选择将资源复制到目标内容目录，同时支持目标目录清理。

## 项目简介

UE-AssetClosureCollector 是一个面向 Unreal Engine 编辑器的资源依赖收集插件，专注于“按资源闭包进行分析、整理与复制”的工作流。它适用于技术美术、工具开发者以及内容团队，用于分析指定资源的完整依赖关系、预览收集结果，并将相关资源复制到受控的目标内容目录中，同时可选清理目标目录中的无关资源。

该插件适合用于资源打包、内容提取、团队交接、外包交付以及可复用资源整理等场景。当前版本提供 Dry Run 演练模式、执行前确认、目标路径校验以及中英文双语界面，以提升实际项目中的安全性和可用性。

## AI 生成与协作声明

本插件的方案设计、源码骨架、迭代代码实现以及仓库文档，均是在人工参与审核的前提下，基于 OpenAI Codex（GPT-5 系列编码代理）协作生成完成。最终的集成、验证、测试与发布责任由项目维护者承担。

## 功能特性

- 支持同时分析一个或多个源资源
- 支持 `/Game/...` 资源路径
- 支持项目 `Content` 目录下的本地 `.uasset` 文件路径
- 递归收集资源依赖闭包
- 支持：
  - 硬依赖
  - 软依赖
  - 管理依赖
- 执行前预览复制计划
- 可选清理目标目录
- 支持 Dry Run 演练模式
- 正式执行前确认弹窗
- 中英文双语界面
- 多资源输入
- 支持本地文件选择器与 Unreal 内容浏览器快速选源
- 内置使用说明窗口

## 典型使用场景

- 从现有项目中提取可复用资源包
- 收集角色、道具、特效或蓝图所需的完整依赖资源
- 为外包交付或团队交接构建干净的目标资源目录
- 重建目标内容目录，只保留当前选择资源所需内容

## 当前工作流

1. 输入一个或多个源资源，每行一个
2. 选择 `/Game/...` 下的目标根路径
3. 勾选依赖选项
4. 执行 `Analyze Closure`
5. 查看：
   - 依赖分析结果
   - 复制计划
   - 清理计划
6. 执行以下之一：
   - `Copy Assets`
   - `Copy + Clean Target`

如果启用了 `Dry Run`，执行只会预览结果，不会真正修改资源。

## 安装方式

本仓库仅包含插件源码。

### 方案 A：作为项目插件安装

推荐大多数用户使用。

1. 打开你的 Unreal 项目目录。
2. 如果不存在，创建以下目录：

```text
<YourProject>/Plugins/
```

3. 将整个 `AssetClosureCollector` 插件目录复制到：

```text
<YourProject>/Plugins/AssetClosureCollector/
```

4. 最终目录结构应如下：

```text
<YourProject>/
  Plugins/
    AssetClosureCollector/
      AssetClosureCollector.uplugin
      Source/
```

5. 打开 `.uproject`。
6. 如果 Unreal 提示需要重建模块，选择 `Yes`。
7. 编译完成后打开编辑器。
8. 进入：

```text
Window -> Asset Closure Collector
```

### 方案 B：作为引擎插件安装

如果你希望同一个引擎版本下的多个项目都能使用该插件，可以采用这种方式。

1. 找到你的 Unreal Engine 安装目录或源码引擎目录。
2. 将插件目录复制到：

```text
<UE_ROOT>/Engine/Plugins/AssetClosureCollector/
```

3. 最终目录结构如下：

```text
<UE_ROOT>/
  Engine/
    Plugins/
      AssetClosureCollector/
        AssetClosureCollector.uplugin
        Source/
```

4. 如有需要，重新生成工程文件。
5. 重新构建引擎，或在下次项目启动时让 Unreal 自动重建插件模块。

## 支持的引擎使用方式

本插件基于 Unreal Engine 5.6 编辑器工作流开发。

它是一个 **仅编辑器使用的插件**，不面向运行时或打包后的游戏逻辑使用。

## 使用方法

### 1. 打开工具

在 Unreal Editor 中：

```text
Window -> Asset Closure Collector
```

### 2. 添加源资源

你可以通过三种方式添加源资源：

- 直接输入 `/Game/...` 资源路径
- 粘贴当前项目 `Content` 目录下的本地 `.uasset` 文件路径
- 使用：
  - `Add File`
  - `Add Asset`

每一行表示一个根资源。

### 3. 设置目标根路径

目标根路径必须：

- 以 `/Game/` 开头
- 不能直接等于 `/Game`

示例：

```text
/Game/CollectedAssets
```

### 4. 选择依赖选项

- `Include Hard`
- `Include Soft`
- `Include Manage`
- `Recursive`
- `Dry Run`

### 5. 分析

点击：

```text
Analyze Closure
```

工具会生成：

- 资源分析结果
- 复制计划
- 目标清理计划

### 6. 执行

- `Copy Assets`
  - 将当前闭包中的资源复制到目标根路径
- `Copy + Clean Target`
  - 复制当前闭包资源，并删除目标目录中不属于当前闭包的资源

当 `Dry Run` 关闭时，插件会在执行前显示确认弹窗。

## 部署到自己的引擎或团队环境

### 面向单个项目

使用项目插件部署方式：

```text
<Project>/Plugins/AssetClosureCollector/
```

这种方式更适合按项目管理，也便于让插件版本与项目版本保持一致。

### 面向同一引擎下的多个项目

使用引擎插件部署方式：

```text
<UE_ROOT>/Engine/Plugins/AssetClosureCollector/
```

适用于：

- 多个项目共用同一个引擎版本
- 团队希望集中安装统一的编辑器工具

复制插件后：

- 源码引擎用户应重新构建
- 安装版引擎用户可能会在项目启动时收到插件重建提示

## 源码结构

```text
AssetClosureCollector/
  AssetClosureCollector.uplugin
  Source/
    AssetClosureCollectorEditor/
      AssetClosureCollectorEditor.Build.cs
      Public/
      Private/
```

## 注意事项与限制

- 当前插件主要面向编辑器工作流，不面向运行时使用。
- 清理操作只影响 **目标根目录**，不会修改源资源所在位置。
- 目标清理数量只统计目标根路径下 **不属于当前闭包** 的资源。
- 空文件夹不计入清理资源。
- 实际行为依赖 Unreal 的 Asset Registry 与编辑器资产 API。
- 对大规模清理操作，建议先使用 `Dry Run` 进行预演。

## 推荐的安全使用流程

1. 开启 `Dry Run`
2. 分析依赖闭包
3. 审查复制计划与清理计划
4. 关闭 `Dry Run`
5. 确认后正式执行

## 仓库内容说明

本仓库用于存放插件源码。

请不要提交以下生成目录：

- `Binaries/`
- `Intermediate/`
- `DerivedDataCache/`
- `Saved/`

## 许可证

详见仓库中的 `LICENSE` 文件。
