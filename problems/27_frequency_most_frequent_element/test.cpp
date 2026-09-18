#include "minitest.h"
#include "solution.h"

#include <vector>

// Contract: LeetCode 1838. In one operation you may increment any element
// by 1; at most k operations in total. Return the maximum possible
// frequency of any element afterwards.

TEST(examples_from_the_problem)
{
    CHECK_EQ(max_frequency({1, 2, 4}, 5), 3);
    CHECK_EQ(max_frequency({1, 4, 8, 13}, 5), 2);
    CHECK_EQ(max_frequency({3, 9, 6}, 2), 1);
}

TEST(k_zero_is_the_plain_mode_count)
{
    CHECK_EQ(max_frequency({1, 1, 2, 2, 2, 3}, 0), 3);
    CHECK_EQ(max_frequency({1, 2, 3}, 0), 1);
}

TEST(single_element)
{
    CHECK_EQ(max_frequency({7}, 0), 1);
    CHECK_EQ(max_frequency({7}, 100), 1);
}

TEST(all_equal)
{
    CHECK_EQ(max_frequency({5, 5, 5, 5}, 0), 4);
    CHECK_EQ(max_frequency({5, 5, 5, 5}, 10), 4);
}

TEST(input_order_does_not_matter)
{
    CHECK_EQ(max_frequency({4, 1, 2}, 5), 3);
    CHECK_EQ(max_frequency({13, 1, 8, 4}, 5), 2);
}

TEST(exact_budget_boundary)
{
    // Raising {1,2,4} to 4 costs 3+2 = 5.
    CHECK_EQ(max_frequency({1, 2, 4}, 5), 3);
    CHECK_EQ(max_frequency({1, 2, 4}, 4), 2);
}

TEST(duplicates_reduce_cost)
{
    // Raising the three 1s to 5 costs 12; the existing duplicates are free.
    CHECK_EQ(max_frequency({1, 1, 1, 5}, 11), 3);
    CHECK_EQ(max_frequency({1, 1, 1, 5}, 12), 4);
}

TEST(large_k_makes_everything_equal)
{
    CHECK_EQ(max_frequency({1, 100, 10000}, 1000000), 3);
}

TEST(budget_larger_than_int_range_of_costs)
{
    // Window sums reach ~4e9, beyond 32-bit int; must not overflow.
    // Raising the lone 1 up to 1e9 costs 999,999,999.
    CHECK_EQ(max_frequency({1, 1000000000, 1000000000, 1000000000, 1000000000}, 999999998), 4);
    CHECK_EQ(max_frequency({1, 1000000000, 1000000000, 1000000000, 1000000000}, 999999999), 5);
}

TEST(many_elements)
{
    std::vector<int> v;
    for (int i = 0; i < 1000; ++i)
        v.push_back(i % 10);
    // 100 copies each of 0..9; raising all 0..8 copies to 9 costs a lot, but k covers exactly
    // the 100 copies of 8 -> 9 plus itself: 200.
    CHECK_EQ(max_frequency(v, 100), 200);
    CHECK_EQ(max_frequency(v, 0), 100);
}

TEST_MAIN()
