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

#include <wpe/view-backend.h>

#include "loader-private.h"
#include "view-backend-private.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


struct wpe_view_backend*
wpe_view_backend_create()
{
    struct wpe_view_backend_interface* backend_interface = wpe_load_object("_wpe_view_backend_interface");
    if (!backend_interface)
        return 0;

    return wpe_view_backend_create_with_backend_interface(backend_interface, 0);
}

struct wpe_view_backend*
wpe_view_backend_create_with_backend_interface(struct wpe_view_backend_interface* interface, void* interface_user_data)
{
    struct wpe_view_backend* backend = calloc(1, sizeof(struct wpe_view_backend));
    if (!backend)
        return 0;

    backend->interface = interface;
    backend->interface_data = backend->interface->create(interface_user_data, backend);

    return backend;
}

void
wpe_view_backend_destroy(struct wpe_view_backend* backend)
{
    backend->interface->destroy(backend->interface_data);
    memset(backend, 0, sizeof(*backend));
    free(backend);
}

static void
wpe_view_backend_notify_activity_state_changed(struct wpe_view_backend* backend)
{
    if (backend->backend_client && backend->backend_client->activity_state_changed)
        backend->backend_client->activity_state_changed(backend->backend_client_data, backend->activity_state);
}

void
wpe_view_backend_set_backend_client(struct wpe_view_backend* backend, const struct wpe_view_backend_client* client, void* client_data)
{
    backend->backend_client = client;
    backend->backend_client_data = client_data;
    if (backend->activity_state)
        wpe_view_backend_notify_activity_state_changed(backend);
}

void
wpe_view_backend_set_input_client(struct wpe_view_backend* backend, const struct wpe_input_client* client, void* client_data)
{
    backend->input.client = client;
    backend->input.client_data = client_data;
}

struct wpe_input*
wpe_view_backend_get_input(struct wpe_view_backend* backend)
{
    return &backend->input;
}

struct wpe_popup*
wpe_view_backend_create_popup(struct wpe_view_backend* backend, int32_t anchor_x, int32_t anchor_y, int32_t anchor_width, int32_t anchor_height,
                              int32_t width, int32_t height, const struct wpe_popup_client* popup_client, const struct wpe_input_client* input_client,
                              void* client_data)
{
    struct wpe_popup* popup;

    if (!backend->platform_interface || !backend->platform_interface->create_popup)
        return NULL;

    popup = calloc(1, sizeof(*popup));

    if (!popup)
        return NULL;

    popup->popup_client = popup_client;
    popup->popup_client_data = client_data;
    popup->input.client = input_client;
    popup->input.client_data = client_data;

    if (!backend->platform_interface->create_popup(backend->platform_interface_data, popup, anchor_x, anchor_y, anchor_width, anchor_height, width, height)) {
        free(popup);
        return NULL;
    }

    assert(popup->interface);
    return popup;
}

struct wpe_buffer*
wpe_view_backend_alloc_buffer(struct wpe_view_backend* backend, const struct wpe_buffer_client* buffer_client, void* client_data, uint32_t format, uint32_t width, uint32_t height)
{
    struct wpe_buffer* buffer;

    if (!backend->platform_interface || !backend->platform_interface->alloc_buffer)
        return NULL;

    buffer = calloc(1, sizeof(*buffer));
    if (!buffer)
        return NULL;

    buffer->buffer_client = buffer_client;
    buffer->client_data = client_data;

    if (!backend->platform_interface->alloc_buffer(backend->platform_interface_data, buffer, format, width, height)) {
        free(buffer);
        return NULL;
    }

    assert(buffer->interface);
    return buffer;
}

void
wpe_view_backend_initialize(struct wpe_view_backend* backend)
{
    backend->interface->initialize(backend->interface_data);
}

int
wpe_view_backend_get_renderer_host_fd(struct wpe_view_backend* backend)
{
    return backend->interface->get_renderer_host_fd(backend->interface_data);
}

void
wpe_view_backend_dispatch_set_size(struct wpe_view_backend* backend, uint32_t width, uint32_t height)
{
    if (backend->backend_client)
        backend->backend_client->set_size(backend->backend_client_data, width, height);
}

