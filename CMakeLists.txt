cmake_minimum_required(VERSION 3.20)
project(voip_client3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)


add_executable(voip_client3 main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        button.h
        button.cpp
        mainwindow.cpp
        mainwindow.h
        button.cpp
        button.h
        StringList.h
        database_handler.h
        add_contact.cpp
        add_contact.h Login.cpp Login.h)

target_link_libraries(voip_client3
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        sqlite3)

