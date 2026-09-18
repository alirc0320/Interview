#include "minitest.h"
#include "solution.h"

#include <vector>

// Contract: LeetCode 207. prerequisites[i] = {a, b} means "to take course a
// you must first take course b". Return true iff all courses can be finished
// (the prerequisite graph has no cycle).

using P = std::vector<std::vector<int>>;

TEST(no_prerequisites)
{
    CHECK(can_finish(5, {}));
    CHECK(can_finish(1, {}));
}

TEST(zero_courses)
{
    CHECK(can_finish(0, {}));
}

TEST(single_prerequisite)
{
    CHECK(can_finish(2, P{{1, 0}}));
}

TEST(two_course_cycle)
{
    CHECK(!can_finish(2, P{{1, 0}, {0, 1}}));
}

TEST(self_prerequisite_is_a_cycle)
{
    CHECK(!can_finish(1, P{{0, 0}}));
    CHECK(!can_finish(3, P{{2, 2}}));
}

TEST(linear_chain)
{
    CHECK(can_finish(5, P{{1, 0}, {2, 1}, {3, 2}, {4, 3}}));
}

TEST(long_cycle)
{
    CHECK(!can_finish(4, P{{1, 0}, {2, 1}, {3, 2}, {0, 3}}));
}

TEST(diamond_dag)
{
    CHECK(can_finish(4, P{{1, 0}, {2, 0}, {3, 1}, {3, 2}}));
}

TEST(cycle_in_a_disconnected_component)
{
    // 0<-1 is fine; 2<->3 is a cycle even though courses 0,1 are acyclic.
    CHECK(!can_finish(4, P{{1, 0}, {3, 2}, {2, 3}}));
}

TEST(cycle_reachable_only_from_a_later_start)
{
    CHECK(!can_finish(5, P{{1, 0}, {2, 1}, {3, 4}, {4, 3}}));
}

TEST(duplicate_edges_are_fine)
{
    CHECK(can_finish(2, P{{1, 0}, {1, 0}, {1, 0}}));
}

TEST(multiple_prerequisites_for_one_course)
{
    CHECK(can_finish(4, P{{3, 0}, {3, 1}, {3, 2}}));
}

TEST(cycle_hidden_behind_a_shared_prefix)
{
    CHECK(!can_finish(5, P{{1, 0}, {2, 1}, {3, 2}, {1, 3}}));
}

TEST(long_chain_does_not_blow_the_stack)
{
    const int n = 100000;
    P prereqs;
    for (int i = 1; i < n; ++i)
        prereqs.push_back({i, i - 1});
    CHECK(can_finish(n, prereqs));
    prereqs.push_back({0, n - 1});
    CHECK(!can_finish(n, prereqs));
}

TEST_MAIN()
