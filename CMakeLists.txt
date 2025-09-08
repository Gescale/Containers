cmake_minimum_required(VERSION 3.16)
project(Containers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Xml)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/logo.png")

# Client Application
qt_add_executable(Containers
    main.cpp
    mainclient.h
    mainclient.cpp
    containerwidget.h
    containerwidget.cpp
    container.h
    container.cpp
    box.h
    box.cpp
    cylinder.h
    cylinder.cpp
    pallet.h
    pallet.cpp
    containerfactory.h
    containerfactory.cpp
    memento.h
    memento.cpp
    xmlworker.h
    xmlworker.cpp
    resources.qrc
)

target_link_libraries(Containers PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Xml
)
