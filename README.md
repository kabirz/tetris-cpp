# 俄罗斯方块 (Tetris) - Qt6实现

一个使用Qt6和C++20开发的经典俄罗斯方块游戏。

## 项目特性

- 🎮 完整的俄罗斯方块游戏逻辑
- 🎨 现代化的Qt6图形界面
- ⌨️ 双模式键盘控制（标准方向键 + Vim风格）
- 👻 方块阴影预览（显示落点位置）
- 🔄 智能墙踢（Wall Kick）功能，靠边旋转自动调整位置
- 📊 实时分数、等级和行数显示
- 🔄 方块预览功能
- ⚡ 随等级提升自动加速
- 🎯 碰撞检测和行消除
- 🏆 游戏结束检测

## 技术栈

- **编程语言**: C++20
- **GUI框架**: Qt 6.10.1
- **构建系统**: CMake 3.20+
- **编译器**: MSVC 2022 (支持C++20)
- **包管理**: vcpkg (可选)

## 项目结构

```
.
├── CMakeLists.txt              # CMake构建配置
├── CMakePresets.json          # CMake预设配置
├── README.md                  # 项目说明文档
├── resources/
│   ├── tetris.ico            # Windows应用程序图标
│   ├── tetris.rc             # Windows资源文件
│   ├── tetris_icon.svg       # SVG图标源文件
│   ├── icon_gen.py           # 图标生成脚本
│   └── resources.qrc         # Qt资源文件
└── src/
    ├── main.cpp              # 程序入口
    ├── mainwindow.h          # 主窗口头文件
    ├── mainwindow.cpp        # 主窗口实现
    ├── tetrisgame.h         # 游戏逻辑头文件
    ├── tetrisgame.cpp       # 游戏逻辑实现
    ├── tetrisboard.h        # 游戏画布头文件
    └── tetrisboard.cpp      # 游戏画布实现
```

## 环境要求

### 必需软件

- **Qt 6.10.1**: 安装在 `C:\Qt\6.10.1\msvc2022_64`
- **CMake**: 版本 3.20 或更高
- **Visual Studio 2022**: 支持C++20的编译器
- **Ninja**: 构建工具（CMake会自动下载）

### 可选软件

- **vcpkg**: C++包管理器（已安装）
- **Python 3 + Pillow**: 用于生成应用程序图标（如果需要重新生成图标）

## 编译步骤

### 使用CMake预设（推荐）

1. **配置项目**
```bash
cmake --preset release
```

2. **编译项目**
```bash
cmake --build --preset release
```

3. **运行程序**
```bash
.\build\bin\Release\Tetris.exe
```

**注意**: windeployqt会在编译后自动部署Qt依赖库，无需手动运行。

## 图标生成

如果需要重新生成应用程序图标，请运行以下命令：

```bash
powershell -Command "cd resources; python icon_gen.py"
```

这将生成包含多种尺寸（16x16, 32x32, 48x48, 64x64, 128x128, 256x256）的tetris.ico文件。

**注意**: 图标文件已包含在项目中，通常不需要重新生成。

## 部署说明

### 使用windeployqt自动部署

项目使用windeployqt工具自动部署Qt依赖库。部署后的可执行文件位于：
```
build/bin/Release/Tetris.exe
```

部署会自动复制以下文件到可执行文件目录：
- Qt6核心库（Qt6Core.dll, Qt6Gui.dll, Qt6Widgets.dll等）
- Qt插件（platforms, imageformats, styles等）
- 翻译文件
- 其他运行时依赖

### 手动部署

如果需要手动部署，请确保以下文件在可执行文件目录中：

**必需的DLL文件：**
- Qt6Core.dll
- Qt6Gui.dll
- Qt6Widgets.dll
- Qt6Network.dll
- Qt6Svg.dll

