cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -D_POSIX_SOURCE")

if(WPE_BACKEND)
  add_definitions(-DWPE_BACKEND=\"${WPE_BACKEND}\")
endif()

set(WPE_INCLUDE_DIRECTORIES
    "include"
    "src"
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
)

add_library(WPE SHARED ${WPE_SOURCES})
target_include_directories(WPE PRIVATE ${WPE_INCLUDE_DIRECTORIES})
target_link_libraries(WPE ${WPE_LIBRARIES})

set(WPE_VERSION_MAJOR 0)
set(WPE_VERSION_MINOR 1)
set(WPE_VERSION ${WPE_VERSION_MAJOR}.${WPE_VERSION_MINOR})

set_target_properties(WPE PROPERTIES VERSION ${WPE_VERSION} SOVERSION ${WPE_VERSION_MAJOR})

install(TARGETS WPE EXPORT WPETargets
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
)
install(
  FILES
    ${WPE_PUBLIC_HEADERS}
    DESTINATION
    include/wpe
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/WPEConfigVersion.cmake"
  VERSION ${WPE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT WPETargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/WPETargets.cmake"
)
configure_file(WPEConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/WPEConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/WPE)
install(EXPORT WPETargets
  FILE
    WPETargets.cmake
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    WPEConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/WPEConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
)
