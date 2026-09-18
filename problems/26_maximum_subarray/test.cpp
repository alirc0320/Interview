#include "minitest.h"
#include "solution.h"

#include <vector>

// Contract: LeetCode 53. Maximum sum of a non-empty contiguous subarray.
// (Empty input isn't tested.)

TEST(classic_example)
{
    CHECK_EQ(max_subarray({-2, 1, -3, 4, -1, 2, 1, -5, 4}), 6);
}

TEST(single_element)
{
    CHECK_EQ(max_subarray({5}), 5);
    CHECK_EQ(max_subarray({-5}), -5);
    CHECK_EQ(max_subarray({0}), 0);
}

TEST(all_positive_takes_everything)
{
    CHECK_EQ(max_subarray({5, 4, -1, 7, 8}), 23);
    CHECK_EQ(max_subarray({1, 2, 3, 4}), 10);
}

TEST(all_negative_takes_the_largest_single)
{
    CHECK_EQ(max_subarray({-3, -1, -2}), -1);
    CHECK_EQ(max_subarray({-8, -7, -6, -5}), -5);
}

TEST(best_subarray_at_the_start)
{
    CHECK_EQ(max_subarray({10, -20, 1, 2}), 10);
}

TEST(best_subarray_at_the_end)
{
    CHECK_EQ(max_subarray({1, 2, -20, 10}), 10);
}

TEST(bridges_a_negative_when_worth_it)
{
    CHECK_EQ(max_subarray({5, -1, 5}), 9);
    CHECK_EQ(max_subarray({5, -6, 5}), 5);
}

TEST(zeros)
{
    CHECK_EQ(max_subarray({0, 0, 0}), 0);
    CHECK_EQ(max_subarray({-1, 0, -2}), 0);
}

TEST(restarts_after_sum_goes_negative)
{
    CHECK_EQ(max_subarray({-2, -3, 4, -1, -2, 1, 5, -3}), 7);
}

TEST(large_values_within_int)
{
    CHECK_EQ(max_subarray({1000000000, 1000000000, -2000000000, 1000000000}), 2000000000);
}

TEST_MAIN()
