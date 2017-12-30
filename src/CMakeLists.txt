cmake_minimum_required(VERSION 2.8.11)
project(core)
add_definitions(-DCORE_LIBRARY)
file(GLOB_RECURSE DATABASE_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(QT_MIN_VERSION "5.6.0")

if(DEFINED QTDIR${_lib_suffix})
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR${_lib_suffix}}")
elseif(DEFINED QTDIR)
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
elseif(DEFINED ENV{QTDIR${_lib_suffix}})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR${_lib_suffix}}")
elseif(DEFINED ENV{QTDIR})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Qml)
include_directories(${Qt5Qml_PRIVATE_INCLUDE_DIRS})

add_library(core SHARED ${DATABASE_SRCS} "functor.h")
set_property(TARGET core PROPERTY CXX_STANDARD 14)
target_include_directories (core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core Qt5::Core Qt5::Qml)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(core Pdh)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(CORE_INSTALL_PATH "plugins/QFlow/Core" CACHE PATH "qFlow Core Library Install Path")
install (TARGETS core DESTINATION ${CORE_INSTALL_PATH} COMPONENT qflow)
install (FILES qmldir DESTINATION ${CORE_INSTALL_PATH} COMPONENT qflow)
