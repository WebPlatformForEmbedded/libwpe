/*
 * Copyright (C) 2015, 2016 Igalia S.L.
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

#ifndef wpe_loader_h
#define wpe_loader_h

#if defined(WPE_COMPILATION)
#include <wpe/export.h>
#endif

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct wpe_loader_interface {
    void* (*load_object)(const char*);
    void (*_wpe_reserved0)(void);
    void (*_wpe_reserved1)(void);
    void (*_wpe_reserved2)(void);
    void (*_wpe_reserved3)(void);
};

WPE_EXPORT
bool
wpe_loader_init(const char* impl_library_name);

WPE_EXPORT
const char*
wpe_loader_get_loaded_implementation_library_name(void);

#ifdef __cplusplus
}
#endif

#endif /* wpe_loader_h */
