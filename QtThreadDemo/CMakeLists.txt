cmake_minimum_required(VERSION 3.16)

project(QtThreadDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(QtThreadDemo
    main.cpp
    MainWindow.cpp
    Worker.cpp
    MainWindow.h
    Worker.h
)

target_link_libraries(QtThreadDemo
    PRIVATE
    Qt6::Widgets
    Qt6::Sql
)
