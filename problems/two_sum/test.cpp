#include "minitest.h"
#include "solution.h"

TEST(finds_pair)
{
    auto result = twoSum({2, 7, 11, 15}, 9);
    CHECK_EQ(result.size(), 2u);
    CHECK(result[0] == 0 && result[1] == 1);
}

TEST(no_pair_returns_empty)
{
    auto result = twoSum({1, 2, 3}, 100);
    CHECK(result.empty());
}

TEST(handles_duplicates)
{
    auto result = twoSum({3, 3}, 6);
    CHECK_EQ(result.size(), 2u);
    CHECK(result[0] == 0 && result[1] == 1);
}

TEST_MAIN()
