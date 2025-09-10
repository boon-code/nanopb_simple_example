#include "static_alloc.h"
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>

#define dbg(fmt, ...) do { fprintf(stderr, fmt "\n",##__VA_ARGS__); } while (0)


static uint8_t *s_arena = NULL;
static size_t s_size = 0U;
static void *s_last = NULL;
static size_t s_last_size = 0U;

#if 0
#define ALIGNED_SIZE(_size, _align) (((_align) - (((_align) - 1U) & (_size))) & ((_align) - 1U) + (_size))

static size_t get_aligned_size(size_t size, size_t align)
{
	size_t mask = size & (align - 1U);
	return (align - (mask & size)) & mask + size;
}
#endif

void arena_init(void *ptr, size_t size)
{
	s_arena = ptr;
	s_size = size;
	s_last = NULL;
	s_last_size = 0U;
}

void arena_deinit(void)
{
	arena_init(NULL, 0U);
}

void *arena_realloc(void *__ptr, size_t __size)
{
	dbg("request %zu bytes with pointer %p", __size, __ptr);

	if ((s_arena != NULL) && (s_size >= 0U)) {
		if (__ptr != NULL) { // actual realloc
			if (s_last != __ptr) {
				dbg("! realloc %p: can only extend the last memory %p region", __ptr, s_last);
				return NULL;
			}

			if (s_last_size > __size) { // shrink
				size_t diff = s_last_size - __size;
				s_last_size -= diff;
				s_size += diff;
				s_arena -= diff;
				if (s_last_size == 0U) {
					dbg("> free last memory %p (free %zu bytes)", __ptr, diff);
					s_last = NULL; // basically ~ free()
				} else {
					dbg("> shrink last memory %p by %zu bytes", __ptr, diff);
				}
			} else if (s_last_size < __size) { // expand
				size_t diff = __size - s_last_size;
				if (diff > s_size) {
					dbg("! expand last memory %p failed -> oom (free=%zu)", __ptr, s_size);
					return NULL;
				}

				s_last_size += diff;
				s_size -= diff;
				s_arena += diff;
				dbg("> expand last memory %p by %zu bytes (free=%zu)", __ptr, diff, s_size);
			}

			return s_last;
		} else { // malloc
			assert((__ptr == NULL) && "__ptr must be NULL here");
			if (__size > s_size) {
				dbg("! malloc: oom (free=%zu)", s_size);
				return NULL;
			}

			s_size -= __size;
			s_arena += __size;
			s_last_size = __size;
			s_last = s_arena;

			dbg("> malloc succeeded (free=%zu)", s_size);

			return s_last;
		}
	} else {
		dbg("arena not initialized");
		return NULL;
	}
}

extern void arena_free(void *__ptr)
{
	if (__ptr == NULL) {
		dbg("free(NULL) -> nop");
		return;
	}

	if (s_arena != NULL) {
		if (s_last == __ptr) {
			assert((__ptr != NULL) && "__ptr mustn't be a NULL pointer here");
			s_size += s_last_size;
			s_arena -= s_last_size;
			s_last_size = 0U;
			s_last = NULL;
		} else {
			dbg("can't free ptr %p", __ptr);
		}
	} else {
		dbg("arena not initialized");
	}
}
