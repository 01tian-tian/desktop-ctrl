# Windows-Desktop-Ctrl-SKILL

`Windows-Desktop-Ctrl-SKILL` 是一个专为 Windows 桌面设计的自动化控制工具，它通过命令行接口提供了一系列强大的功能，旨在简化和自动化图形用户界面（GUI）任务。无论是进行屏幕截图、模拟鼠标键盘操作，还是管理剪贴板内容，`Windows-Desktop-Ctrl-SKILL` 都能够提供高效且可靠的解决方案。

## 核心功能

该工具的核心功能围绕着自动化桌面交互，主要包括：

- **屏幕操作**：
  - **屏幕截图**：能够捕获当前屏幕的图像，并支持保存为多种格式（例如 PNG），同时包含鼠标光标，便于后续分析。
  - **屏幕信息获取**：提供当前屏幕的逻辑宽度、高度以及缩放因子等详细信息。

- **鼠标控制**：
  - **精确点击**：支持模拟鼠标左键单击、双击、右键和中键点击，可指定屏幕上的归一化坐标。
  - **鼠标拖拽**：实现从起始点到结束点的鼠标拖拽操作，适用于文件移动、窗口调整等场景。
  - **光标移动**：将鼠标光标精确移动到屏幕上的任意指定位置。
  - **滚轮滚动**：模拟鼠标滚轮的上下左右滚动，支持自定义滚动方向。

- **键盘控制**：
  - **文本输入**：支持通过命令行参数或标准输入进行文本输入，尤其适用于自动化表单填写或消息发送。
  - **热键组合**：能够发送复杂的热键组合（例如 `Ctrl+Shift+N`），以触发系统或应用程序的特定功能。
  - **按键模拟**：提供独立的按键按下和释放功能，允许更精细的键盘操作控制。

- **剪贴板管理**：
  - **获取内容**：读取当前系统剪贴板中的文本内容。
  - **设置内容**：将指定的文本内容写入系统剪贴板。

- **辅助功能**：
  - **延迟等待**：支持自定义毫秒级的等待时间，确保操作间的时序正确性。
  - **版本信息**：提供工具的版本和运行平台信息。

## Agent 工作流

`Windows-Desktop-Ctrl-SKILL` 的设计理念是支持 Agent 进行桌面自动化任务，其典型工作流遵循以下循环模式：

```
截图 → 分析 → 执行 → 截图验证 → 循环
```

1.  **截图**：获取当前屏幕的实时状态。
2.  **分析**：对截图进行图像识别和分析，以识别用户界面元素及其精确坐标。
3.  **执行**：根据分析结果，调用 `Windows-Desktop-Ctrl-SKILL` 命令执行相应的桌面操作。
4.  **验证**：再次进行屏幕截图，以确认上一步操作是否成功并达到预期效果。
5.  **循环**：重复上述步骤，直至整个自动化任务完成。

**重要提示**：在执行任何桌面操作之前，务必通过截图来确认目标元素的位置，避免使用硬编码的坐标，以适应不同分辨率和布局的桌面环境。

## 坐标系

`Windows-Desktop-Ctrl-SKILL` 采用归一化坐标系 `[0, 1]`，这使得它能够与视觉语言模型（VLM）的输出无缝对接。在该坐标系中：

- `(0,0)` 代表屏幕的左上角。
- `(1,1)` 代表屏幕的右下角。

## 快速开始

以下是使用 `Windows-Desktop-Ctrl-SKILL` 进行桌面自动化的一些基本示例：

### 1. 截图查看屏幕

```bash
./Windows-Desktop-Ctrl-SKILL screenshot
# 命令执行后，截图将保存到 Temp/screenshot.png。您可以通过分析此图片来识别屏幕内容。
# 输出示例: {"ok":true,"path":"...","width":1920,"height":1200}
```

### 2. 点击目标

假设通过截图分析得到目标按钮位于屏幕的 `(0.5, 0.5)` 处（屏幕中心）：

```bash
./Windows-Desktop-Ctrl-SKILL click --x 0.5 --y 0.5
```

### 3. 输入文本

在已激活的文本输入框中输入内容：

```bash
./Windows-Desktop-Ctrl-SKILL type --text "Hello, Manus AI!"
```

### 4. 验证结果

操作完成后，再次截图以验证操作是否成功：

```bash
./Windows-Desktop-Ctrl-SKILL screenshot
# 分析新的截图，确认操作结果。
```

## 命令参考

| 命令类型   | 命令示例                                                               | 描述                                                                 |
| :--------- | :--------------------------------------------------------------------- | :------------------------------------------------------------------- |
| **截图**   | `Windows-Desktop-Ctrl-SKILL screenshot`                                              | 截取屏幕并保存为 `Temp/screenshot.png`。                             |
| **鼠标**   | `Windows-Desktop-Ctrl-SKILL click --x 0.5 --y 0.5`                                   | 在指定归一化坐标处进行左键单击。                                     |
|            | `Windows-Desktop-Ctrl-SKILL double-click --x 0.5 --y 0.5`                            | 在指定归一化坐标处进行双击。                                         |
|            | `Windows-Desktop-Ctrl-SKILL right-click --x 0.5 --y 0.5`                             | 在指定归一化坐标处进行右键单击。                                     |
|            | `Windows-Desktop-Ctrl-SKILL mouse-move --x 0.5 --y 0.5`                              | 将鼠标移动到指定归一化坐标。                                         |
|            | `Windows-Desktop-Ctrl-SKILL drag --sx 0.1 --sy 0.1 --ex 0.9 --ey 0.9`                | 从 `(sx, sy)` 拖拽到 `(ex, ey)`。                                     |
| **键盘**   | `Windows-Desktop-Ctrl-SKILL type --text "Hello World"`                               | 输入指定文本。                                                       |
|            | `Windows-Desktop-Ctrl-SKILL hotkey --keys "ctrl+shift+n"`                            | 模拟热键组合。                                                       |
|            | `Windows-Desktop-Ctrl-SKILL press --key shift`                                       | 按下指定按键（不释放）。                                             |
|            | `Windows-Desktop-Ctrl-SKILL release --key shift`                                     | 释放指定按键。                                                       |
| **滚动**   | `Windows-Desktop-Ctrl-SKILL scroll --x 0.5 --y 0.5 --direction down`                 | 在指定坐标处向下滚动。`direction` 可选 `up/down/left/right`。       |
| **剪贴板** | `Windows-Desktop-Ctrl-SKILL clipboard --get`                                         | 获取剪贴板内容。                                                     |
|            | `Windows-Desktop-Ctrl-SKILL clipboard --set "要复制的内容"`                          | 设置剪贴板内容。                                                     |
| **其他**   | `Windows-Desktop-Ctrl-SKILL wait --ms 1000`                                          | 等待 1000 毫秒。                                                     |
|            | `Windows-Desktop-Ctrl-SKILL screen-size`                                             | 获取屏幕尺寸信息。                                                   |
|            | `Windows-Desktop-Ctrl-SKILL info`                                                    | 获取工具版本和平台信息。                                             |

## 注意事项

- 截图默认包含鼠标光标，有助于分析鼠标位置。
- 中文输入通过剪贴板方式实现，确保输入准确性。
- 所有命令的输出均为 JSON 格式，便于程序化解析。
- 每次执行操作前，建议通过截图确认目标位置，以提高自动化任务的鲁棒性。

## 许可证

本项目采用 [MIT 许可证](LICENSE) 开源。
