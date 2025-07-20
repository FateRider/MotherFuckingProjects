cmake_minimum_required(VERSION 3.16)

project(Qtlayout)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(Qtlayout
    ./src/main.cpp
    ./src/MainWindow.cpp
    ./src/MainWindow.hpp
)


target_link_libraries(Qtlayout
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
