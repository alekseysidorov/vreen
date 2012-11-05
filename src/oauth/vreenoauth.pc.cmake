prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${CMAKE_INSTALL_PREFIX}/bin
libdir=${LIB_DESTINATION}
includedir=${CMAKE_INSTALL_PREFIX}/include

Name: vreenoauth
Description: OAuth2 connection library for vreen
Requires: QtCore QtWebKit vreen
Version: ${CMAKE_VREEN_VERSION_MAJOR}.${CMAKE_VREEN_VERSION_MINOR}.${CMAKE_VREEN_VERSION_PATCH}
Libs: -L${LIB_DESTINATION} -lvreenoauth
Cflags: -I${CMAKE_INSTALL_PREFIX}/include/vreen

