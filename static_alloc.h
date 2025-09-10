#pragma once

#include <features.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>


extern void arena_init(void *ptr, size_t size);

extern void arena_deinit(void);

extern void *arena_realloc(void *__ptr, size_t __size) __attribute_warn_unused_result__;

extern void arena_free(void *__ptr);
