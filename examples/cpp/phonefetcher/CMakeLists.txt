add_simple_executable(phonefetcher
    INTERNAL
    CXX11
    INCLUDES ${VREEN_INCLUDE_DIR}
    LIBRARIES vreen vreenoauth
    QT Core Network Gui Widgets
)
