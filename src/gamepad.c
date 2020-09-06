/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gamepad-private.h"

#include "loader-private.h"
#include <stdlib.h>

struct wpe_gamepad_provider*
wpe_gamepad_provider_create()
{
    struct wpe_gamepad_provider_interface* provider_interface = wpe_load_object("_wpe_gamepad_provider_interface");
    if (!provider_interface)
        return 0;

    struct wpe_gamepad_provider* provider = calloc(1, sizeof(struct wpe_gamepad_provider));
    if (!provider)
        return 0;

    provider->interface = provider_interface;
    provider->interface_data = provider->interface->create(provider);
    return provider;
}

void
wpe_gamepad_provider_destroy(struct wpe_gamepad_provider* provider)
{
    provider->interface->destroy(provider->interface_data);
    provider->interface_data = 0;
    free(provider);
}

void
wpe_gamepad_provider_set_client(struct wpe_gamepad_provider* provider, struct wpe_gamepad_provider_client* client, void* client_data)
{
    provider->provider_client = client;
    provider->provider_client_data = client_data;
}

void
wpe_gamepad_provider_start(struct wpe_gamepad_provider* provider)
{
    provider->interface->start(provider->interface_data);
}

void
wpe_gamepad_provider_stop(struct wpe_gamepad_provider* provider)
{
    provider->interface->stop(provider->interface_data);
}

struct wpe_view_backend*
wpe_gamepad_provider_get_view_for_gamepad_input(struct wpe_gamepad_provider* provider, struct wpe_gamepad* gamepad)
{
    return provider->interface->get_view_for_gamepad_input(provider->interface_data, gamepad ? gamepad->interface_data : 0);
}

void
wpe_gamepad_provider_dispatch_gamepad_connected(struct wpe_gamepad_provider* provider, uint32_t gamepad_id)
{
    if (provider->provider_client)
        provider->provider_client->gamepad_connected(provider->provider_client_data, gamepad_id);
}

void
wpe_gamepad_provider_dispatch_gamepad_disconnected(struct wpe_gamepad_provider* provider, uint32_t gamepad_id)
{
    if (provider->provider_client)
        provider->provider_client->gamepad_disconnected(provider->provider_client_data, gamepad_id);
}

struct wpe_gamepad*
wpe_gamepad_create(struct wpe_gamepad_provider* provider, uint32_t gamepad_id)
{
    struct wpe_gamepad_interface* gamepad_interface = wpe_load_object("_wpe_gamepad_interface");
    if (!gamepad_interface)
        return 0;

    struct wpe_gamepad* gamepad = calloc(1, sizeof(struct wpe_gamepad));
    if (!gamepad)
        return 0;

    gamepad->gamepad_id = gamepad_id;
    gamepad->interface = gamepad_interface;
    gamepad->interface_data = gamepad->interface->create(provider->interface_data, gamepad, gamepad_id);
    return gamepad;
}

void
wpe_gamepad_destroy(struct wpe_gamepad* gamepad)
{
    gamepad->interface->destroy(gamepad->interface_data);
    gamepad->interface_data = 0;
    free(gamepad);
}

void
wpe_gamepad_set_client(struct wpe_gamepad* gamepad, struct wpe_gamepad_client* client, void* client_data)
{
    gamepad->gamepad_client = client;
    gamepad->gamepad_client_data = client_data;
}
uint32_t
wpe_gamepad_get_id(struct wpe_gamepad* gamepad)
{
    return gamepad->interface->get_id(gamepad->interface_data);
}

const char*
wpe_gamepad_get_device_name(struct wpe_gamepad* gamepad)
{
    return gamepad->interface->get_device_name(gamepad->interface_data);
}

uint32_t
wpe_gamepad_get_button_count(struct wpe_gamepad* gamepad)
{
    return gamepad->interface->get_button_count(gamepad->interface_data);
}

uint32_t
wpe_gamepad_copy_button_values(struct wpe_gamepad* gamepad, double* button_array, uint32_t button_array_length)
{
    return gamepad->interface->copy_button_values(gamepad->interface_data, button_array, button_array_length);
}

uint32_t
wpe_gamepad_get_axis_count(struct wpe_gamepad* gamepad)
{
    return gamepad->interface->get_axis_count(gamepad->interface_data);
}

uint32_t
wpe_gamepad_copy_axis_values(struct wpe_gamepad* gamepad, double* axis_array, uint32_t axis_array_length)
{
    return gamepad->interface->copy_axis_values(gamepad->interface_data, axis_array, axis_array_length);
}

void
wpe_gamepad_dispatch_button_values_changed(struct wpe_gamepad* gamepad)
{
    if (gamepad->gamepad_client)
        gamepad->gamepad_client->button_values_changed(gamepad, gamepad->gamepad_client_data);
}

void
wpe_gamepad_dispatch_axis_values_changed(struct wpe_gamepad* gamepad)
{
    if (gamepad->gamepad_client)
        gamepad->gamepad_client->axis_values_changed(gamepad, gamepad->gamepad_client_data);
}
