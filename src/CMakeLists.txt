cmake_minimum_required(VERSION 2.8.11)
set (CMAKE_PREFIX_PATH "/home/michal/Qt/5.6/gcc_64/lib/cmake")
project(core)
add_definitions(-DCORE_LIBRARY)
aux_source_directory(. SRC_LIST)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(QT_MIN_VERSION "5.6.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Qml)
include_directories(${Qt5Qml_PRIVATE_INCLUDE_DIRS})

add_library(core SHARED ${SRC_LIST})
set_property(TARGET core PROPERTY CXX_STANDARD 11)
target_include_directories (core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core Qt5::Core Qt5::Qml)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(core Pdh)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
