cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(VersioningUtils)

SET_PROJECT_VERSION(1 3 1)
set(WPE_API_VERSION "1.0")

# Before making a release, the LT_VERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A
CALCULATE_LIBRARY_VERSIONS_FROM_LIBTOOL_TRIPLE(LIBWPE 3 1 2)

project(libwpe VERSION "${PROJECT_VERSION}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -D_POSIX_SOURCE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(DERIVED_SOURCES_DIR "${CMAKE_BINARY_DIR}/DerivedSources/wpe")
configure_file(include/wpe/version.h.cmake ${DERIVED_SOURCES_DIR}/version.h @ONLY)
configure_file(include/wpe/version-deprecated.h.cmake ${DERIVED_SOURCES_DIR}/version-deprecated.h @ONLY)

include(DistTargets)
include(GNUInstallDirs)

find_package(EGL REQUIRED)
find_package(Libxkbcommon REQUIRED)

add_definitions(${EGL_DEFINITIONS})
add_definitions(-DWPE_COMPILATION)

if(WPE_BACKEND)
  add_definitions(-DWPE_BACKEND=\"${WPE_BACKEND}\")
endif()

set(WPE_INCLUDE_DIRECTORIES
    "include"
    "src"
    ${DERIVED_SOURCES_DIR}
    ${EGL_INCLUDE_DIRS}
)

set(WPE_LIBRARIES
    dl
    ${LIBXKBCOMMON_LIBRARIES}
)

set(WPE_SOURCES
    src/input.c
    src/input-method.c
    src/key-unicode.c
    src/loader.c
    src/pasteboard.c
    src/pasteboard-generic.cpp
    src/pasteboard-noop.cpp
    src/renderer-backend-egl.c
    src/renderer-host.c
    src/version.c
    src/view-backend.c
    src/view-ui.c
)

set(WPE_PUBLIC_HEADERS
  ${DERIVED_SOURCES_DIR}/version.h
  ${DERIVED_SOURCES_DIR}/version-deprecated.h
  include/wpe/export.h
  include/wpe/input.h
  include/wpe/input-method.h
  include/wpe/keysyms.h
  include/wpe/loader.h
  include/wpe/pasteboard.h
  include/wpe/renderer-backend-egl.h
  include/wpe/renderer-host.h
  include/wpe/view-backend.h
  include/wpe/view-ui.h
  include/wpe/wpe-egl.h
  include/wpe/wpe.h
)

add_library(wpe SHARED ${WPE_SOURCES})
target_include_directories(wpe PRIVATE ${WPE_INCLUDE_DIRECTORIES})
target_link_libraries(wpe ${WPE_LIBRARIES})

set_target_properties(wpe PROPERTIES
  OUTPUT_NAME wpe-${WPE_API_VERSION}
  VERSION ${LIBWPE_VERSION}
  SOVERSION ${LIBWPE_VERSION_MAJOR}
)

install(TARGETS wpe
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
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

option(BUILD_DOCS "Build the documentation" OFF)
IF(BUILD_DOCS)
  find_program(HOTDOC hotdoc)
  IF(NOT HOTDOC)
    message(FATAL_ERROR "hotdoc not found!")
  ENDIF()

  execute_process(
    COMMAND ${HOTDOC} --has-extension c-extension
    RESULT_VARIABLE HAS_HOTDOC_C_EXTENSION
    ERROR_QUIET
  )
  IF("${HAS_HOTDOC_C_EXTENSION}" EQUAL 0)
    add_custom_target(Documentation ALL
      ${HOTDOC} run
      --project-name=libwpe
      --project-version=1.0
      --include-paths="${CMAKE_SOURCE_DIR}/docs"
      --sitemap=${CMAKE_SOURCE_DIR}/docs/sitemap.txt
      --output=${CMAKE_CURRENT_BINARY_DIR}/Documentation/
      --c-sources "${CMAKE_SOURCE_DIR}/include/wpe/*.h"
                  "${DERIVED_SOURCES_DIR}/version.h"
                  "${DERIVED_SOURCES_DIR}/version-deprecated.h"
      --extra-c-flags=-DWPE_COMPILATION=1
      --c-include-directories ${CMAKE_SOURCE_DIR}/include ${DERIVED_SOURCES_DIR}
      --c-smart-index
    )
  ELSE()
      MESSAGE(FATAL_ERROR "Hotdoc C extension not found... can't build the documentation.")
  ENDIF()
ENDIF()
