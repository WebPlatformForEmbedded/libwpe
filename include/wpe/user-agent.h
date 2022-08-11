/*
 * Copyright (C) 2022 Igalia S.L.
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

#ifndef wpe_user_agent_h
#define wpe_user_agent_h

/**
 * SECTION:user-agent
 * @short_description: Library User Agent
 * @title: User Agent
 */

#if defined(WPE_ENABLE_UA) && WPE_ENABLE_UA

#if defined(WPE_COMPILATION)
#include "export.h"
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct wpe_user_agent_provider;

struct wpe_user_agent_provider_interface {
    void* (*create)(struct wpe_user_agent_provider*);
    void (*destroy)(void*);
    const char* (*architecture)(void*);
    uint8_t (*bitness)(void*);
    bool (*mobile)(void*);
    const char* (*model)(void*);
    const char* (*platform)(void*);
    const char* (*platform_version)(void*);
};

WPE_EXPORT
struct wpe_user_agent_provider* wpe_user_agent_provider_create(void);

WPE_EXPORT
void wpe_user_agent_provider_destroy(struct wpe_user_agent_provider*);

WPE_EXPORT
const char* wpe_user_agent_get_architecture(struct wpe_user_agent_provider*);

WPE_EXPORT
uint8_t wpe_user_agent_get_bitness(struct wpe_user_agent_provider*);

WPE_EXPORT
bool wpe_user_agent_is_mobile(struct wpe_user_agent_provider*);

WPE_EXPORT
const char* wpe_user_agent_get_model(struct wpe_user_agent_provider*);

WPE_EXPORT
const char* wpe_user_agent_get_platform(struct wpe_user_agent_provider*);

WPE_EXPORT
const char* wpe_user_agent_get_platform_version(struct wpe_user_agent_provider*);

WPE_EXPORT
void wpe_user_agent_register_interface(struct wpe_user_agent_provider_interface*);

#ifdef __cplusplus
}
#endif

#endif /* defined(WPE_ENABLE_UA) && WPE_ENABLE_UA */

#endif /* wpe_user_agent_h */
