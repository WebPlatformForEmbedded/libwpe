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

#include "loader-private.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTHOF(array) (sizeof(array) / sizeof(array[0]))

static void* s_impl_library = 0;
static struct wpe_loader_interface* s_impl_loader = 0;

#ifndef WPE_BACKEND
static char* s_impl_library_path = NULL;
static char s_impl_library_path_buffer[512];
#endif

#ifndef WPE_BACKEND
static void
wpe_loader_set_impl_library_path(const char* impl_library_path)
{
    if (!impl_library_path)
        return;

    size_t len = strlen(impl_library_path) + 1;
    if (len == 1)
        return;

    if (len > LENGTHOF(s_impl_library_path_buffer))
        s_impl_library_path = malloc(len);
    else
        s_impl_library_path = s_impl_library_path_buffer;
    memcpy(s_impl_library_path, impl_library_path, len);
}
#endif /* !WPE_BACKEND */

void
load_impl_library(void)
{
#ifdef WPE_BACKEND
    s_impl_library = dlopen(WPE_BACKEND, RTLD_NOW);
    if (!s_impl_library) {
        fprintf(stderr, "wpe: could not load compile-time defined WPE_BACKEND: %s\n", dlerror());
        abort();
    }
#else /* !WPE_BACKEND */
#ifndef NDEBUG
    // Get the impl library from an environment variable, if available.
    const char* env_library_path = getenv("WPE_BACKEND_LIBRARY");
    if (env_library_path) {
        s_impl_library = dlopen(env_library_path, RTLD_NOW);
        if (!s_impl_library) {
            fprintf(stderr, "wpe: could not load specified WPE_BACKEND_LIBRARY: %s\n", dlerror());
            abort();
        }
        wpe_loader_set_impl_library_path(env_library_path);
    }
#endif /* !NDEBUG */
    if (!s_impl_library) {
        // Load libWPEBackend-default.so by ... default.
        s_impl_library = dlopen(WPE_BACKENDS_DIR "/libWPEBackend-default.so", RTLD_NOW);
        if (!s_impl_library) {
            fprintf(stderr, "wpe: could not load the impl library. Is there any backend installed?: %s\n", dlerror());
            abort();
        }
        wpe_loader_set_impl_library_path(WPE_BACKENDS_DIR "/libWPEBackend-default.so");
    }
#endif /* WPE_BACKEND */

    s_impl_loader = dlsym(s_impl_library, "_wpe_loader_interface");
}

bool
wpe_loader_init(const char* impl_library_name)
{
#ifndef WPE_BACKEND
    if (!(impl_library_name && impl_library_name[0] != '\0')) {
        fprintf(stderr, "wpe_loader_init: invalid implementation library name\n");
        abort();
    }

    const bool relative_path = (impl_library_name[0] != '/');

    size_t len = strlen(impl_library_name) + 1 + (relative_path ? LENGTHOF(WPE_BACKENDS_DIR) : 0);
    char impl_library_path[len];

    if (relative_path)
        snprintf(impl_library_path, len, WPE_BACKENDS_DIR "/%s", impl_library_name);
    else
        strncpy(impl_library_path, impl_library_name, len);

    if (s_impl_library) {
        if (!s_impl_library_path || strcmp(s_impl_library_path, impl_library_path) != 0) {
            fprintf(stderr, "wpe_loader_init: already initialized\n");
            return false;
        }
        return true;
    }

    s_impl_library = dlopen(impl_library_path, RTLD_NOW);
    if (!s_impl_library) {
        fprintf(stderr, "wpe_loader_init could not load the library '%s': %s\n", impl_library_path, dlerror());
        return false;
    }
    wpe_loader_set_impl_library_path(impl_library_path);

    s_impl_loader = dlsym(s_impl_library, "_wpe_loader_interface");
    return true;
#else /* WPE_BACKEND */
    return false;
#endif /* !WPE_BACKEND */
}

const char*
wpe_loader_get_loaded_implementation_library_name(void)
{
#ifdef WPE_BACKEND
    return s_impl_library ? WPE_BACKEND : NULL;
#else /* !WPE_BACKEND */
    return s_impl_library_path;
#endif /* WPE_BACKEND */
}

void*
wpe_load_object(const char* object_name)
{
    if (!s_impl_library)
        load_impl_library();

    if (s_impl_loader) {
        if (!s_impl_loader->load_object) {
            fprintf(stderr, "wpe_load_object: failed to load object with name '%s': backend doesn't implement load_object vfunc\n", object_name);
            abort();
        }
        return s_impl_loader->load_object(object_name);
    }

    void* object = dlsym(s_impl_library, object_name);
    if (!object)
        fprintf(stderr, "wpe_load_object: failed to load object with name '%s'\n", object_name);

    return object;
}
