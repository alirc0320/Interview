#include "minitest.h"
#include "solution.h"

#include <algorithm>
#include <vector>

// Contract: LeetCode 378. Matrix rows and columns are each sorted ascending.
// Return the k-th smallest element (1-indexed) counting duplicates.

using M = std::vector<std::vector<int>>;

TEST(examples_from_the_problem)
{
    CHECK_EQ(kth_smallest(M{{1, 5, 9}, {10, 11, 13}, {12, 13, 15}}, 8), 13);
    CHECK_EQ(kth_smallest(M{{-5}}, 1), -5);
}

TEST(k_one_is_top_left)
{
    CHECK_EQ(kth_smallest(M{{1, 2}, {3, 4}}, 1), 1);
}

TEST(k_n_squared_is_bottom_right)
{
    CHECK_EQ(kth_smallest(M{{1, 2}, {3, 4}}, 4), 4);
    CHECK_EQ(kth_smallest(M{{1, 5, 9}, {10, 11, 13}, {12, 13, 15}}, 9), 15);
}

TEST(every_k_in_a_small_matrix)
{
    M m{{1, 3, 5}, {6, 7, 12}, {11, 14, 14}};
    std::vector<int> flat = {1, 3, 5, 6, 7, 12, 11, 14, 14};
    std::sort(flat.begin(), flat.end());
    for (int k = 1; k <= 9; ++k)
        CHECK_EQ(kth_smallest(m, k), flat[static_cast<std::size_t>(k - 1)]);
}

TEST(duplicates_count_separately)
{
    CHECK_EQ(kth_smallest(M{{1, 1}, {1, 1}}, 3), 1);
    CHECK_EQ(kth_smallest(M{{1, 2}, {1, 3}}, 2), 1);
    CHECK_EQ(kth_smallest(M{{1, 2}, {1, 3}}, 3), 2);
}

TEST(single_row_and_single_column_are_square_only_but_one_by_one_works)
{
    CHECK_EQ(kth_smallest(M{{7}}, 1), 7);
}

TEST(negative_values)
{
    CHECK_EQ(kth_smallest(M{{-10, -5}, {-7, 0}}, 2), -7);
    CHECK_EQ(kth_smallest(M{{-10, -5}, {-7, 0}}, 3), -5);
}

TEST(extreme_values)
{
    const int lo = -2147483647 - 1;
    const int hi = 2147483647;
    CHECK_EQ(kth_smallest(M{{lo, 0}, {0, hi}}, 1), lo);
    CHECK_EQ(kth_smallest(M{{lo, 0}, {0, hi}}, 4), hi);
    CHECK_EQ(kth_smallest(M{{lo, 0}, {0, hi}}, 2), 0);
}

TEST(larger_matrix_against_flatten_and_sort)
{
    const int n = 40;
    M m(static_cast<std::size_t>(n), std::vector<int>(static_cast<std::size_t>(n)));
    std::vector<int> flat;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int v = i * 3 + j * 5 + (i * j) % 7; // non-decreasing along rows and columns
            m[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] = v;
            flat.push_back(v);
        }
    }
    std::sort(flat.begin(), flat.end());
    for (int k : {1, 2, 17, 400, 799, 800, 1234, 1599, 1600})
        CHECK_EQ(kth_smallest(m, k), flat[static_cast<std::size_t>(k - 1)]);
}

TEST_MAIN()
