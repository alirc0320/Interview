#include "minitest.h"
#include "solution.h"

// Contract: LeetCode 362. hit(t) records a hit at second t; get_hits(t)
// returns the hits in the past 300 seconds, i.e. timestamps in
// (t - 300, t]. Calls come in non-decreasing timestamp order; several hits
// may share a timestamp.

TEST(leetcode_example)
{
    HitCounter c;
    c.hit(1);
    c.hit(2);
    c.hit(3);
    CHECK_EQ(c.get_hits(4), 3);
    c.hit(300);
    CHECK_EQ(c.get_hits(300), 4);
    CHECK_EQ(c.get_hits(301), 3);
}

TEST(no_hits)
{
    HitCounter c;
    CHECK_EQ(c.get_hits(1), 0);
    CHECK_EQ(c.get_hits(1000), 0);
}

TEST(multiple_hits_at_the_same_timestamp)
{
    HitCounter c;
    c.hit(5);
    c.hit(5);
    c.hit(5);
    CHECK_EQ(c.get_hits(5), 3);
    CHECK_EQ(c.get_hits(6), 3);
}

TEST(window_boundary_is_299_seconds_back_inclusive)
{
    HitCounter c;
    c.hit(1);
    CHECK_EQ(c.get_hits(300), 1); // 300 - 1 = 299 -> still inside
    CHECK_EQ(c.get_hits(301), 0); // 300 seconds old -> expired
}

TEST(old_hits_expire_but_recent_ones_remain)
{
    HitCounter c;
    c.hit(1);
    c.hit(100);
    c.hit(200);
    CHECK_EQ(c.get_hits(250), 3);
    CHECK_EQ(c.get_hits(301), 2);
    CHECK_EQ(c.get_hits(400), 1);
    CHECK_EQ(c.get_hits(500), 0);
}

TEST(query_does_not_consume_hits)
{
    HitCounter c;
    c.hit(10);
    CHECK_EQ(c.get_hits(20), 1);
    CHECK_EQ(c.get_hits(20), 1);
    CHECK_EQ(c.get_hits(21), 1);
}

TEST(hits_after_a_long_quiet_period)
{
    HitCounter c;
    c.hit(1);
    CHECK_EQ(c.get_hits(10000), 0);
    c.hit(10000);
    CHECK_EQ(c.get_hits(10000), 1);
}

TEST(steady_stream_keeps_a_rolling_window)
{
    HitCounter c;
    for (int t = 1; t <= 1000; ++t)
    {
        c.hit(t);
        int expected = t < 300 ? t : 300;
        CHECK_EQ(c.get_hits(t), expected);
    }
}

TEST(heavy_same_timestamp_load)
{
    HitCounter c;
    for (int i = 0; i < 100000; ++i)
        c.hit(50);
    CHECK_EQ(c.get_hits(60), 100000);
    CHECK_EQ(c.get_hits(349), 100000);
    CHECK_EQ(c.get_hits(350), 0);
}

TEST_MAIN()
