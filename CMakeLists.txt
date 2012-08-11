project(vreen)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if(COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
endif(COMMAND cmake_policy)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(QT_USE_QTCORE true)
set(QT_USE_QTNETWORK true)
#set(CMAKE_AUTOMOC true)

find_package(Qt4 REQUIRED)

include(UseQt4)
include(CommonUtils)

option(VREEN_WITH_OAUTH "Enable webkit based oauth authorization" ON)
option(VREEN_WITH_QMLAPI "Enable QtQuick bindings for vreen" ON)
option(VREEN_INSTALL_HEADERS "Install devel headers for vreen" ON)

#TODO check if vars is defined
set(RLIBDIR bin)
set(LIBDIR lib${LIB_SUFFIX})
set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIBDIR})

set(CMAKE_VREEN_VERSION_MAJOR 0 CACHE INT "Major vk version number" FORCE)
set(CMAKE_VREEN_VERSION_MINOR 9 CACHE INT "Minor vk version number" FORCE)
set(CMAKE_VREEN_VERSION_PATCH 1 CACHE INT "Release vk version number" FORCE)
set(CMAKE_VREEN_VERSION_STRING "${CMAKE_VREEN_VERSION_MAJOR}.${CMAKE_VREEN_VERSION_MINOR}.${CMAKE_VREEN_VERSION_PATCH}" CACHE STRING "vreen version string" FORCE)

add_subdirectory(src)

get_target_property(UNINSTALL_LOCATION uninstall LOCATION)
if(NOT UNINSTALL_LOCATION)
    configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)
    add_custom_target(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
