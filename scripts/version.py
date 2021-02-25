#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2021 Igalia S.L.
#
# Distributed under terms of the MIT license.

from os import environ, path
import re

version = {}
version_re = re.compile(r"^#define\s+WPE_([A-Z]+)_VERSION\s+(\d+)$")
version_file = path.join(environ["MESON_SOURCE_ROOT"],
                         "include", "wpe", "libwpe-version.h")

with open(version_file, "r") as f:
    for line in f.readlines():
        m = version_re.match(line)
        if m:
            version[m[1]] = m[2]

print("{}.{}.{}".format(version["MAJOR"], version["MINOR"], version["MICRO"]))
