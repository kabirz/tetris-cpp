cmake_minimum_required(VERSION 3.20)
project(Tetris VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用Qt的自动MOC、UIC和RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找Qt6包
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Svg)

# 源文件
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/tetrisgame.cpp
    src/tetrisboard.cpp
)

# 头文件
set(HEADERS
    src/mainwindow.h
    src/tetrisgame.h
    src/tetrisboard.h
)

# 资源文件
set(RESOURCES
    resources/resources.qrc
)

# Windows资源文件（用于设置可执行文件图标）
if(WIN32)
    list(APPEND RESOURCES resources/tetris.rc)
endif()

# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# 链接Qt6库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Svg
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Windows特定设置
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # 查找windeployqt工具
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin" "${Qt6_DIR}/bin")
    
    if(WINDEPLOYQT_EXECUTABLE)
        # 添加windeployqt自定义目标
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --release
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Running windeployqt to deploy Qt dependencies..."
            WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
        message(STATUS "windeployqt found: ${WINDEPLOYQT_EXECUTABLE}")
    else()
        message(WARNING "windeployqt not found. Qt dependencies will not be automatically deployed.")
    endif()
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)