cmake_minimum_required(VERSION 3.5)

project(ClassDiagramTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Qml Quick QuickControls2 Widgets)

add_executable(ClassDiagramTool
    src/main.cpp
    src/diagrammanager.cpp src/diagrammanager.h
    src/HeaderParser.cpp src/HeaderParser.h
    src/resources.qrc
)

target_link_libraries(ClassDiagramTool PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Widgets
)