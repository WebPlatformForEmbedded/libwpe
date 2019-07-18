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

#include <wpe/input-method.h>

#include "input-method-private.h"
#include <stdlib.h>


struct wpe_input_method*
wpe_input_method_create()
{
    struct wpe_input_method* input_method = calloc(1, sizeof(struct wpe_input_method));
    return input_method;
}

void
wpe_input_method_destroy(struct wpe_input_method* input_method)
{
    free(input_method);
}

void
wpe_input_method_set_controller(struct wpe_input_method* input_method, struct wpe_input_method_controller* controller, void* controller_data)
{
    input_method->controller = controller;
    input_method->controller_data = controller_data;
}

void
wpe_input_method_enable(struct wpe_input_method* input_method)
{
    if (input_method->controller)
        input_method->controller->enable(input_method->controller_data);
}

void
wpe_input_method_disable(struct wpe_input_method* input_method)
{
    if (input_method->controller)
        input_method->controller->disable(input_method->controller_data);
}
