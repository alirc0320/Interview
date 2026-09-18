#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <limits>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - Row-major. C = A x B, A is M x K, B is K x N, C is M x N.
//   - Returns true on success, false on invalid arguments; C is written
//     only on success.
//   - Null a/b/c -> false (for non-empty results).
//   - M*K, K*N, or M*N overflowing size_t -> false, without touching
//     memory (the pointers below are real but tiny).
//   - K == 0 -> C is filled with zeros (empty sum). M == 0 or N == 0
//     -> nothing to compute, returns true.
//
// All values are small integers so float arithmetic is exact and
// results can be compared with ==.
// ---------------------------------------------------------------------

namespace
{
bool equal(const std::vector<float>& got, const std::vector<float>& want)
{
    return got == want;
}
} // namespace

TEST(two_by_two)
{
    // [1 2]   [5 6]   [19 22]
    // [3 4] x [7 8] = [43 50]
    std::vector<float> a = {1, 2, 3, 4};
    std::vector<float> b = {5, 6, 7, 8};
    std::vector<float> c(4, -1.0f);
    CHECK(matmul(a.data(), b.data(), c.data(), 2, 2, 2));
    CHECK(equal(c, {19, 22, 43, 50}));
}

TEST(identity_leaves_matrix_unchanged)
{
    std::vector<float> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<float> id = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    std::vector<float> c(9, -1.0f);
    CHECK(matmul(a.data(), id.data(), c.data(), 3, 3, 3));
    CHECK(equal(c, a));
    CHECK(matmul(id.data(), a.data(), c.data(), 3, 3, 3));
    CHECK(equal(c, a));
}

TEST(rectangular_2x3_times_3x2)
{
    // [1 2 3]   [ 7  8]   [ 58  64]
    // [4 5 6] x [ 9 10] = [139 154]
    //           [11 12]
    std::vector<float> a = {1, 2, 3, 4, 5, 6};
    std::vector<float> b = {7, 8, 9, 10, 11, 12};
    std::vector<float> c(4, -1.0f);
    CHECK(matmul(a.data(), b.data(), c.data(), 2, 3, 2));
    CHECK(equal(c, {58, 64, 139, 154}));
}

TEST(rectangular_3x2_times_2x4)
{
    std::vector<float> a = {1, 2, 3, 4, 5, 6};                // 3x2
    std::vector<float> b = {1, 0, 2, 1, 0, 1, 1, 2};          // 2x4
    std::vector<float> c(12, -1.0f);                           // 3x4
    CHECK(matmul(a.data(), b.data(), c.data(), 3, 2, 4));
    // row0: [1 2] -> [1, 2, 4, 5]
    // row1: [3 4] -> [3, 4, 10, 11]
    // row2: [5 6] -> [5, 6, 16, 17]
    CHECK(equal(c, {1, 2, 4, 5, 3, 4, 10, 11, 5, 6, 16, 17}));
}

TEST(row_vector_times_column_vector_is_dot_product)
{
    std::vector<float> a = {1, 2, 3};
    std::vector<float> b = {4, 5, 6};
    std::vector<float> c(1, -1.0f);
    CHECK(matmul(a.data(), b.data(), c.data(), 1, 3, 1));
    CHECK(equal(c, {32}));
}

TEST(column_vector_times_row_vector_is_outer_product)
{
    std::vector<float> a = {1, 2, 3}; // 3x1
    std::vector<float> b = {4, 5};    // 1x2
    std::vector<float> c(6, -1.0f);
    CHECK(matmul(a.data(), b.data(), c.data(), 3, 1, 2));
    CHECK(equal(c, {4, 5, 8, 10, 12, 15}));
}

TEST(one_by_one)
{
    float a = 3, b = 4, c = -1;
    CHECK(matmul(&a, &b, &c, 1, 1, 1));
    CHECK(c == 12.0f);
}

