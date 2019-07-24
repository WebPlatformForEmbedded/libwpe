/*
 * Copyright (C) 2019 Garmin Ltd. or its subsidiaries
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

#pragma once

#if !defined(__WPE_H_INSIDE__) && !defined(WPE_COMPILATION)
#error "Only <wpe/wpe.h> can be included directly."
#endif

#if defined(WPE_COMPILATION)
#include <wpe/export.h>
#endif

#include <stdbool.h>
#include <stdint.h>
/*  Four-character-code (FOURCC) */
#define wpe_fourcc(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

#define WPE_FOURCC_XRGB8888 wpe_fourcc('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */
#define WPE_FOURCC_ARGB8888 wpe_fourcc('A', 'R', '2', '4') /* [31:0] A:R:G:B 8:8:8:8 little endian */

#ifdef __cplusplus
extern "C" {
#endif

struct wpe_view_backend;
struct wpe_popup;
struct wpe_popup_info;
struct wpe_buffer;
struct wpe_buffer_info;

struct wpe_view_platform_interface {
    bool (*create_popup)(void*, struct wpe_popup*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
    bool (*alloc_buffer)(void*, struct wpe_buffer*, uint32_t, uint32_t, uint32_t);

    void (*_wpe_reserved0)(void);
    void (*_wpe_reserved1)(void);
    void (*_wpe_reserved2)(void);
    void (*_wpe_reserved3)(void);
};

struct wpe_popup_interface {
    void (*destroy)(void*);
    void (*attach_buffer)(void*, void*);
    void (*get_info)(void*, struct wpe_popup_info*);
};

struct wpe_buffer_interface {
    void (*destroy)(void*);
    void (*get_info)(void*, struct wpe_buffer_info* info);
};

WPE_EXPORT
void
wpe_view_backend_set_platform_interface(struct wpe_view_backend*, const struct wpe_view_platform_interface*, void*);

WPE_EXPORT
void
wpe_popup_set_interface(struct wpe_popup*, const struct wpe_popup_interface*, void*);

WPE_EXPORT
void
wpe_popup_dispatch_dismissed(struct wpe_popup*);

WPE_EXPORT
void
wpe_popup_dispatch_frame_displayed(struct wpe_popup*);

WPE_EXPORT
void
wpe_buffer_set_interface(struct wpe_buffer*, const struct wpe_buffer_interface*, void*);

WPE_EXPORT
void
wpe_buffer_dispatch_release(struct wpe_buffer*);

#ifdef __cplusplus
}
#endif

