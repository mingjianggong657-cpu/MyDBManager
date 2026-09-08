cmake_minimum_required(VERSION 3.16)

project(MyDBManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(MyDBManager
    src/main.cpp
    src/MainWindow.cpp
    src/DatabaseManager.cpp

    include/MainWindow.h
    include/DatabaseManager.h
)

target_include_directories(MyDBManager
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(MyDBManager
    PRIVATE
    Qt6::Widgets
    Qt6::Sql
)
