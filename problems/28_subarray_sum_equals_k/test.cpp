#include "minitest.h"
#include "solution.h"

#include <vector>

// Contract: LeetCode 560. Count contiguous (non-empty) subarrays whose sum
// equals k. Elements may be negative or zero.

TEST(examples_from_the_problem)
{
    CHECK_EQ(subarray_sum({1, 1, 1}, 2), 2);
    CHECK_EQ(subarray_sum({1, 2, 3}, 3), 2);
}

TEST(empty_input_has_no_subarrays)
{
    CHECK_EQ(subarray_sum({}, 0), 0);
    CHECK_EQ(subarray_sum({}, 5), 0);
}

TEST(single_element)
{
    CHECK_EQ(subarray_sum({5}, 5), 1);
    CHECK_EQ(subarray_sum({5}, 4), 0);
    CHECK_EQ(subarray_sum({0}, 0), 1);
}

TEST(no_match)
{
    CHECK_EQ(subarray_sum({1, 2, 3}, 100), 0);
}

TEST(whole_array_matches)
{
    CHECK_EQ(subarray_sum({1, 2, 3}, 6), 1);
}

TEST(negatives_and_zeros_create_multiple_matches)
{
    // [1,-1], [1,-1,0], [-1,0,... no: subarrays summing to 0: [1,-1], [1,-1,0], [0]
    CHECK_EQ(subarray_sum({1, -1, 0}, 0), 3);
}

TEST(all_zeros_counts_every_subarray)
{
    // n*(n+1)/2 subarrays, all sum 0.
    CHECK_EQ(subarray_sum({0, 0, 0, 0}, 0), 10);
}

TEST(all_negative)
{
    CHECK_EQ(subarray_sum({-1, -1, 1}, 0), 1);
    CHECK_EQ(subarray_sum({-1, -2, -3}, -3), 2);
}

TEST(negative_target)
{
    CHECK_EQ(subarray_sum({1, -2, 1, -2}, -1), 3);
}

TEST(overlapping_matches_are_all_counted)
{
    CHECK_EQ(subarray_sum({3, 4, 7, 2, -3, 1, 4, 2}, 7), 4);
}

TEST(prefix_sum_can_exceed_int_range_partial_sums)
{
    // Prefix sums reach 3e9; a 32-bit accumulator would overflow.
    CHECK_EQ(subarray_sum({1000000000, 1000000000, 1000000000}, 2000000000), 2);
}

TEST(longer_array_against_brute_force)
{
    std::vector<int> v;
    for (int i = 0; i < 60; ++i)
        v.push_back((i * 37) % 11 - 5);
    for (int k = -6; k <= 6; ++k)
    {
        int expected = 0;
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            int sum = 0;
            for (std::size_t j = i; j < v.size(); ++j)
            {
                sum += v[j];
                if (sum == k)
                    ++expected;
            }
        }
        CHECK_EQ(subarray_sum(v, k), expected);
    }
}

TEST_MAIN()
