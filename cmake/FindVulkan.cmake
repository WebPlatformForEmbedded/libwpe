# - Try to Find Vulkan
# Once done, this will define
#
#  VULKAN_FOUND - system has Vulkan installed.
#  VULKAN_INCLUDE_DIRS - directories which contain the Vulkan headers.
#  VULKAN_LIBRARIES - libraries required to link against Vulkan.
#  VULKAN_DEFINITIONS - Compiler switches required for using Vulkan.
#
# Copyright (C) 2019 Igalia S.L.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND ITS CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


find_package(PkgConfig)

pkg_check_modules(PC_VULKAN vulkan)

if (PC_VULKAN_FOUND)
    set(VULKAN_DEFINITIONS ${PC_VULKAN_CFLAGS_OTHERS})
endif ()

find_path(VULKAN_INCLUDE_DIRS NAMES vulkan/vulkan.h
    HINTS ${PC_VULKAN_INCLUDEDIR} ${PC_VULKAN_INCLUDE_DIRS}
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vulkan DEFAULT_MSG VULKAN_INCLUDE_DIRS)

mark_as_advanced(VULKAN_INCLUDE_DIRS)
