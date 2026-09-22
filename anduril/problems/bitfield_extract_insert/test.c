#include "minitest.h"
#include "solution.h"

#include <limits.h>
#include <stdint.h>

static const uint32_t kAll = 0xFFFFFFFFu;

TEST(extract_low_nibble)
{
    uint32_t out = 0;
    CHECK(extract_bits(0xABCD1234u, 0, 4, &out));
    CHECK_EQ(out, 0x4u);
}

TEST(extract_middle_field)
{
    uint32_t out = 0;
    CHECK(extract_bits(0xABCD1234u, 8, 8, &out));
    CHECK_EQ(out, 0x12u);
    CHECK(extract_bits(0xABCD1234u, 12, 12, &out));
    CHECK_EQ(out, 0xCD1u);
}

TEST(extract_top_bits)
{
    uint32_t out = 0;
    CHECK(extract_bits(0xABCD1234u, 28, 4, &out));
    CHECK_EQ(out, 0xAu);
    CHECK(extract_bits(0x80000000u, 31, 1, &out));
    CHECK_EQ(out, 1u);
}

TEST(extract_full_width_is_the_whole_value)
{
    /* count == 32 would be `1u << 32` (UB) in a naive mask computation. */
    uint32_t out = 0;
    CHECK(extract_bits(0xDEADBEEFu, 0, 32, &out));
    CHECK_EQ(out, 0xDEADBEEFu);
    CHECK(extract_bits(kAll, 0, 32, &out));
    CHECK_EQ(out, kAll);
}

TEST(extract_zero_count_returns_zero)
{
    uint32_t out = 123;
    CHECK(extract_bits(kAll, 0, 0, &out));
    CHECK_EQ(out, 0u);
    CHECK(extract_bits(kAll, 16, 0, &out));
    CHECK_EQ(out, 0u);
}

TEST(extract_result_is_masked_above_the_field)
{
    uint32_t out = 0;
    CHECK(extract_bits(kAll, 4, 3, &out));
    CHECK_EQ(out, 0x7u);
    CHECK(extract_bits(kAll, 0, 31, &out));
    CHECK_EQ(out, 0x7FFFFFFFu);
}

TEST(extract_rejects_out_of_range)
{
    uint32_t out = 999;
    CHECK(!extract_bits(kAll, 0, 33, &out));
    CHECK(!extract_bits(kAll, 1, 32, &out));
    CHECK(!extract_bits(kAll, 31, 2, &out));
    CHECK(!extract_bits(kAll, 32, 1, &out));
    CHECK(!extract_bits(kAll, 40, 4, &out));
    CHECK_EQ(out, 999u); /* untouched on failure */
}

TEST(extract_rejects_start_plus_count_wraparound)
{
    uint32_t out = 0;
    unsigned huge = UINT_MAX;
    CHECK(!extract_bits(kAll, huge, 2, &out));
    CHECK(!extract_bits(kAll, 2, huge, &out));
}

TEST(insert_into_low_bits)
{
    uint32_t out = 0;
    CHECK(insert_bits(0xFFFFFFFFu, 0x0u, 0, 4, &out));
    CHECK_EQ(out, 0xFFFFFFF0u);
    CHECK(insert_bits(0x0u, 0xAu, 0, 4, &out));
    CHECK_EQ(out, 0xAu);
}

TEST(insert_into_middle_preserves_neighbours)
{
    uint32_t out = 0;
    CHECK(insert_bits(0xFFFFFFFFu, 0x00u, 8, 8, &out));
    CHECK_EQ(out, 0xFFFF00FFu);
    CHECK(insert_bits(0x00000000u, 0xABu, 8, 8, &out));
    CHECK_EQ(out, 0x0000AB00u);
    CHECK(insert_bits(0x12345678u, 0xFu, 12, 4, &out));
    CHECK_EQ(out, 0x1234F678u);
}

TEST(insert_replaces_previous_field_contents)
{
    uint32_t out = 0;
    CHECK(insert_bits(0xFFFFFFFFu, 0x5u, 4, 4, &out));
    CHECK_EQ(out, 0xFFFFFF5Fu);
    CHECK(insert_bits(0x000000F0u, 0x3u, 4, 4, &out));
    CHECK_EQ(out, 0x00000030u);
}

TEST(insert_full_width_replaces_everything)
{
    uint32_t out = 0;
    CHECK(insert_bits(0x12345678u, 0xDEADBEEFu, 0, 32, &out));
    CHECK_EQ(out, 0xDEADBEEFu);
}

TEST(insert_zero_count_is_identity_unless_field_nonzero)
{
    uint32_t out = 0;
    CHECK(insert_bits(0x12345678u, 0u, 0, 0, &out));
    CHECK_EQ(out, 0x12345678u);
    CHECK(!insert_bits(0x12345678u, 1u, 0, 0, &out));
}

TEST(insert_rejects_field_wider_than_count)
{
    uint32_t out = 0;
    CHECK(!insert_bits(0u, 0x10u, 0, 4, &out)); /* 0x10 needs 5 bits */
    CHECK(!insert_bits(0u, 2u, 0, 1, &out));
    CHECK(!insert_bits(0u, kAll, 8, 8, &out));
}

TEST(insert_rejects_out_of_range)
{
    uint32_t out = 0;
    CHECK(!insert_bits(0u, 0u, 0, 33, &out));
    CHECK(!insert_bits(0u, 0u, 1, 32, &out));
    CHECK(!insert_bits(0u, 0u, 31, 2, &out));
    CHECK(!insert_bits(0u, 0u, 32, 1, &out));
}

TEST(insert_then_extract_round_trips)
{
    for (unsigned start = 0; start < 32; ++start)
    {
        for (unsigned count = 1; start + count <= 32; ++count)
        {
            uint32_t max_field = (count == 32) ? kAll : ((1u << count) - 1u);
            uint32_t field = 0xA5A5A5A5u & max_field;
            uint32_t inserted = 0;
            CHECK(insert_bits(0x0F0F0F0Fu, field, start, count, &inserted));
            uint32_t extracted = 0;
            CHECK(extract_bits(inserted, start, count, &extracted));
            CHECK_EQ(extracted, field);
        }
    }
}

int main(void)
{
    RUN_TEST(extract_low_nibble);
    RUN_TEST(extract_middle_field);
    RUN_TEST(extract_top_bits);
    RUN_TEST(extract_full_width_is_the_whole_value);
    RUN_TEST(extract_zero_count_returns_zero);
    RUN_TEST(extract_result_is_masked_above_the_field);
    RUN_TEST(extract_rejects_out_of_range);
    RUN_TEST(extract_rejects_start_plus_count_wraparound);
    RUN_TEST(insert_into_low_bits);
    RUN_TEST(insert_into_middle_preserves_neighbours);
    RUN_TEST(insert_replaces_previous_field_contents);
    RUN_TEST(insert_full_width_replaces_everything);
    RUN_TEST(insert_zero_count_is_identity_unless_field_nonzero);
    RUN_TEST(insert_rejects_field_wider_than_count);
    RUN_TEST(insert_rejects_out_of_range);
    RUN_TEST(insert_then_extract_round_trips);
    TEST_SUMMARY();
    return TEST_EXIT_CODE();
}
