#pragma once
#include <cstddef>

// TODO: implement memmove semantics (correct for overlapping and
// non-overlapping ranges). Define behavior for zero length and be
// ready to explain which copy direction is safe for each overlap
// arrangement.
void* my_memmove(void* dst, const void* src, std::size_t n);
