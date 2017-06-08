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
#include <stdlib.h>


__attribute__((visibility("default")))
struct wpe_view_backend*
wpe_view_backend_create()
{
    struct wpe_view_backend_interface* backend_interface = wpe_load_object("_wpe_view_backend_interface");
    if (!backend_interface)
        return 0;

    return wpe_view_backend_create_with_backend_interface(backend_interface, 0);
}

__attribute__((visibility("default")))
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

__attribute__((visibility("default")))
void
wpe_view_backend_destroy(struct wpe_view_backend* backend)
{
    backend->interface->destroy(backend->interface_data);
    memset(backend, 0, sizeof(*backend));
    free(backend);
}

__attribute__((visibility("default")))
void
wpe_view_backend_set_backend_client(struct wpe_view_backend* backend, const struct wpe_view_backend_client* client, void* client_data)
{
    backend->backend_client = client;
    backend->backend_client_data = client_data;
}

__attribute__((visibility("default")))
void
wpe_view_backend_set_input_client(struct wpe_view_backend* backend, const struct wpe_input_client* client, void* client_data)
{
    backend->input.client = client;
    backend->input.client_data = client_data;
}

__attribute__((visibility("default")))
void
wpe_view_backend_initialize(struct wpe_view_backend* backend)
{
    backend->interface->initialize(backend->interface_data);
}

__attribute__((visibility("default")))
int
wpe_view_backend_get_renderer_host_fd(struct wpe_view_backend* backend)
{
    return backend->interface->get_renderer_host_fd(backend->interface_data);
}

__attribute__((visibility("default")))
struct wpe_input*
wpe_view_backend_get_input(struct wpe_view_backend* backend)
{
    return &backend->input;
}

__attribute__((visibility("default")))
void
wpe_view_backend_dispatch_set_size(struct wpe_view_backend* backend, uint32_t width, uint32_t height)
{
    if (backend->backend_client)
        backend->backend_client->set_size(backend->backend_client_data, width, height);
}

__attribute__((visibility("default")))
void
wpe_view_backend_dispatch_frame_displayed(struct wpe_view_backend* backend)
{
    if (backend->backend_client)
        backend->backend_client->frame_displayed(backend->backend_client_data);
}

__attribute__((visibility("default")))
void
wpe_input_dispatch_keyboard_event(struct wpe_input* backend, struct wpe_input_keyboard_event* event)
{
    if (backend->client)
        backend->client->handle_keyboard_event(backend->client_data, event);
}

__attribute__((visibility("default")))
void
wpe_input_dispatch_pointer_event(struct wpe_input* backend, struct wpe_input_pointer_event* event)
{
    if (backend->client)
        backend->client->handle_pointer_event(backend->client_data, event);
}

__attribute__((visibility("default")))
void
wpe_input_dispatch_axis_event(struct wpe_input* backend, struct wpe_input_axis_event* event)
{
    if (backend->client)
        backend->client->handle_axis_event(backend->client_data, event);
}

__attribute__((visibility("default")))
void
wpe_input_dispatch_touch_event(struct wpe_input* backend, struct wpe_input_touch_event* event)
{
    if (backend->client)
        backend->client->handle_touch_event(backend->client_data, event);
}
