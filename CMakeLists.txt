cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(wpebackend VERSION 0.1)

set(WPE_API_VERSION "0.1")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -D_POSIX_SOURCE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(DistTargets)
include(GNUInstallDirs)

find_package(EGL REQUIRED)

add_definitions(${EGL_DEFINITIONS})
add_definitions(-DWPE_COMPILATION)

if(WPE_BACKEND)
  add_definitions(-DWPE_BACKEND=\"${WPE_BACKEND}\")
endif()

set(WPE_INCLUDE_DIRECTORIES
    "include"
    "src"
    ${EGL_INCLUDE_DIRS}
)

set(WPE_LIBRARIES
    dl
)

set(WPE_SOURCES
    src/input.c
    src/loader.c
    src/pasteboard.c
    src/pasteboard-generic.cpp
    src/pasteboard-noop.cpp
    src/renderer-backend-egl.c
    src/renderer-host.c
    src/view-backend.c
)

set(WPE_PUBLIC_HEADERS
  include/wpe/input.h
  include/wpe/loader.h
  include/wpe/pasteboard.h
  include/wpe/renderer-backend-egl.h
  include/wpe/renderer-host.h
  include/wpe/view-backend.h
  include/wpe/wpe-egl.h
  include/wpe/wpe.h
)

add_library(WPEBackend SHARED ${WPE_SOURCES})
target_include_directories(WPEBackend PRIVATE ${WPE_INCLUDE_DIRECTORIES})
target_link_libraries(WPEBackend ${WPE_LIBRARIES})

set_target_properties(WPEBackend PROPERTIES
  OUTPUT_NAME WPEBackend-${WPE_API_VERSION}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS WPEBackend
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
  FILES
    ${WPE_PUBLIC_HEADERS}
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/wpe-${WPE_API_VERSION}/wpe
)

configure_file(wpe.pc.cmake wpe-${WPE_API_VERSION}.pc @ONLY)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/wpe-${WPE_API_VERSION}.pc"
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
