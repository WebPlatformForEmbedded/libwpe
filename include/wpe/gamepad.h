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

#if !defined(__WPE_H_INSIDE__) && !defined(WPE_COMPILATION)
#error "Only <wpe/wpe.h> can be included directly."
#endif

#ifndef wpe_gamepad_h
#define wpe_gamepad_h

#if defined(WPE_COMPILATION)
#include <wpe/export.h>
#endif

#include <stdint.h>

/*
 * https://www.w3.org/TR/gamepad/#remapping
 *
 * buttons[0] Bottom button in right cluster
 * buttons[1] Right button in right cluster
 * buttons[2] Left button in right cluster
 * buttons[3] Top button in right cluster
 * buttons[4] Top left front button
 * buttons[5] Top right front button
 * buttons[6] Bottom left front button
 * buttons[7] Bottom right front button
 * buttons[8] Left button in center cluster
 * buttons[9] Right button in center cluster
 * buttons[10] Left stick pressed button
 * buttons[11] Right stick pressed button
 * buttons[12] Top button in left cluster
 * buttons[13] Bottom button in left cluster
 * buttons[14] Right button in left cluster
 * buttons[15] Left button in left cluster
 *
 * axes[0] Horizontal axis for left stick (negative left/positive right)
 * axes[1] Vertical axis for left stick (negative up/positive down)
 * axes[2] Horizontal axis for right stick (negative left/positive right)
 * axes[3] Vertical axis for right stick (negative up/positive down)
 */

#ifdef __cplusplus
extern "C" {
#endif

struct wpe_view_backend;

struct wpe_gamepad;
struct wpe_gamepad_client;

struct wpe_gamepad_provider;
struct wpe_gamepad_provider_client;

struct wpe_gamepad_provider_interface {
    void* (*create)(struct wpe_gamepad_provider*);
    void  (*destroy)(void*);
    void  (*start)(void*);
    void  (*stop)(void*);
    struct wpe_view_backend* (*get_view_for_gamepad_input)(void*, void*);
};

struct wpe_gamepad_interface {
    void* (*create)(void*, struct wpe_gamepad*, uint32_t);
    void  (*destroy)(void*);
    uint32_t (*get_id)(void*);
    const char* (*get_device_name)(void*);
    uint32_t (*get_button_count)(void*);
    uint32_t (*copy_button_values)(void*, double*, uint32_t);
    uint32_t (*get_axis_count)(void*);
    uint32_t (*copy_axis_values)(void*, double*, uint32_t);
};

WPE_EXPORT
struct wpe_gamepad_provider*
wpe_gamepad_provider_create();

WPE_EXPORT
void
wpe_gamepad_provider_destroy(struct wpe_gamepad_provider*);

WPE_EXPORT
void
wpe_gamepad_provider_set_client(struct wpe_gamepad_provider*, struct wpe_gamepad_provider_client*, void*);

WPE_EXPORT
void
wpe_gamepad_provider_start(struct wpe_gamepad_provider*);

WPE_EXPORT
void
wpe_gamepad_provider_stop(struct wpe_gamepad_provider*);

WPE_EXPORT
struct wpe_view_backend*
wpe_gamepad_provider_get_view_for_gamepad_input(struct wpe_gamepad_provider*, struct wpe_gamepad*);

WPE_EXPORT
void
wpe_gamepad_provider_dispatch_gamepad_connected(struct wpe_gamepad_provider*, uint32_t gamepad_id);

WPE_EXPORT
void
wpe_gamepad_provider_dispatch_gamepad_disconnected(struct wpe_gamepad_provider*, uint32_t gamepad_id);

WPE_EXPORT
struct wpe_gamepad*
wpe_gamepad_create(struct wpe_gamepad_provider* provider, uint32_t gamepad_id);

WPE_EXPORT
void
wpe_gamepad_destroy(struct wpe_gamepad*);

WPE_EXPORT
void
wpe_gamepad_set_client(struct wpe_gamepad*, struct wpe_gamepad_client*, void*);

WPE_EXPORT
uint32_t
wpe_gamepad_get_id(struct wpe_gamepad*);

WPE_EXPORT
const char*
wpe_gamepad_get_device_name(struct wpe_gamepad*);

WPE_EXPORT
uint32_t
wpe_gamepad_get_button_count(struct wpe_gamepad*);

WPE_EXPORT
uint32_t
wpe_gamepad_copy_button_values(struct wpe_gamepad*, double*, uint32_t);

WPE_EXPORT
uint32_t
wpe_gamepad_get_axis_count(struct wpe_gamepad*);

WPE_EXPORT
uint32_t
wpe_gamepad_copy_axis_values(struct wpe_gamepad*, double*, uint32_t);

WPE_EXPORT
void
wpe_gamepad_dispatch_button_values_changed(struct wpe_gamepad*);

WPE_EXPORT
void
wpe_gamepad_dispatch_axis_values_changed(struct wpe_gamepad*);

struct wpe_gamepad_provider_client {
    void (*gamepad_connected)(void*, uint32_t gamepad_id);
    void (*gamepad_disconnected)(void*, uint32_t gamepad_id);
};

struct wpe_gamepad_client {
    void (*button_values_changed)(struct wpe_gamepad*, void*);
    void (*axis_values_changed)(struct wpe_gamepad*, void*);
};

#ifdef __cplusplus
}
#endif

#endif /* wpe_gamepad_h */
