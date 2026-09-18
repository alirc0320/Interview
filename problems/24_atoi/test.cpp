#include "minitest.h"
#include "solution.h"

#include <climits>
#include <string>

// Contract: LeetCode 8 "String to Integer (atoi)":
//   skip leading spaces, optional +/- sign, read digits until a non-digit,
//   no digits -> 0, clamp to [INT_MIN, INT_MAX] on overflow.

TEST(simple_numbers)
{
    CHECK_EQ(my_atoi("42"), 42);
    CHECK_EQ(my_atoi("0"), 0);
    CHECK_EQ(my_atoi("7"), 7);
    CHECK_EQ(my_atoi("1234567"), 1234567);
}

TEST(signs)
{
    CHECK_EQ(my_atoi("-42"), -42);
    CHECK_EQ(my_atoi("+42"), 42);
    CHECK_EQ(my_atoi("-0"), 0);
    CHECK_EQ(my_atoi("+0"), 0);
}

TEST(leading_whitespace_is_skipped)
{
    CHECK_EQ(my_atoi("   42"), 42);
    CHECK_EQ(my_atoi("   -42"), -42);
    CHECK_EQ(my_atoi(" +7"), 7);
}

TEST(leading_zeros)
{
    CHECK_EQ(my_atoi("000123"), 123);
    CHECK_EQ(my_atoi("-000123"), -123);
    CHECK_EQ(my_atoi("0000"), 0);
}

TEST(stops_at_first_non_digit)
{
    CHECK_EQ(my_atoi("4193 with words"), 4193);
    CHECK_EQ(my_atoi("12abc34"), 12);
    CHECK_EQ(my_atoi("3.14159"), 3);
    CHECK_EQ(my_atoi("-91283472332x"), INT_MIN);
    CHECK_EQ(my_atoi("42 7"), 42);
}

TEST(no_digits_returns_zero)
{
    CHECK_EQ(my_atoi(""), 0);
    CHECK_EQ(my_atoi("   "), 0);
    CHECK_EQ(my_atoi("words and 987"), 0);
    CHECK_EQ(my_atoi("abc"), 0);
    CHECK_EQ(my_atoi("+"), 0);
    CHECK_EQ(my_atoi("-"), 0);
}

TEST(only_one_sign_allowed)
{
    CHECK_EQ(my_atoi("+-12"), 0);
    CHECK_EQ(my_atoi("-+12"), 0);
    CHECK_EQ(my_atoi("--5"), 0);
}

TEST(whitespace_after_sign_is_not_skipped)
{
    CHECK_EQ(my_atoi("- 42"), 0);
    CHECK_EQ(my_atoi("+ 42"), 0);
}

TEST(int_limits_are_exact)
{
    CHECK_EQ(my_atoi("2147483647"), INT_MAX);
    CHECK_EQ(my_atoi("-2147483648"), INT_MIN);
    CHECK_EQ(my_atoi("2147483646"), INT_MAX - 1);
    CHECK_EQ(my_atoi("-2147483647"), INT_MIN + 1);
}

TEST(positive_overflow_clamps)
{
    CHECK_EQ(my_atoi("2147483648"), INT_MAX);
    CHECK_EQ(my_atoi("2147483650"), INT_MAX);
    CHECK_EQ(my_atoi("9999999999"), INT_MAX);
    CHECK_EQ(my_atoi("21474836470"), INT_MAX);
    CHECK_EQ(my_atoi("99999999999999999999999999"), INT_MAX);
}

TEST(negative_overflow_clamps)
{
    CHECK_EQ(my_atoi("-2147483649"), INT_MIN);
    CHECK_EQ(my_atoi("-9999999999"), INT_MIN);
    CHECK_EQ(my_atoi("-99999999999999999999999999"), INT_MIN);
}

TEST(leading_zeros_do_not_trigger_overflow)
{
    CHECK_EQ(my_atoi("00000000000000000000000042"), 42);
    CHECK_EQ(my_atoi("-0000000000000000000000002147483648"), INT_MIN);
}

TEST_MAIN()
