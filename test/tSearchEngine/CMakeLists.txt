cmake_minimum_required(VERSION 3.16)

project(searchEngine LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test2)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test2/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test2)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test2/ ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test2/test3.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(searchEngine
          tst_searchengine.cpp
          searchengine.h
          DocumentBase.cpp
          MainSearchEngine.cpp
          SearchServer.cpp
          WorkingWithJson.h
          wordIndexing.h
          blacklistOfWords.cpp
          history.cpp
          requestJson.cpp
          sistemJson.cpp
          errormessage.h
          errormessage.cpp
          SearchEngine.h
          wordIndexing.cpp
)
add_test(NAME searchEngine COMMAND searchEngine)

target_link_libraries(searchEngine PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(searchEngine PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)
