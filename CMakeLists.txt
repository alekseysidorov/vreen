
project(vreen)

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CommonUtils)

option(VREEN_WITH_AUTH_WIDGET "Enable widget based oauth authorization" ON)
option(VREEN_WITH_DIRECTAUTH "Enable direct authorization" OFF)
option(VREEN_WITH_QMLAPI "Enable QtQuick bindings for vreen" ON)
option(VREEN_WITH_EXAMPLES "Enable vreen tests" ON)
option(VREEN_WITH_TESTS "Enable vreen examples" ON)
option(VREEN_INSTALL_HEADERS "Install devel headers for vreen" ON)
option(VREEN_DEVELOPER_BUILD "Install devel headers for vreen" OFF)

#TODO check if vars is defined
set(RLIBDIR bin)
set(LIBDIR lib${LIB_SUFFIX})
set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIBDIR})
if(APPLE)
    set(CMAKE_INSTALL_NAME_DIR "${LIB_DESTINATION}") #hack for mac
endif()

set(CMAKE_VREEN_VERSION_MAJOR 2 CACHE INT "Major vk version number" FORCE)
set(CMAKE_VREEN_VERSION_MINOR 0 CACHE INT "Minor vk version number" FORCE)
set(CMAKE_VREEN_VERSION_PATCH 0 CACHE INT "Release vk version number" FORCE)
set(CMAKE_VREEN_VERSION_STRING "${CMAKE_VREEN_VERSION_MAJOR}.${CMAKE_VREEN_VERSION_MINOR}.${CMAKE_VREEN_VERSION_PATCH}" CACHE STRING "vreen version string" FORCE)

set(QT_IMPORTS_DIR "/usr/lib/x86_64-linux-gnu/qt5/qml/" CACHE PATH "Qt qml imports directory")
set(VREEN_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include)
set(VREEN_PRIVATE_INCLUDE_DIR ${VREEN_INCLUDE_DIR}/vreen/${CMAKE_VREEN_VERSION_STRING})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_subdirectory(src)

# if(VREEN_WITH_EXAMPLES)
#     add_subdirectory(examples)
# endif()
# if(VREEN_WITH_TESTS)
#     enable_testing()
#     add_subdirectory(tests)
# endif()
