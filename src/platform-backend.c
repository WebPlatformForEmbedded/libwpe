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

#include <wpe/platform-backend.h>

#include "view-backend-private.h"

void
wpe_view_backend_set_platform_interface(struct wpe_view_backend* backend, const struct wpe_view_platform_interface* interface, void* data)
{
    backend->platform_interface = interface;
    backend->platform_interface_data = data;
}

void
wpe_popup_set_interface(struct wpe_popup* popup, const struct wpe_popup_interface* interface, void* interface_data)
{
    popup->interface = interface;
    popup->interface_data = interface_data;
}

void
wpe_popup_dispatch_dismissed(struct wpe_popup* popup)
{
    if (popup->popup_client)
        popup->popup_client->dismissed(popup->popup_client_data);
}

void
wpe_popup_dispatch_frame_displayed(struct wpe_popup* popup)
{
    if (popup->popup_client)
        popup->popup_client->frame_displayed(popup->popup_client_data);
}

void
wpe_buffer_set_interface(struct wpe_buffer* buffer, const struct wpe_buffer_interface* interface, void* interface_data)
{
    buffer->interface = interface;
    buffer->interface_data = interface_data;
}

void
wpe_buffer_dispatch_release(struct wpe_buffer* buffer)
{
    if (buffer->buffer_client)
        buffer->buffer_client->release(buffer->client_data);
}


