prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

Name: wpe-@WPE_API_VERSION@
Description: The wpe library
Version: @PROJECT_VERSION@
Requires: xkbcommon
Cflags: -I${includedir}/wpe-@WPE_API_VERSION@
Libs: -L${libdir} -lWPEBackend-@WPE_API_VERSION@
