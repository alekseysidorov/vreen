macro(VREEN_ADD_DECLARATIVE_EXAMPLE name)
    add_custom_directory(qml)

    add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qml
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}/qml
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/qml
    )
    add_custom_target(${name}-qml
	ALL
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qml)

    if(USE_QT5)
	set(_declarative Quick)
    else()
	set(_declarative Declarative)
    endif()

    add_simple_executable(${name}
	INTERNAL
	CXX11
	INCLUDES ${QT_QTDECLARATIVE_INCLUDE_DIR}
	LIBRARIES ${QT_QTDECLARATIVE_LIBRARY}
	SOURCE_DIR ../common
	QT Core Gui Widgets ${_declarative}
    )
endmacro()

add_subdirectory(dialogs)
add_subdirectory(photoalbum)
add_subdirectory(audio)
