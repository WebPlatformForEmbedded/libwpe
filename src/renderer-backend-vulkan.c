/*
 * Copyright (C) 2019 Igalia S.L.
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

#include <wpe/renderer-backend-vulkan.h>

#include "loader-private.h"
#include "renderer-backend-vulkan-private.h"
#include <stdlib.h>

struct wpe_renderer_backend_vulkan*
wpe_renderer_backend_vulkan_create(int host_fd)
{
    struct wpe_renderer_backend_vulkan* backend = calloc(1, sizeof(struct wpe_renderer_backend_vulkan));
    if (!backend)
        return 0;

    backend->interface = wpe_load_object("_wpe_renderer_backend_vulkan_interface");
    if (!backend->interface) {
        free(backend);
        return 0;
    }

    backend->interface_data = backend->interface->create(host_fd);

    return backend;
}

void
wpe_renderer_backend_vulkan_destroy(struct wpe_renderer_backend_vulkan* backend)
{
    backend->interface->destroy(backend->interface_data);
    backend->interface_data = 0;

    free(backend);
}

void
wpe_renderer_backend_vulkan_initialize(struct wpe_renderer_backend_vulkan* backend, const VkApplicationInfo* application_info, const VkAllocationCallbacks* allocator, const struct wpe_renderer_backend_vulkan_initialization_parameters* initialization_parameters)
{
    backend->interface->initialize(backend->interface_data, application_info, allocator, initialization_parameters);
}

VkInstance
wpe_renderer_backend_vulkan_get_instance(struct wpe_renderer_backend_vulkan* backend)
{
    return backend->interface->get_instance(backend->interface_data);
}

bool
wpe_renderer_backend_vulkan_supports_physical_device(struct wpe_renderer_backend_vulkan* backend, VkPhysicalDevice physical_device, uint32_t queue_family_index)
{
    return backend->interface->supports_physical_device(backend->interface_data, physical_device, queue_family_index);
}

struct wpe_renderer_backend_vulkan_target*
wpe_renderer_backend_vulkan_target_create(int host_fd)
{
    struct wpe_renderer_backend_vulkan_target* target = calloc(1, sizeof(struct wpe_renderer_backend_vulkan_target));
    if (!target)
        return 0;

    target->interface = wpe_load_object("_wpe_renderer_backend_vulkan_target_interface");
    if (!target->interface) {
        free(target);
        return 0;
    }

    target->interface_data = target->interface->create(target, host_fd);

    return target;
}

void
wpe_renderer_backend_vulkan_target_destroy(struct wpe_renderer_backend_vulkan_target* target)
{
    target->interface->destroy(target->interface_data);
    target->interface_data = 0;

    free(target);
}

void
wpe_renderer_backend_vulkan_target_set_client(struct wpe_renderer_backend_vulkan_target* target, const struct wpe_renderer_backend_vulkan_target_client* client, void* client_data)
{
    target->client = client;
    target->client_data = client_data;
}

void
wpe_renderer_backend_vulkan_target_initialize(struct wpe_renderer_backend_vulkan_target* target, struct wpe_renderer_backend_vulkan* backend, uint32_t width, uint32_t height)
{
    target->interface->initialize(target->interface_data, backend->interface_data, width, height);
}

VkSurfaceKHR
wpe_renderer_backend_vulkan_target_get_surface(struct wpe_renderer_backend_vulkan_target* target)
{
    return target->interface->get_surface(target->interface_data);
}

void
wpe_renderer_backend_vulkan_target_resize(struct wpe_renderer_backend_vulkan_target* target, uint32_t width, uint32_t height)
{
    target->interface->resize(target->interface_data, width, height);
}

void
wpe_renderer_backend_vulkan_target_frame_will_render(struct wpe_renderer_backend_vulkan_target* target)
{
    target->interface->frame_will_render(target->interface_data);
}

void
wpe_renderer_backend_vulkan_target_frame_rendered(struct wpe_renderer_backend_vulkan_target* target)
{
    target->interface->frame_rendered(target->interface_data);
}

void
wpe_renderer_backend_vulkan_target_dispatch_frame_complete(struct wpe_renderer_backend_vulkan_target* target)
{
    if (target->client)
        target->client->frame_complete(target->client_data);
}
