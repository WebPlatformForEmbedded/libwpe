prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib
backendsdir=${libdir}/wpe-@WPE_API_VERSION@

Name: wpe-@WPE_API_VERSION@
Description: The wpe library
Version: @PROJECT_VERSION@
Requires: xkbcommon
Cflags: -I${includedir}/wpe-@WPE_API_VERSION@
Libs: -L${libdir} -lwpe-@WPE_API_VERSION@
