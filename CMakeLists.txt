cmake_minimum_required(VERSION 3.14)

project(FinalProject LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ErrorLog.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(FinalProject
  main.cpp
  WorkingWithJson.h
  startsearch.h
  startsearch.cpp
)


target_link_libraries(FinalProject PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(FinalProject PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)
target_link_libraries(FinalProject PRIVATE nlohmann_json::nlohmann_json)
