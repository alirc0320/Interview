#include "minitest.h"
#include "solution.h"

#include <cstdint>
#include <vector>

// Contract: LeetCode 2461. Among all length-k subarrays whose elements are
// all distinct, return the maximum sum; 0 if none exists (including k > n).

TEST(examples_from_the_problem)
{
    CHECK_EQ(max_sum_distinct_subarray({1, 5, 4, 2, 9, 9, 9}, 3), 15);
    CHECK_EQ(max_sum_distinct_subarray({4, 4, 4}, 3), 0);
}

TEST(k_larger_than_array_returns_zero)
{
    CHECK_EQ(max_sum_distinct_subarray({1, 2, 3}, 4), 0);
    CHECK_EQ(max_sum_distinct_subarray({}, 1), 0);
}

TEST(k_equals_array_length)
{
    CHECK_EQ(max_sum_distinct_subarray({1, 2, 3}, 3), 6);
    CHECK_EQ(max_sum_distinct_subarray({1, 2, 2}, 3), 0);
}

TEST(k_equals_one_is_the_max_element)
{
    CHECK_EQ(max_sum_distinct_subarray({3, 9, 2, 9, 1}, 1), 9);
}

TEST(window_with_duplicates_is_skipped_but_later_windows_count)
{
    // [5,5,1] and [5,1,1] invalid; [1,1,7] invalid; [1,7,8] valid.
    CHECK_EQ(max_sum_distinct_subarray({5, 5, 1, 1, 7, 8}, 3), 16);
}

TEST(duplicate_at_the_window_edges)
{
    CHECK_EQ(max_sum_distinct_subarray({1, 2, 3, 1}, 3), 6);
    CHECK_EQ(max_sum_distinct_subarray({1, 2, 3, 1}, 4), 0);
}

TEST(all_distinct_picks_largest_window)
{
    CHECK_EQ(max_sum_distinct_subarray({1, 2, 3, 4, 5, 6}, 2), 11);
    CHECK_EQ(max_sum_distinct_subarray({6, 5, 4, 3, 2, 1}, 4), 18);
}

TEST(duplicate_value_leaves_window_then_reenters)
{
    // Windows of 3: [1,2,1] bad, [2,1,2] bad, [1,2,3] ok, [2,3,1] ok.
    CHECK_EQ(max_sum_distinct_subarray({1, 2, 1, 2, 3, 1}, 3), 6);
}

TEST(sum_exceeds_32_bits)
{
    std::int64_t expected = 2000000000LL + 1999999999LL + 1999999998LL;
    CHECK(max_sum_distinct_subarray({2000000000, 1999999999, 1999999998}, 3) == expected);
}

TEST(against_brute_force)
{
    std::vector<int> v;
    for (int i = 0; i < 80; ++i)
        v.push_back((i * 17 + 3) % 9);
    for (int k = 1; k <= 9; ++k)
    {
        std::int64_t best = 0;
        for (std::size_t i = 0; i + static_cast<std::size_t>(k) <= v.size(); ++i)
        {
            bool seen[9] = {};
            bool ok = true;
            std::int64_t sum = 0;
            for (std::size_t j = i; j < i + static_cast<std::size_t>(k); ++j)
            {
                if (seen[v[j]])
                    ok = false;
                seen[v[j]] = true;
                sum += v[j];
            }
            if (ok && sum > best)
                best = sum;
        }
        CHECK(max_sum_distinct_subarray(v, k) == best);
    }
}

TEST_MAIN()
