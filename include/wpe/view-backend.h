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

#ifndef wpe_view_backend_h
#define wpe_view_backend_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
/*  Four-character-code (FOURCC) */
#define wpe_fourcc(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

#define WPE_FOURCC_XRGB8888 wpe_fourcc('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */
#define WPE_FOURCC_ARGB8888 wpe_fourcc('A', 'R', '2', '4') /* [31:0] A:R:G:B 8:8:8:8 little endian */

struct wpe_view_backend;

struct wpe_input;

struct wpe_input_axis_event;
struct wpe_input_keyboard_event;
struct wpe_input_pointer_event;
struct wpe_input_touch_event;

struct wpe_popup;

struct wpe_buffer;
struct wpe_buffer_info;

struct wpe_view_backend_client;
struct wpe_input_client;
struct wpe_popup_client;
struct wpe_buffer_client;

struct wpe_view_backend_interface {
    void* (*create)(void*, struct wpe_view_backend*);
    void (*destroy)(void*);

    void (*initialize)(void*);
    int (*get_renderer_host_fd)(void*);

    bool (*create_popup)(void*, struct wpe_popup*, int32_t, int32_t);
    bool (*alloc_buffer)(void*, struct wpe_buffer*, uint32_t, uint32_t, uint32_t);
};

struct wpe_popup_interface {
    void (*destroy)(void*);
    void (*attach_buffer)(void*, void*);
};

struct wpe_buffer_interface {
    void (*destroy)(void*);
    void (*get_info)(void*, struct wpe_buffer_info* info);
};


struct wpe_view_backend*
wpe_view_backend_create();

struct wpe_view_backend*
wpe_view_backend_create_with_backend_interface(struct wpe_view_backend_interface*, void*);

void
wpe_view_backend_destroy(struct wpe_view_backend*);

void
wpe_view_backend_set_backend_client(struct wpe_view_backend*, const struct wpe_view_backend_client*, void*);

void
wpe_view_backend_set_input_client(struct wpe_view_backend*, const struct wpe_input_client*, void*);

void
wpe_view_backend_initialize(struct wpe_view_backend*);

int
wpe_view_backend_get_renderer_host_fd(struct wpe_view_backend*);

struct wpe_input*
wpe_view_backend_get_input(struct wpe_view_backend*);

struct wpe_view_backend_client {
    void (*set_size)(void*, uint32_t, uint32_t);
    void (*frame_displayed)(void*);
};

struct wpe_popup*
wpe_view_backend_create_popup(struct wpe_view_backend*, int32_t, int32_t, const struct wpe_popup_client*, const struct wpe_input_client*, void*);

struct wpe_buffer*
wpe_view_backend_alloc_buffer(struct wpe_view_backend*, const struct wpe_buffer_client*, void*, uint32_t, uint32_t, uint32_t);

void
wpe_view_backend_dispatch_set_size(struct wpe_view_backend*, uint32_t, uint32_t);

void
wpe_view_backend_dispatch_frame_displayed(struct wpe_view_backend*);


struct wpe_input_client {
    void (*handle_keyboard_event)(void*, struct wpe_input_keyboard_event*);
    void (*handle_pointer_event)(void*, struct wpe_input_pointer_event*);
    void (*handle_axis_event)(void*, struct wpe_input_axis_event*);
    void (*handle_touch_event)(void*, struct wpe_input_touch_event*);
};

void
wpe_input_dispatch_keyboard_event(struct wpe_input*, struct wpe_input_keyboard_event*);

void
wpe_input_dispatch_pointer_event(struct wpe_input*, struct wpe_input_pointer_event*);

void
wpe_input_dispatch_axis_event(struct wpe_input*, struct wpe_input_axis_event*);

void
wpe_input_dispatch_touch_event(struct wpe_input*, struct wpe_input_touch_event*);

void
wpe_popup_set_interface(struct wpe_popup*, const struct wpe_popup_interface*, void*);

void
wpe_popup_destroy(struct wpe_popup*);

void
wpe_popup_attach_buffer(struct wpe_popup*, struct wpe_buffer*);

struct wpe_input*
wpe_popup_get_input(struct wpe_popup*);

struct wpe_popup_client {
    void (*dismissed)(void*);
    void (*frame_displayed)(void*);
};

void
wpe_popup_dispatch_dismissed(struct wpe_popup*);

void
wpe_popup_dispatch_frame_displayed(struct wpe_popup*);

void
wpe_buffer_set_interface(struct wpe_buffer*, const struct wpe_buffer_interface*, void*);

struct wpe_buffer_info {
    uint32_t format;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    void* data;
};

void
wpe_buffer_destroy(struct wpe_buffer*);

void
wpe_buffer_get_info(struct wpe_buffer*, struct wpe_buffer_info*);

struct wpe_buffer_client {
    void (*release)(void*);
};

void
wpe_buffer_dispatch_release(struct wpe_buffer*);

#ifdef __cplusplus
}
#endif

#endif // wpe_view_backend_h
