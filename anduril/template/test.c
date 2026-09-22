#include "minitest.h"
#include "solution.h"

TEST(basic)
{
    CHECK_EQ(solve(1), 1);
}

int main(void)
{
    RUN_TEST(basic);
    TEST_SUMMARY();
    return TEST_EXIT_CODE();
}
