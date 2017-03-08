prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

Name: wpe
Description: The wpe library
Version: @WPE_VERSION@
Cflags: -I${includedir}
Libs: -L${libdir} -lWPEBackend
