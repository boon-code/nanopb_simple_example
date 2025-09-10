#!/bin/sh

gcc \
    "$@" \
    -D 'PB_SYSTEM_HEADER="my_system.h"' \
    -I nanopb/ -I . \
    simple.c \
    simple.pb.c \
    nanopb/*.c \
    static_alloc.c \
    -o test
