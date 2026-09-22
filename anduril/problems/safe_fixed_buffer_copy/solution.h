#pragma once
#include <stdbool.h>
#include <stddef.h>

/* Bounded string copy/append into a fixed-size stack/static buffer — the
 * "why not just use strcpy" question (GFG Q18/Q19/Q20: stack overflow,
 * fragmentation, memory leaks all trace back to exactly this kind of
 * unchecked write). No malloc anywhere; dst is always caller-owned,
 * fixed-capacity storage.
 *
 * Both functions must NEVER write to dst[dst_capacity] or beyond, and
 * must ALWAYS leave dst NUL-terminated on return (even when dst_capacity
 * is 0 — in that case, they simply cannot write anything, including no
 * terminator, and must say so via the return value rather than writing
 * out of bounds). */

/* Returns true if all of src (including its terminator) fit within
 * dst_capacity bytes; false if truncated (dst is still NUL-terminated,
 * just shorter than src) or if dst_capacity == 0 (nothing was written). */
bool safe_strcpy(char *dst, size_t dst_capacity, const char *src);

/* Appends src onto the end of the NUL-terminated string already in dst.
 * Returns true if all of src fit; false if truncated, if dst wasn't
 * already NUL-terminated within dst_capacity, or if dst_capacity == 0. */
bool safe_strcat(char *dst, size_t dst_capacity, const char *src);
