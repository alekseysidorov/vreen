prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${CMAKE_INSTALL_PREFIX}/bin
libdir=${LIB_DESTINATION}
includedir=${CMAKE_INSTALL_PREFIX}/include

Name: vkit
Description: Simple and fast Qt Binding for vk.com API
Requires: QtCore QtNetwork
Version: ${CMAKE_LIBVK_VERSION_MAJOR}.${CMAKE_LIBVK_VERSION_MINOR}.${CMAKE_LIBVK_VERSION_PATCH}
Libs: -L${LIB_DESTINATION} -lvkit
Cflags: -I${CMAKE_INSTALL_PREFIX}/include/

