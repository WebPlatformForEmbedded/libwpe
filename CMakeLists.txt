cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(VersioningUtils)

read_version_header("" "WPE_[A-Z]+_VERSION" "${CMAKE_CURRENT_SOURCE_DIR}/include/wpe/libwpe-version.h")
set_project_version(${WPE_MAJOR_VERSION} ${WPE_MINOR_VERSION} ${WPE_MICRO_VERSION})
set(WPE_API_VERSION "1.0")

# Before making a release, the LT_VERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A
calculate_library_versions_from_libtool_triple(LIBWPE 6 0 5)

project(libwpe VERSION "${PROJECT_VERSION}")

set(WPE_BACKEND
    ""
    CACHE STRING "Name of the backend library to load, instead of libWPEBackend-default.so"
)

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

foreach (cxxflag -fno-exceptions -fno-rtti -Wall)
    check_cxx_compiler_flag("${cxxflag}" CXX_HAS_FLAG_${cxxflag})
    if (CXX_HAS_FLAG_${cxxflag})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxxflag}")
    endif ()
endforeach ()

foreach (cflag -Wall)
    check_c_compiler_flag("${cflag}" CC_HAS_FLAG_${cflag})
    if (CC_HAS_FLAG_${cflag})
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${cflag}")
    endif ()
endforeach ()

include(DistTargets)
include(GNUInstallDirs)

find_package(EGL REQUIRED)
find_package(Libxkbcommon REQUIRED)

set(WPE_PUBLIC_HEADERS
    include/wpe/libwpe-version.h
    include/wpe/version.h
    include/wpe/version-deprecated.h
    include/wpe/export.h
    include/wpe/input.h
    include/wpe/keysyms.h
    include/wpe/loader.h
    include/wpe/pasteboard.h
    include/wpe/renderer-backend-egl.h
    include/wpe/renderer-host.h
    include/wpe/view-backend.h
    include/wpe/wpe-egl.h
    include/wpe/wpe.h
)

add_library(
    wpe SHARED
    src/input.c
    src/key-unicode.c
    src/loader.c
    src/pasteboard.c
    src/pasteboard-generic.cpp
    src/pasteboard-noop.cpp
    src/renderer-backend-egl.c
    src/renderer-host.c
    src/version.c
    src/view-backend.c
)
target_include_directories(
    wpe PRIVATE "include" "src" $<TARGET_PROPERTY:GL::egl,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(
    wpe PRIVATE WPE_COMPILATION $<TARGET_PROPERTY:GL::egl,INTERFACE_COMPILE_DEFINITIONS>
)
if (WPE_BACKEND)
    target_compile_definitions(wpe PRIVATE WPE_BACKEND=\"${WPE_BACKEND}\")
endif ()
target_compile_options(wpe PRIVATE $<TARGET_PROPERTY:GL::egl,INTERFACE_COMPILE_OPTIONS>)
target_link_libraries(wpe PRIVATE XkbCommon::libxkbcommon ${CMAKE_DL_LIBS})

set_target_properties(
    wpe
    PROPERTIES
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
        OUTPUT_NAME wpe-${WPE_API_VERSION}
        VERSION ${LIBWPE_VERSION}
        SOVERSION ${LIBWPE_VERSION_MAJOR}
)

install(
    TARGETS wpe
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${WPE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wpe-${WPE_API_VERSION}/wpe)

configure_file(wpe.pc.in wpe-${WPE_API_VERSION}.pc @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/wpe-${WPE_API_VERSION}.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

option(BUILD_DOCS "Build the documentation" OFF)
if (BUILD_DOCS)
    find_program(HOTDOC hotdoc)
    if (NOT HOTDOC)
        message(FATAL_ERROR "hotdoc not found!")
    endif ()

    execute_process(
        COMMAND ${HOTDOC} --has-extension c-extension
        RESULT_VARIABLE HAS_HOTDOC_C_EXTENSION
        ERROR_QUIET
    )
    if ("${HAS_HOTDOC_C_EXTENSION}" EQUAL 0)
        add_custom_target(
            Documentation ALL
            ${HOTDOC}
            run
            --project-name=libwpe
            --project-version=1.0
            --include-paths="${CMAKE_CURRENT_SOURCE_DIR}/docs"
            --sitemap=${CMAKE_CURRENT_SOURCE_DIR}/docs/sitemap.txt
            --output=${CMAKE_CURRENT_BINARY_DIR}/Documentation/
            --c-sources
            "${CMAKE_CURRENT_SOURCE_DIR}/include/wpe/*.h"
            --extra-c-flags=-DWPE_COMPILATION=1
            --c-include-directories
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            --c-smart-index
        )
    else ()
        message(FATAL_ERROR "Hotdoc C extension not found... can't build the documentation.")
    endif ()
endif ()
