#include "minitest.h"
#include "solution.h"

TEST(set_bit)
{
    uint32_t reg = 0;
    reg_set_bit(&reg, 0);
    CHECK_EQ(reg, 0x1u);
    reg_set_bit(&reg, 3);
    CHECK_EQ(reg, 0x9u);
    reg_set_bit(&reg, 31);
    CHECK_EQ(reg, 0x80000009u);
    reg_set_bit(&reg, 3); /* setting an already-set bit is a no-op */
    CHECK_EQ(reg, 0x80000009u);
}

TEST(clear_bit)
{
    uint32_t reg = 0xFFFFFFFFu;
    reg_clear_bit(&reg, 0);
    CHECK_EQ(reg, 0xFFFFFFFEu);
    reg_clear_bit(&reg, 31);
    CHECK_EQ(reg, 0x7FFFFFFEu);
    reg_clear_bit(&reg, 0); /* clearing an already-clear bit is a no-op */
    CHECK_EQ(reg, 0x7FFFFFFEu);
}

TEST(toggle_bit)
{
    uint32_t reg = 0;
    reg_toggle_bit(&reg, 5);
    CHECK_EQ(reg, 0x20u);
    reg_toggle_bit(&reg, 5);
    CHECK_EQ(reg, 0x0u);
}

TEST(test_bit)
{
    uint32_t reg = 0x00000005u; /* bits 0 and 2 set */
    CHECK_EQ(reg_test_bit(&reg, 0), 1);
    CHECK_EQ(reg_test_bit(&reg, 1), 0);
    CHECK_EQ(reg_test_bit(&reg, 2), 1);
    CHECK_EQ(reg_test_bit(&reg, 31), 0);
}

TEST(write_masked_replaces_only_masked_bits)
{
    uint32_t reg = 0xF0F0F0F0u;
    /* Replace the low byte's field bits [4:7] (mask 0xF0) with 0x3 shifted
     * into place, leaving every other bit untouched. */
    reg_write_masked(&reg, 0x000000F0u, 0x00000030u);
    CHECK_EQ(reg, 0xF0F0F030u);
}

TEST(write_masked_with_all_ones_mask_is_a_full_replace)
{
    uint32_t reg = 0x12345678u;
    reg_write_masked(&reg, 0xFFFFFFFFu, 0xDEADBEEFu);
    CHECK_EQ(reg, 0xDEADBEEFu);
}

TEST(write_masked_ignores_value_bits_outside_the_mask)
{
    uint32_t reg = 0x00000000u;
    /* value has bits set outside mask; those must NOT leak into reg. */
    reg_write_masked(&reg, 0x0000000Fu, 0xFFFFFFFFu);
    CHECK_EQ(reg, 0x0000000Fu);
}

int main(void)
{
    RUN_TEST(set_bit);
    RUN_TEST(clear_bit);
    RUN_TEST(toggle_bit);
    RUN_TEST(test_bit);
    RUN_TEST(write_masked_replaces_only_masked_bits);
    RUN_TEST(write_masked_with_all_ones_mask_is_a_full_replace);
    RUN_TEST(write_masked_ignores_value_bits_outside_the_mask);
    TEST_SUMMARY();
    return TEST_EXIT_CODE();
}
