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

#TODO check if vars is defined
set(RLIBDIR bin)
set(LIBDIR lib${LIB_SUFFIX})

set(CMAKE_LIBVK_VERSION_MAJOR 0 CACHE INT "Major vk version number" FORCE)
set(CMAKE_LIBVK_VERSION_MINOR 8 CACHE INT "Minor vk version number" FORCE)
set(CMAKE_LIBVK_VERSION_PATCH 0 CACHE INT "Release vk version number" FORCE)
set(CMAKE_LIBVK_VERSION_STRING "${CMAKE_LIBVK_VERSION_MAJOR}.${CMAKE_LIBVK_VERSION_MINOR}.${CMAKE_LIBVK_VERSION_PATCH}" CACHE STRING "vkit version string" FORCE)

#add pkgconfig file
set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIBDIR})
configure_file(vkit.pc.cmake
	${CMAKE_CURRENT_BINARY_DIR}/vkit.pc
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vkit.pc
	DESTINATION ${LIB_DESTINATION}/pkgconfig
)
add_subdirectory(src)
