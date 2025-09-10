#pragma once

#define PB_SYSTEM_HEADER "my_system.h"
#define PB_ENABLE_MALLOC 1

#define pb_realloc(ptr, size) arena_realloc(ptr, size)
#define pb_free(ptr) arena_free(ptr)
