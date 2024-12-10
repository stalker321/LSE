cmake_minimum_required(VERSION 3.14)

project(FinalProject LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)

set(JSON_BuildTests OFF CACHE INTERNAL "")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ErrorLog.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/requests.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/programInfo.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/stopWord.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(FinalProject
  main.cpp
# Worcking with JSON
  WorkingWithJson/history.cpp
  WorkingWithJson/sistemJson.cpp
  WorkingWithJson/requestJson.cpp
  WorkingWithJson/blacklistOfWords.cpp
  WorkingWithJson/WorkingWithJson.h
# Search Engine
  searchEngine/DocumentBase.cpp
  searchEngine/SearchServer.cpp
  searchEngine/MainSearchEngine.cpp
  searchEngine/SearchEngine.h
# Network requests
  networkrequests/networkRequests.h
  networkrequests/networkRequests.cpp
# Error message
  errormessage/errormessage.cpp
  errormessage/errormessage.h
# Program message
  programmessage/programmessage.cpp
  programmessage/programmessage.h
# Interaction
  interaction/interaction.h
  interaction/interaction.cpp
# Word indexing
  wordIndexing/wordIndexing.h
  wordIndexing/wordIndexing.cpp
)

target_include_directories(FinalProject PRIVATE WorkingWithJson)
target_include_directories(FinalProject PRIVATE networkrequests)
target_include_directories(FinalProject PRIVATE programmessage)
target_include_directories(FinalProject PRIVATE searchEngine)
target_include_directories(FinalProject PRIVATE errormessage)
target_include_directories(FinalProject PRIVATE wordIndexing)
target_include_directories(FinalProject PRIVATE interaction)

target_link_libraries(FinalProject PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(FinalProject PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)
target_link_libraries(FinalProject PRIVATE Qt${QT_VERSION_MAJOR}::Network)
