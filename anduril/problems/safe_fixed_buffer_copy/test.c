#include "minitest.h"
#include "solution.h"

#include <string.h>

TEST(copy_fits_exactly)
{
    char dst[6];
    CHECK(safe_strcpy(dst, sizeof(dst), "hello")); /* "hello\0" is 6 bytes */
    CHECK_EQ(strcmp(dst, "hello"), 0);
}

TEST(copy_truncates_when_too_long)
{
    char dst[4];
    CHECK(!safe_strcpy(dst, sizeof(dst), "hello")); /* only room for "hel\0" */
    CHECK_EQ(strcmp(dst, "hel"), 0);
    CHECK_EQ(dst[3], '\0');
}

TEST(copy_with_zero_capacity_writes_nothing)
{
    char dst[4] = {'X', 'X', 'X', 'X'};
    CHECK(!safe_strcpy(dst, 0, "hi"));
    CHECK_EQ(dst[0], 'X'); /* untouched — writing dst[0] would be OOB */
}

TEST(copy_with_capacity_one_produces_empty_string)
{
    char dst[1];
    CHECK(!safe_strcpy(dst, sizeof(dst), "hi"));
    CHECK_EQ(dst[0], '\0');
}

TEST(copy_empty_source)
{
    char dst[4] = {'X', 'X', 'X', 'X'};
    CHECK(safe_strcpy(dst, sizeof(dst), ""));
    CHECK_EQ(dst[0], '\0');
}

TEST(cat_appends_when_it_fits)
{
    char dst[12] = "foo";
    CHECK(safe_strcat(dst, sizeof(dst), "bar"));
    CHECK_EQ(strcmp(dst, "foobar"), 0);
}

TEST(cat_truncates_when_it_does_not_fit)
{
    char dst[7] = "foo"; /* 3 used, 3 free + NUL before overflow */
    CHECK(!safe_strcat(dst, sizeof(dst), "barbaz"));
    CHECK_EQ(strcmp(dst, "foobar"), 0); /* "foo" + "bar", "baz" dropped */
    CHECK_EQ(dst[6], '\0');
}

TEST(cat_onto_already_full_buffer_appends_nothing)
{
    char dst[4] = "abc"; /* completely full: 3 chars + NUL == capacity */
    CHECK(!safe_strcat(dst, sizeof(dst), "xyz"));
    CHECK_EQ(strcmp(dst, "abc"), 0);
}

int main(void)
{
    RUN_TEST(copy_fits_exactly);
    RUN_TEST(copy_truncates_when_too_long);
    RUN_TEST(copy_with_zero_capacity_writes_nothing);
    RUN_TEST(copy_with_capacity_one_produces_empty_string);
    RUN_TEST(copy_empty_source);
    RUN_TEST(cat_appends_when_it_fits);
    RUN_TEST(cat_truncates_when_it_does_not_fit);
    RUN_TEST(cat_onto_already_full_buffer_appends_nothing);
    TEST_SUMMARY();
    return TEST_EXIT_CODE();
}
