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
#ifndef wpe_gamepad_private_h
#define wpe_gamepad_private_h

#include <wpe/gamepad.h>

#ifdef __cplusplus
extern "C" {
#endif

struct wpe_gamepad_provider {
    struct wpe_gamepad_provider_interface* interface;
    void* interface_data;
    const struct wpe_gamepad_provider_client* provider_client;
    void* provider_client_data;
};

struct wpe_gamepad {
    struct wpe_gamepad_interface* interface;
    void* interface_data;
    const struct wpe_gamepad_client* gamepad_client;
    void* gamepad_client_data;
    uint32_t gamepad_id;
};

#ifdef __cplusplus
}
#endif

#endif /* wpe_gamepad_private_h */
