# 俄罗斯方块 (Tetris) - Qt6实现

一个使用Qt6和C++20开发的经典俄罗斯方块游戏。

## 项目特性

- 🎮 完整的俄罗斯方块游戏逻辑
- 🎨 现代化的Qt6图形界面
- ⌨️ 双模式键盘控制（标准方向键 + Vim风格）
- 👻 方块阴影预览（显示落点位置）
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

## 编译步骤

### 方法1: 使用CMake预设（推荐）

1. **配置项目**
```bash
cmake --preset default
```

2. **编译项目**
```bash
cmake --build build --config Release
```

3. **部署Qt依赖**
```bash
"C:/Qt/6.10.1/msvc2022_64/bin/windeployqt.exe" build/bin/Release/Tetris.exe
```

4. **运行程序**
```bash
.\build\bin\Release\Tetris.exe
```

### 方法2: 手动配置

1. **创建构建目录**
```bash
mkdir build
cd build
```

2. **配置CMake**
```bash
cmake -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/msvc2022_64" -DCMAKE_CXX_STANDARD=20 ..
```

3. **编译**
```bash
cmake --build .
```

4. **运行**
```bash
.\bin\Tetris.exe
```

### 方法3: 使用Visual Studio

1. **生成Visual Studio解决方案**
```bash
cmake -G "Visual Studio 18 2026" -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/msvc2022_64" ..
```

2. **打开解决方案**
```
打开生成的 Tetris.sln 文件
```

3. **编译和运行**
```
在Visual Studio中按F5编译并运行
```

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

- **default**: 默认配置，使用Ninja生成器
- **debug**: Debug构建模式
- **release**: Release构建模式

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

## 致谢

- Qt框架开发团队
- C++标准委员会
- 开源社区