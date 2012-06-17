find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(K8JSON k8json)
endif()

if(NOT K8JSON_FOUND)
	message(STATUS "Using internal copy of k8json")
	set(K8JSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

	list(APPEND SRC "3rdparty/k8json/k8json.cpp")
	add_library(k8json STATIC ${SRC})
	target_link_libraries(k8json ${QT_QTCORE_LIBRARY})
	update_compiler_flags(k8json)
	set(K8JSON_LIBRARIES k8json)
endif()

add_subdirectory(api)