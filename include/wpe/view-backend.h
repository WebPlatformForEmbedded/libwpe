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

#ifndef wpe_view_backend_h
#define wpe_view_backend_h

#if defined(WPE_COMPILATION)
#include <wpe/export.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct wpe_view_backend;

struct wpe_input_axis_event;
struct wpe_input_keyboard_event;
struct wpe_input_pointer_event;
struct wpe_input_touch_event;

struct wpe_view_backend_client;
struct wpe_view_backend_input_client;

struct wpe_view_backend_interface {
    void* (*create)(void*, struct wpe_view_backend*);
    void (*destroy)(void*);

    void (*initialize)(void*);
    int (*get_renderer_host_fd)(void*);

    void (*_wpe_reserved0)(void);
    void (*_wpe_reserved1)(void);
    void (*_wpe_reserved2)(void);
    void (*_wpe_reserved3)(void);
};


WPE_EXPORT
struct wpe_view_backend*
wpe_view_backend_create();

WPE_EXPORT
struct wpe_view_backend*
wpe_view_backend_create_with_backend_interface(struct wpe_view_backend_interface*, void*);

WPE_EXPORT
void
wpe_view_backend_destroy(struct wpe_view_backend*);

WPE_EXPORT
void 
wpe_view_backend_set_backend_client(struct wpe_view_backend*, const struct wpe_view_backend_client*, void*);

WPE_EXPORT
void
wpe_view_backend_set_input_client(struct wpe_view_backend*, const struct wpe_view_backend_input_client*, void*);

WPE_EXPORT
void
wpe_view_backend_initialize(struct wpe_view_backend*);

WPE_EXPORT
int
wpe_view_backend_get_renderer_host_fd(struct wpe_view_backend*);

enum wpe_view_activity_state {
    wpe_view_activity_state_visible   = 1 << 0,
    wpe_view_activity_state_focused   = 1 << 1,
    wpe_view_activity_state_in_window = 1 << 2
};

struct wpe_view_backend_client {
    void (*set_size)(void*, uint32_t, uint32_t);
    void (*frame_displayed)(void*);
    void (*activity_state_changed)(void*, uint32_t);
    void* (*get_accessible)(void*);
    void (*set_device_scale_factor)(void*, float);
    void (*_wpe_reserved0)(void);
};

WPE_EXPORT
void
wpe_view_backend_dispatch_set_size(struct wpe_view_backend*, uint32_t, uint32_t);

WPE_EXPORT
void
wpe_view_backend_dispatch_frame_displayed(struct wpe_view_backend*);

WPE_EXPORT
void
wpe_view_backend_add_activity_state(struct wpe_view_backend*, uint32_t);

WPE_EXPORT
void
wpe_view_backend_remove_activity_state(struct wpe_view_backend*, uint32_t);

WPE_EXPORT
uint32_t
wpe_view_backend_get_activity_state(struct wpe_view_backend*);

WPE_EXPORT
void*
wpe_view_backend_dispatch_get_accessible(struct wpe_view_backend* backend);

WPE_EXPORT
void
wpe_view_backend_dispatch_set_device_scale_factor(struct wpe_view_backend*, float);

struct wpe_view_backend_input_client {
    void (*handle_keyboard_event)(void*, struct wpe_input_keyboard_event*);
    void (*handle_pointer_event)(void*, struct wpe_input_pointer_event*);
    void (*handle_axis_event)(void*, struct wpe_input_axis_event*);
    void (*handle_touch_event)(void*, struct wpe_input_touch_event*);
    void (*_wpe_reserved0)(void);
    void (*_wpe_reserved1)(void);
    void (*_wpe_reserved2)(void);
    void (*_wpe_reserved3)(void);
};

WPE_EXPORT
void
wpe_view_backend_dispatch_keyboard_event(struct wpe_view_backend*, struct wpe_input_keyboard_event*);

WPE_EXPORT
void
wpe_view_backend_dispatch_pointer_event(struct wpe_view_backend*, struct wpe_input_pointer_event*);

WPE_EXPORT
void
wpe_view_backend_dispatch_axis_event(struct wpe_view_backend*, struct wpe_input_axis_event*);

WPE_EXPORT
void
wpe_view_backend_dispatch_touch_event(struct wpe_view_backend*, struct wpe_input_touch_event*);

#ifdef __cplusplus
}
#endif

#endif /* wpe_view_backend_h */
