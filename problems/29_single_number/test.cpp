#include "minitest.h"
#include "solution.h"

#include <climits>
#include <vector>

// Contract: LeetCode 136. Every element appears twice except one; return it.

TEST(examples_from_the_problem)
{
    CHECK_EQ(single_number({2, 2, 1}), 1);
    CHECK_EQ(single_number({4, 1, 2, 1, 2}), 4);
    CHECK_EQ(single_number({1}), 1);
}

TEST(single_element_only)
{
    CHECK_EQ(single_number({0}), 0);
    CHECK_EQ(single_number({-7}), -7);
}

TEST(negative_numbers)
{
    CHECK_EQ(single_number({-1, -1, -2}), -2);
    CHECK_EQ(single_number({-3, 5, -3}), 5);
}

TEST(zero_is_the_single)
{
    CHECK_EQ(single_number({1, 0, 1}), 0);
}

TEST(zero_is_the_pair)
{
    CHECK_EQ(single_number({0, 9, 0}), 9);
}

TEST(extremes)
{
    CHECK_EQ(single_number({INT_MAX, 3, 3}), INT_MAX);
    CHECK_EQ(single_number({INT_MIN, 8, 8}), INT_MIN);
    CHECK_EQ(single_number({INT_MIN, INT_MAX, INT_MIN}), INT_MAX);
}

TEST(pairs_are_not_adjacent)
{
    CHECK_EQ(single_number({1, 2, 3, 1, 2, 3, 99, 4, 4}), 99);
}

TEST(large_input)
{
    std::vector<int> v;
    for (int i = 1; i <= 50000; ++i)
    {
        v.push_back(i);
        v.push_back(i);
    }
    v.insert(v.begin() + 12345, 987654321);
    CHECK_EQ(single_number(v), 987654321);
}

TEST_MAIN()
