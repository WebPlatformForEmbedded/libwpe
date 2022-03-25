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

#if defined(WPE_ENABLE_UA) && WPE_ENABLE_UA

#include "../include/wpe/user-agent.h"
#include <stdlib.h>

struct wpe_user_agent_provider {
    void* backend;
};

static struct wpe_user_agent_provider_interface* provider_interface = NULL;

struct wpe_user_agent_provider*
wpe_user_agent_provider_create()
{
    if (!provider_interface)
        return NULL;

    struct wpe_user_agent_provider* provider = calloc(1, sizeof(struct wpe_user_agent_provider));
    if (!provider)
        return NULL;

    if (provider_interface->create)
        provider->backend = provider_interface->create(provider);

    return provider;
}

void
wpe_user_agent_provider_destroy(struct wpe_user_agent_provider* provider)
{
    if (!provider)
        return;

    if (provider_interface && provider_interface->destroy)
        provider_interface->destroy(provider);
    provider->backend = NULL;
    free(provider);
}

const char*
wpe_user_agent_get_architecture(struct wpe_user_agent_provider* provider)
{
    if (provider && provider_interface && provider_interface->architecture)
        return provider_interface->architecture(provider->backend);
    return NULL;
}

uint8_t
wpe_user_agent_get_bitness(struct wpe_user_agent_provider* provider)
{
    if (provider && provider_interface && provider_interface->bitness)
        return provider_interface->bitness(provider->backend);
    return 0;
}

bool
wpe_user_agent_is_mobile(struct wpe_user_agent_provider* provider)
{
    if (provider && provider_interface && provider_interface->mobile)
        return provider_interface->mobile(provider->backend);
    return false;
}

const char*
wpe_user_agent_get_model(struct wpe_user_agent_provider* provider)
{
    if (provider && provider_interface && provider_interface->model)
        return provider_interface->model(provider->backend);
    return NULL;
}

const char*
wpe_user_agent_get_platform(struct wpe_user_agent_provider* provider)
{
    if (provider && provider_interface && provider_interface->platform)
        return provider_interface->platform(provider->backend);
    return NULL;
}

const char*
wpe_user_agent_get_platform_version(struct wpe_user_agent_provider* provider)
{
    if (provider && provider_interface && provider_interface->platform_version)
        return provider_interface->platform_version(provider->backend);
    return NULL;
}

void
wpe_user_agent_register_interface(struct wpe_user_agent_provider_interface* provider)
{
    if (provider && !provider_interface)
        provider_interface = provider;
}

#endif /* defined(WPE_ENABLE_UA) && WPE_ENABLE_UA */
