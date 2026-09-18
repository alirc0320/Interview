#include "minitest.h"
#include "solution.h"

#include <cstdint>

// ---------------------------------------------------------------------
// Registers are mocked with a plain local `volatile std::uint32_t`.
//
// Assumed contract (adjust if you design it differently on purpose):
//   - Field = bits [start, start + count), bit 0 = least significant.
//   - read_field returns the field right-aligned (shifted down to bit 0).
//   - set_field writes `value` into the field, leaving every other bit
//     untouched. Bits of `value` that don't fit in `count` bits are
//     masked off rather than spilling into neighbouring bits.
//   - clear_field zeroes the field, leaving every other bit untouched.
//   - count == 0 is a no-op (read returns 0).
//   - Only valid ranges (start + count <= 32) are tested, since these
//     functions have no error channel.
// ---------------------------------------------------------------------

TEST(read_field_extracts_and_right_aligns)
{
    volatile std::uint32_t reg = 0xABCD1234u;
    CHECK_EQ(read_field(&reg, 0, 4), 0x4u);
    CHECK_EQ(read_field(&reg, 8, 8), 0x12u);
    CHECK_EQ(read_field(&reg, 16, 16), 0xABCDu);
    CHECK_EQ(read_field(&reg, 28, 4), 0xAu);
}

TEST(read_field_single_bit)
{
    volatile std::uint32_t reg = 0b1010u;
    CHECK_EQ(read_field(&reg, 0, 1), 0u);
    CHECK_EQ(read_field(&reg, 1, 1), 1u);
    CHECK_EQ(read_field(&reg, 3, 1), 1u);
}

TEST(read_field_full_width)
{
    volatile std::uint32_t reg = 0xDEADBEEFu;
    CHECK_EQ(read_field(&reg, 0, 32), 0xDEADBEEFu);
}

TEST(read_field_zero_count_is_zero)
{
    volatile std::uint32_t reg = 0xFFFFFFFFu;
    CHECK_EQ(read_field(&reg, 5, 0), 0u);
}

TEST(read_field_does_not_modify_the_register)
{
    volatile std::uint32_t reg = 0x12345678u;
    read_field(&reg, 4, 8);
    CHECK_EQ(reg, 0x12345678u);
}

TEST(set_field_writes_value_in_place)
{
    volatile std::uint32_t reg = 0;
    set_field(&reg, 8, 8, 0xABu);
    CHECK_EQ(reg, 0x0000AB00u);
}

TEST(set_field_preserves_surrounding_bits)
{
    volatile std::uint32_t reg = 0xFFFFFFFFu;
    set_field(&reg, 8, 8, 0x00u);
    CHECK_EQ(reg, 0xFFFF00FFu);

    reg = 0x12345678u;
    set_field(&reg, 12, 4, 0xFu);
    CHECK_EQ(reg, 0x1234F678u);
}

TEST(set_field_overwrites_old_field_contents)
{
    volatile std::uint32_t reg = 0xFFFFFFFFu;
    set_field(&reg, 4, 4, 0x5u);
    CHECK_EQ(reg, 0xFFFFFF5Fu);
}

TEST(set_field_masks_oversized_value)
{
    volatile std::uint32_t reg = 0;
    set_field(&reg, 4, 4, 0xFFu); // only the low 4 bits belong to the field
    CHECK_EQ(reg, 0x000000F0u);

    reg = 0x0000000Fu;
    set_field(&reg, 4, 4, 0xF3u);
    CHECK_EQ(reg, 0x0000003Fu); // bits above the field untouched, low nibble preserved
}

TEST(set_field_top_bit)
{
    volatile std::uint32_t reg = 0;
    set_field(&reg, 31, 1, 1u);
    CHECK_EQ(reg, 0x80000000u);
}

TEST(set_field_full_width)
{
    volatile std::uint32_t reg = 0x12345678u;
    set_field(&reg, 0, 32, 0xDEADBEEFu);
    CHECK_EQ(reg, 0xDEADBEEFu);
}

TEST(set_field_zero_count_is_noop)
{
    volatile std::uint32_t reg = 0x12345678u;
    set_field(&reg, 8, 0, 0xFFu);
    CHECK_EQ(reg, 0x12345678u);
}

TEST(clear_field_zeroes_only_the_field)
{
    volatile std::uint32_t reg = 0xFFFFFFFFu;
    clear_field(&reg, 8, 8);
    CHECK_EQ(reg, 0xFFFF00FFu);

    clear_field(&reg, 0, 4);
    CHECK_EQ(reg, 0xFFFF00F0u);

    clear_field(&reg, 28, 4);
    CHECK_EQ(reg, 0x0FFF00F0u);
}

TEST(clear_field_full_width)
{
    volatile std::uint32_t reg = 0xFFFFFFFFu;
    clear_field(&reg, 0, 32);
    CHECK_EQ(reg, 0u);
}

TEST(clear_field_zero_count_is_noop)
{
    volatile std::uint32_t reg = 0xFFFFFFFFu;
    clear_field(&reg, 8, 0);
    CHECK_EQ(reg, 0xFFFFFFFFu);
}

TEST(clear_already_clear_field_is_harmless)
{
    volatile std::uint32_t reg = 0xF0F0F0F0u;
    clear_field(&reg, 0, 4);
    CHECK_EQ(reg, 0xF0F0F0F0u);
}

TEST(set_then_read_round_trips_for_every_valid_range)
{
    for (unsigned start = 0; start < 32; ++start)
    {
        for (unsigned count = 1; start + count <= 32; ++count)
        {
            volatile std::uint32_t reg = 0xA5A5A5A5u;
            std::uint32_t mask = count == 32 ? 0xFFFFFFFFu : ((1u << count) - 1u);
            std::uint32_t value = 0x5A5A5A5Au & mask;
            set_field(&reg, start, count, value);
            CHECK_EQ(read_field(&reg, start, count), value);
        }
    }
}

TEST(neighbouring_fields_are_independent)
{
    volatile std::uint32_t reg = 0;
    set_field(&reg, 0, 4, 0x1u);
    set_field(&reg, 4, 4, 0x2u);
    set_field(&reg, 8, 4, 0x3u);
    CHECK_EQ(reg, 0x321u);
    clear_field(&reg, 4, 4);
    CHECK_EQ(reg, 0x301u);
    CHECK_EQ(read_field(&reg, 0, 4), 0x1u);
    CHECK_EQ(read_field(&reg, 8, 4), 0x3u);
}

TEST_MAIN()
