cmake_minimum_required(VERSION 3.10)
project(computerMechanics)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:\\Qt\\5.15.2\\mingw81_64\\lib\\cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(computerMechanics  WIN32
        main.cpp
        Element.h
        Element.cpp
        elementItem.h
        elementItem.cpp
        input.h
        input.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
        Material.h
        Material.cpp
        method.h
        method.cpp
        node.h
        node.cpp
        nodeeditdialog.h
        nodeeditdialog.cpp
        nodeeditdialog.ui
        nodeItem.h
        nodeItem.cpp
        TotalNode.h
        TotalNode.cpp
        Message.cpp 
        Message.h
        elementeditdialog.h
        elementeditdialog.cpp
        elementeditdialog.ui
        icons.qrc
        )
target_link_libraries(computerMechanics
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