void
wpe_view_backend_dispatch_frame_displayed(struct wpe_view_backend* backend)
{
    if (backend->backend_client)
        backend->backend_client->frame_displayed(backend->backend_client_data);
}

void
wpe_view_backend_add_activity_state(struct wpe_view_backend* backend, uint32_t state)
{
    const uint32_t new_state = backend->activity_state | state;
    if (backend->activity_state != new_state) {
        backend->activity_state = new_state;
        wpe_view_backend_notify_activity_state_changed(backend);
    }
}

void
wpe_view_backend_remove_activity_state(struct wpe_view_backend* backend, uint32_t state)
{
    const uint32_t new_state = backend->activity_state & ~state;
    if (backend->activity_state != new_state) {
        backend->activity_state = new_state;
        wpe_view_backend_notify_activity_state_changed(backend);
    }
}

uint32_t
wpe_view_backend_get_activity_state(struct wpe_view_backend* backend)
{
    return backend->activity_state;
}

void*
wpe_view_backend_dispatch_get_accessible(struct wpe_view_backend* backend)
{
    if (backend->backend_client && backend->backend_client->get_accessible)
        return backend->backend_client->get_accessible(backend->backend_client_data);
    return NULL;
}

void
wpe_view_backend_dispatch_set_device_scale_factor(struct wpe_view_backend* backend, float scale)
{
    if (backend->backend_client && backend->backend_client->set_device_scale_factor)
        backend->backend_client->set_device_scale_factor(backend->backend_client_data, scale);
}

void
wpe_view_backend_dispatch_keyboard_event(struct wpe_view_backend* backend, struct wpe_input_keyboard_event* event)
{
    wpe_input_dispatch_keyboard_event(&backend->input, event);
}

void
wpe_view_backend_dispatch_pointer_event(struct wpe_view_backend* backend, struct wpe_input_pointer_event* event)
{
    wpe_input_dispatch_pointer_event(&backend->input, event);
}

void
wpe_view_backend_dispatch_axis_event(struct wpe_view_backend* backend, struct wpe_input_axis_event* event)
{
    wpe_input_dispatch_axis_event(&backend->input, event);
}

void
wpe_view_backend_dispatch_touch_event(struct wpe_view_backend* backend, struct wpe_input_touch_event* event)
{
    wpe_input_dispatch_touch_event(&backend->input, event);
}

void
wpe_input_dispatch_keyboard_event(struct wpe_input* backend, struct wpe_input_keyboard_event* event)
{
    if (backend->client)
        backend->client->handle_keyboard_event(backend->client_data, event);
}

void
wpe_input_dispatch_pointer_event(struct wpe_input* backend, struct wpe_input_pointer_event* event)
{
    if (backend->client)
        backend->client->handle_pointer_event(backend->client_data, event);
}

void
wpe_input_dispatch_axis_event(struct wpe_input* backend, struct wpe_input_axis_event* event)
{
    if (backend->client)
        backend->client->handle_axis_event(backend->client_data, event);
}

void
wpe_input_dispatch_touch_event(struct wpe_input* backend, struct wpe_input_touch_event* event)
{
    if (backend->client)
        backend->client->handle_touch_event(backend->client_data, event);
}

void
wpe_popup_destroy(struct wpe_popup* popup)
{
    popup->interface->destroy(popup->interface_data);
    memset(popup, 0, sizeof(*popup));
    free(popup);
}

void
wpe_popup_attach_buffer(struct wpe_popup* popup, struct wpe_buffer* buffer)
{
    popup->interface->attach_buffer(popup->interface_data, buffer ? buffer->interface_data : NULL);
}

struct wpe_input*
wpe_popup_get_input(struct wpe_popup* popup)
{
    return &popup->input;
}

void
wpe_popup_get_info(struct wpe_popup* popup, struct wpe_popup_info* info)
{
    popup->interface->get_info(popup->interface_data, info);
}

void
wpe_buffer_destroy(struct wpe_buffer* buffer)
{
    buffer->interface->destroy(buffer->interface_data);
    memset(buffer, 0, sizeof(*buffer));
    free(buffer);
}

void
wpe_buffer_get_info(struct wpe_buffer* buffer, struct wpe_buffer_info* info)
{
    buffer->interface->get_info(buffer->interface_data, info);
}