TEST(output_is_overwritten_not_accumulated)
{
    std::vector<float> a = {1, 1, 1, 1};
    std::vector<float> b = {1, 1, 1, 1};
    std::vector<float> c(4, 100.0f); // stale data must not leak into the result
    CHECK(matmul(a.data(), b.data(), c.data(), 2, 2, 2));
    CHECK(equal(c, {2, 2, 2, 2}));
}

TEST(negative_and_zero_values)
{
    std::vector<float> a = {-1, 0, 2, -3};
    std::vector<float> b = {4, -5, 0, 6};
    std::vector<float> c(4, -1.0f);
    CHECK(matmul(a.data(), b.data(), c.data(), 2, 2, 2));
    // [-1*4+0*0, -1*-5+0*6] = [-4, 5]
    // [ 2*4+-3*0, 2*-5+-3*6] = [8, -28]
    CHECK(equal(c, {-4, 5, 8, -28}));
}

TEST(larger_matrix_against_reference_loop)
{
    const std::size_t m = 7, k = 5, n = 9;
    std::vector<float> a(m * k), b(k * n), c(m * n, -1.0f), expected(m * n, 0.0f);
    for (std::size_t i = 0; i < a.size(); ++i)
        a[i] = static_cast<float>(static_cast<int>(i % 7) - 3);
    for (std::size_t i = 0; i < b.size(); ++i)
        b[i] = static_cast<float>(static_cast<int>(i % 5) - 2);
    for (std::size_t i = 0; i < m; ++i)
        for (std::size_t j = 0; j < n; ++j)
            for (std::size_t p = 0; p < k; ++p)
                expected[i * n + j] += a[i * k + p] * b[p * n + j];

    CHECK(matmul(a.data(), b.data(), c.data(), m, k, n));
    CHECK(equal(c, expected));
}

TEST(null_pointers_are_rejected)
{
    std::vector<float> a = {1, 2, 3, 4}, b = {1, 2, 3, 4}, c(4, -1.0f);
    CHECK(!matmul(nullptr, b.data(), c.data(), 2, 2, 2));
    CHECK(!matmul(a.data(), nullptr, c.data(), 2, 2, 2));
    CHECK(!matmul(a.data(), b.data(), nullptr, 2, 2, 2));
}

TEST(failure_does_not_write_to_output)
{
    std::vector<float> a = {1, 2, 3, 4}, c(4, 42.0f);
    CHECK(!matmul(a.data(), nullptr, c.data(), 2, 2, 2));
    CHECK(equal(c, {42, 42, 42, 42}));
}

TEST(zero_inner_dimension_yields_zero_matrix)
{
    float dummy = 0;
    std::vector<float> c(6, 7.0f);
    CHECK(matmul(&dummy, &dummy, c.data(), 2, 0, 3));
    CHECK(equal(c, {0, 0, 0, 0, 0, 0}));
}

TEST(zero_rows_or_cols_is_a_successful_noop)
{
    float dummy = 0;
    float c = 7.0f;
    CHECK(matmul(&dummy, &dummy, &c, 0, 3, 2));
    CHECK(matmul(&dummy, &dummy, &c, 2, 3, 0));
    CHECK(c == 7.0f); // untouched
}

TEST(dimension_overflow_is_rejected)
{
    const std::size_t big = std::numeric_limits<std::size_t>::max() / 2 + 1;
    float dummy = 0;
    // m*k overflows
    CHECK(!matmul(&dummy, &dummy, &dummy, big, 2, 1));
    // k*n overflows
    CHECK(!matmul(&dummy, &dummy, &dummy, 1, big, 2));
    // m*n overflows
    CHECK(!matmul(&dummy, &dummy, &dummy, big, 1, 2));
}

TEST(size_max_dimensions_are_rejected)
{
    const std::size_t max = std::numeric_limits<std::size_t>::max();
    float dummy = 0;
    CHECK(!matmul(&dummy, &dummy, &dummy, max, max, max));
}

TEST_MAIN()
