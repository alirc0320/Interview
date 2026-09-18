#include "minitest.h"
#include "solution.h"

#include <vector>

// Contract: LeetCode 658. `arr` is sorted ascending. Return the k closest
// integers to x, sorted ascending. Ties in distance prefer the smaller value.

using V = std::vector<int>;

TEST(basic_example)
{
    CHECK(find_closest_elements({1, 2, 3, 4, 5}, 4, 3) == (V{1, 2, 3, 4}));
}

TEST(x_below_range)
{
    CHECK(find_closest_elements({1, 2, 3, 4, 5}, 4, -1) == (V{1, 2, 3, 4}));
}

TEST(x_above_range)
{
    CHECK(find_closest_elements({1, 2, 3, 4, 5}, 4, 10) == (V{2, 3, 4, 5}));
}

TEST(tie_prefers_smaller_value)
{
    // 1 and 3 are both distance 1 from 2 -> pick 1.
    CHECK(find_closest_elements({1, 3}, 1, 2) == (V{1}));
    CHECK(find_closest_elements({1, 2, 4, 5}, 2, 3) == (V{2, 4}));
    CHECK(find_closest_elements({0, 0, 1, 2, 3, 3, 4, 7, 8, 8}, 3, 5) == (V{3, 3, 4}));
}

TEST(k_equals_one)
{
    CHECK(find_closest_elements({1, 5, 10}, 1, 6) == (V{5}));
    CHECK(find_closest_elements({1, 5, 10}, 1, 8) == (V{10}));
    CHECK(find_closest_elements({1, 5, 10}, 1, 7) == (V{5}));
}

TEST(k_equals_size_returns_everything)
{
    CHECK(find_closest_elements({1, 2, 3}, 3, 100) == (V{1, 2, 3}));
    CHECK(find_closest_elements({7}, 1, 0) == (V{7}));
}

TEST(result_is_sorted_even_when_window_straddles_x)
{
    CHECK(find_closest_elements({1, 10, 15, 25, 35, 45, 50, 59}, 1, 30) == (V{25}));
    CHECK(find_closest_elements({1, 1, 1, 10, 10, 10}, 1, 9) == (V{10}));
    CHECK(find_closest_elements({1, 10, 15, 25, 35, 45, 50, 59}, 3, 30) == (V{15, 25, 35}));
}

TEST(duplicates)
{
    CHECK(find_closest_elements({1, 1, 1, 2, 2, 2, 3, 3, 3}, 3, 2) == (V{2, 2, 2}));
    CHECK(find_closest_elements({0, 0, 0, 0}, 2, 5) == (V{0, 0}));
}

TEST(negative_values)
{
    CHECK(find_closest_elements({-10, -5, -1, 0, 4}, 2, -4) == (V{-5, -1}));
    CHECK(find_closest_elements({-10, -5, -1, 0, 4}, 3, -6) == (V{-10, -5, -1}));
}

TEST(extreme_values_do_not_overflow_distance)
{
    const int lo = -2147483647 - 1;
    const int hi = 2147483647;
    CHECK(find_closest_elements({lo, 0, hi}, 1, hi) == (V{hi}));
    CHECK(find_closest_elements({lo, 0, hi}, 1, lo) == (V{lo}));
    CHECK(find_closest_elements({lo, hi}, 1, 0) == (V{hi})); // distances 2^31 vs 2^31-1
}

TEST_MAIN()
