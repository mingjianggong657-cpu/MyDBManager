cmake_minimum_required(VERSION 3.16)

project(QtModelViewDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(QtModelViewDemo
    main.cpp
    MainWindow.cpp
    MainWindow.h
    TableModel.cpp
    TableModel.h
    Worker.cpp
    Worker.h
)

target_link_libraries(QtModelViewDemo
    PRIVATE
    Qt6::Widgets
    Qt6::Sql
)
