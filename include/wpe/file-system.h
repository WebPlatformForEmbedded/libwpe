/*
 * Copyright (C) 2022 Igalia S.L.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__WPE_H_INSIDE__) && !defined(WPE_COMPILATION)
#error "Only <wpe/wpe.h> can be included directly."
#endif

#ifndef wpe_file_system_h
#define wpe_file_system_h

/**
 * SECTION:file-system
 * @short_description: Library File System
 * @title: File System
 */

#if defined(WPE_ENABLE_FS) && WPE_ENABLE_FS

#if defined(WPE_COMPILATION)
#include "export.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum wpe_file_system_path {
    WPE_FILE_SYSTEM_PATH_USER_DATA,
    WPE_FILE_SYSTEM_PATH_USER_CACHE,
};

typedef const char* (*wpe_file_system_path_handler)(void* userdata, enum wpe_file_system_path);

/* Used by the embedder to customize paths. */
WPE_EXPORT
void wpe_file_system_path_set_handler(wpe_file_system_path_handler handler, void* userdata);

/* Used inside WebKit to obtain the paths, if a handler is not set or returns NULL, may fall back to g_get_user_{cache,data}_dir */
WPE_EXPORT
const char* wpe_file_system_path_get(enum wpe_file_system_path);

#ifdef __cplusplus
}
#endif

#endif /* defined(WPE_ENABLE_FS) && WPE_ENABLE_FS */

#endif /* wpe_file_system_h */
