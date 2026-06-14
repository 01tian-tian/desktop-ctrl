# desktop-ctrl

`desktop-ctrl` 是一个基于 C++ 开发的 Windows 桌面控制工具，旨在通过命令行提供对桌面环境的多种交互能力。它允许用户执行屏幕截图、获取屏幕信息、模拟鼠标和键盘操作，以及管理剪贴板内容等。

## 功能特性

- **屏幕操作**：
  - 截取屏幕截图并保存为指定格式（例如 PNG）。
  - 获取当前屏幕的逻辑宽度、高度和缩放因子。

- **鼠标控制**：
  - 模拟鼠标单击（左键、双击、右键、中键）。
  - 模拟鼠标拖拽操作。
  - 精确移动鼠标光标到指定坐标。
  - 模拟鼠标滚轮滚动。

- **键盘控制**：
  - 模拟文本输入，支持从命令行参数或标准输入读取文本。
  - 发送热键组合。
  - 模拟按键按下和释放。

- **剪贴板管理**：
  - 获取当前剪贴板中的文本内容。
  - 设置剪贴板的文本内容。

- **通用功能**：
  - 提供版本和平台信息。
  - 支持自定义等待时间。

## 构建与运行

本项目使用 CMake 进行构建。请确保您的系统已安装 CMake 和 C++ 编译器（如 MinGW-w64 或 Visual Studio）。

### 构建步骤

1. 克隆仓库到本地：
   ```bash
   git clone [仓库地址]
   cd desktop-ctrl
   ```

2. 创建构建目录并生成项目文件：
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. 编译项目：
   ```bash
   cmake --build .
   ```

编译成功后，您将在 `build` 目录下找到可执行文件 `desktop-ctrl.exe`。

## 使用示例

以下是一些 `desktop-ctrl` 命令的使用示例：

### 获取屏幕信息

```bash
./desktop-ctrl info
```

### 截取屏幕截图

```bash
./desktop-ctrl screenshot --format png
```

### 模拟鼠标点击

在屏幕中心进行左键单击：
```bash
./desktop-ctrl click --x 0.5 --y 0.5
```

### 模拟键盘输入

输入文本：
```bash
./desktop-ctrl type --text "Hello, World!"
```

### 设置剪贴板内容

```bash
./desktop-ctrl clipboard --set "这是要复制到剪贴板的文本"
```

### 获取剪贴板内容

```bash
./desktop-ctrl clipboard --get
```

## 许可证

本项目采用 [MIT 许可证](LICENSE) 开源。
