#include "minitest.h"
#include "solution.h"

TEST(swap16_known_value)
{
    CHECK_EQ(swap16(0x1234u), 0x3412u);
    CHECK_EQ(swap16(0x00FFu), 0xFF00u);
    CHECK_EQ(swap16(0x0000u), 0x0000u);
}

TEST(swap32_known_value)
{
    CHECK_EQ(swap32(0x12345678u), 0x78563412u);
    // CHECK_EQ(swap32(0x000000FFu), 0xFF000000u);
    // CHECK_EQ(swap32(0xDEADBEEFu), 0xEFBEADDEu);
}

TEST(swap_is_an_involution)
{
    CHECK_EQ(swap16(swap16(0xABCDu)), 0xABCDu);
    CHECK_EQ(swap32(swap32(0xCAFEF00Du)), 0xCAFEF00Du);
}

/* This test doesn't assume the build machine's endianness — it checks
 * host_to_be32/be32_to_host are internally consistent with whatever
 * host_is_little_endian() itself reports. That's deliberate: an embedded
 * codebase has to work correctly on both little- and big-endian targets
 * without the test suite hardcoding which one CI happens to run on. */
TEST(host_to_be32_matches_detected_endianness)
{
    uint32_t value = 0x12345678u;
    uint32_t be = host_to_be32(value);

    if (host_is_little_endian())
    {
        CHECK_EQ(be, swap32(value));
    }
    else
    {
        CHECK_EQ(be, value);
    }
}

TEST(be32_to_host_round_trips)
{
    uint32_t values[] = {0x00000000u, 0xFFFFFFFFu, 0x12345678u, 0x80000001u};
    for (unsigned i = 0; i < 4; ++i)
    {
        uint32_t be = host_to_be32(values[i]);
        CHECK_EQ(be32_to_host(be), values[i]);
    }
}

int main(void)
{
    RUN_TEST(swap16_known_value);
    RUN_TEST(swap32_known_value);
    RUN_TEST(swap_is_an_involution);
    RUN_TEST(host_to_be32_matches_detected_endianness);
    RUN_TEST(be32_to_host_round_trips);
    TEST_SUMMARY();
    return TEST_EXIT_CODE();
}
