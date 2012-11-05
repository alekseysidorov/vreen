set(QT_USE_QTDECLARATIVE true)

add_custom_directory(qml)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qml
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}/qml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/qml
)
add_custom_target(quickvreen-qml
    ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qml)

add_simple_executable(quickvreen
    INTERNAL
    CXX11
    INCLUDES ${QT_QTDECLARATIVE_INCLUDE_DIR} ${PROJECT_BINARY_DIR}/include
    LIBRARIES ${QT_QTDECLARATIVE_LIBRARY} vreen vreenoauth
)