**必需的插件目录：**
- platforms/qwindows.dll
- imageformats/qjpeg.dll, qico.dll, qgif.dll, qsvg.dll
- styles/qmodernwindowsstyle.dll
- iconengines/qsvgicon.dll
- generic/qtuiotouchplugin.dll
- networkinformation/qnetworklistmanager.dll
- tls/qcertonlybackend.dll, qschannelbackend.dll

**其他依赖：**
- icuuc.dll
- opengl32sw.dll
- D3Dcompiler_47.dll
- translations/（翻译文件目录）

## 游戏操作

### 键盘控制

| 按键 | 功能 |
|-------|------|
| ← | 向左移动 |
| → | 向右移动 |
| ↓ | 加速下落 |
| ↑ | 旋转方块 |
| 空格 | 直接下落 |

### 菜单快捷键

| 快捷键 | 功能 |
|---------|------|
| Ctrl+S | 开始游戏 |
| Ctrl+P | 暂停/继续 |
| Ctrl+R | 重置游戏 |
| Ctrl+Q | 退出游戏 |

## 游戏规则

1. **目标**: 通过移动和旋转方块，填满整行来消除行
2. **得分**: 消除的行数越多，得分越高
   - 消除1行: 100 × 等级
   - 消除2行: 300 × 等级
   - 消除3行: 500 × 等级
   - 消除4行: 800 × 等级
3. **升级**: 每消除10行升一级，下落速度加快
4. **游戏结束**: 当新方块无法放置时游戏结束

## 方块类型

游戏包含7种经典方块：

| 方块 | 颜色 | 形状 |
|-------|-------|------|
| I | 青色 | ████ |
| O | 黄色 | ██<br>██ |
| T | 紫色 | ███<br> █ |
| S | 绿色 |  ██<br>██ |
| Z | 红色 | ██<br>  ██ |
| J | 蓝色 | █<br>███ |
| L | 橙色 |   █<br>███ |

## CMake预设说明

项目提供了以下CMake预设：

### 配置预设

- **default**: 默认配置，使用Visual Studio 18 2026生成器
- **debug**: Debug构建模式（继承default）
- **release**: Release构建模式（继承default）

### 构建预设

- **debug**: 使用debug配置构建
- **release**: 使用release配置构建

## 故障排除

### 编译错误

**问题**: 找不到Qt6头文件
```
解决方案: 确保Qt6安装在 C:\Qt\6.10.1\msvc2022_64
```

**问题**: CMake找不到Qt6
```
解决方案: 检查CMAKE_PREFIX_PATH设置是否正确
```

### 运行时错误

**问题**: 缺少Qt6 DLL
```
解决方案: 运行windeployqt自动部署，或手动复制所需的DLL文件
"C:/Qt/6.10.1/msvc2022_64/bin/windeployqt.exe" build/bin/Release/Tetris.exe
```

**问题**: 程序无法启动
```
解决方案: 确保使用正确的编译器（MSVC 2022）并已部署所有依赖
```

**问题**: 找不到平台插件
```
解决方案: 确保platforms目录和qwindows.dll在可执行文件目录中
```

## 开发说明

### 添加新功能

1. 在相应的头文件中声明新方法
2. 在对应的cpp文件中实现方法
3. 在主窗口中连接信号和槽
4. 重新编译项目

### 代码风格

- 使用C++20特性
- 遵循Qt编码规范
- 使用智能指针管理资源
- 使用RAII原则

## 许可证

本项目仅供学习和参考使用。

## 贡献

欢迎提交问题和改进建议！

## 作者

使用Qt6和C++20开发的俄罗斯方块游戏

## 更新日志

### v1.0.0 (2024)
- 初始版本发布
- 实现基本游戏功能
- 添加图形界面
- 支持键盘控制
- 实现分数系统

### v1.0.1 (2025)
- 集成windeployqt自动部署Qt依赖
- 添加Windows应用程序图标支持
- 优化构建配置

## 致谢

- Qt框架开发团队
- C++标准委员会
- 开源社区