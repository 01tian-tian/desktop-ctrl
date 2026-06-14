cmake_minimum_required(VERSION 3.15)
project(desktop-ctrl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(desktop-ctrl
    src/main.cpp
    src/types.cpp
    src/json.cpp
    src/screen.cpp
    src/screenshot.cpp
    src/input.cpp
    src/clipboard.cpp
)

target_include_directories(desktop-ctrl PRIVATE include src)

target_link_libraries(desktop-ctrl
    user32
    gdi32
    gdiplus
    shcore
    kernel32
)

add_definitions(-DUNICODE -D_UNICODE)
