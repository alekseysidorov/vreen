find_package(Qt4 COMPONENTS QtTest)

include_directories(../src/api
    ../src/oauth
    ../src/qml/src
    .
)

add_subdirectory(auto) 
