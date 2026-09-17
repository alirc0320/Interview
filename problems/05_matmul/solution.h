#pragma once
#include <cstddef>

// C = A x B (row-major). A is M x K, B is K x N, C is M x N.
// TODO: implement. Validate arguments and guard dimension arithmetic
// against overflow.
bool matmul(const float* a, const float* b, float* c, std::size_t m, std::size_t k, std::size_t n);
