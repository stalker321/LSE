cmake_minimum_required(VERSION 3.5)

project(WorkingWithJson LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/requests.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/stopWord.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WorkingWithJson
          tst_workingwithjson.cpp
          WorkingWithJson.h
          history.cpp
          requestJson.cpp
          sistemJson.cpp
          errormessage.cpp
          errormessage.h
          blacklistOfWords.cpp
)
add_test(NAME WorkingWithJson COMMAND WorkingWithJson)

target_link_libraries(WorkingWithJson PRIVATE Qt${QT_VERSION_MAJOR}::Test)

