#include "minitest.h"
#include "solution.h"

#include <cstdint>
#include <limits>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - Bits are numbered from 0 = least significant. `start` is the lowest
//     bit of the field; the field occupies bits [start, start + count).
//   - start + count > 32 (including unsigned wraparound) -> std::nullopt.
//   - count == 0 is valid: extract_bits returns 0, insert_bits returns
//     `value` unchanged (when field == 0).
//   - insert_bits rejects (nullopt) a `field` that does not fit in
//     `count` bits, rather than silently truncating or corrupting
//     neighbouring bits.
// ---------------------------------------------------------------------

namespace
{
constexpr std::uint32_t kAll = std::numeric_limits<std::uint32_t>::max();

// Helpers so failures print something readable (optional isn't streamable).
bool is(const std::optional<std::uint32_t>& got, std::uint32_t want)
{
    return got.has_value() && *got == want;
}
} // namespace

// ---- extract_bits -----------------------------------------------------

TEST(extract_low_nibble)
{
    CHECK(is(extract_bits(0xABCD1234u, 0, 4), 0x4u));
}

TEST(extract_middle_field)
{
    CHECK(is(extract_bits(0xABCD1234u, 8, 8), 0x12u));
    CHECK(is(extract_bits(0xABCD1234u, 12, 12), 0xCD1u));
}

TEST(extract_top_bits)
{
    CHECK(is(extract_bits(0xABCD1234u, 28, 4), 0xAu));
    CHECK(is(extract_bits(0x80000000u, 31, 1), 1u));
}

TEST(extract_single_bits)
{
    CHECK(is(extract_bits(0b1010u, 0, 1), 0u));
    CHECK(is(extract_bits(0b1010u, 1, 1), 1u));
    CHECK(is(extract_bits(0b1010u, 2, 1), 0u));
    CHECK(is(extract_bits(0b1010u, 3, 1), 1u));
}

TEST(extract_full_width_is_the_whole_value)
{
    // count == 32 would be `1u << 32` (UB) in a naive mask computation.
    CHECK(is(extract_bits(0xDEADBEEFu, 0, 32), 0xDEADBEEFu));
    CHECK(is(extract_bits(kAll, 0, 32), kAll));
}

TEST(extract_zero_count_returns_zero)
{
    CHECK(is(extract_bits(kAll, 0, 0), 0u));
    CHECK(is(extract_bits(kAll, 16, 0), 0u));
}

TEST(extract_result_is_not_shifted_by_unrelated_high_bits)
{
    // Bits above the field must be masked off.
    CHECK(is(extract_bits(kAll, 4, 3), 0x7u));
    CHECK(is(extract_bits(kAll, 0, 31), 0x7FFFFFFFu));
}

TEST(extract_rejects_out_of_range)
{
    CHECK(!extract_bits(kAll, 0, 33));
    CHECK(!extract_bits(kAll, 1, 32));
    CHECK(!extract_bits(kAll, 31, 2));
    CHECK(!extract_bits(kAll, 32, 1));
    CHECK(!extract_bits(kAll, 40, 4));
}

TEST(extract_rejects_start_plus_count_wraparound)
{
    unsigned huge = std::numeric_limits<unsigned>::max();
    CHECK(!extract_bits(kAll, huge, 2));
    CHECK(!extract_bits(kAll, 2, huge));
}

// ---- insert_bits ------------------------------------------------------

TEST(insert_into_low_bits)
{
    CHECK(is(insert_bits(0xFFFFFFFFu, 0x0u, 0, 4), 0xFFFFFFF0u));
    CHECK(is(insert_bits(0x0u, 0xAu, 0, 4), 0xAu));
}

TEST(insert_into_middle_preserves_neighbours)
{
    CHECK(is(insert_bits(0xFFFFFFFFu, 0x00u, 8, 8), 0xFFFF00FFu));
    CHECK(is(insert_bits(0x00000000u, 0xABu, 8, 8), 0x0000AB00u));
    CHECK(is(insert_bits(0x12345678u, 0xFu, 12, 4), 0x1234F678u));
}

TEST(insert_replaces_previous_field_contents)
{
    // Old bits in the field must be overwritten, not OR-ed with.
    CHECK(is(insert_bits(0xFFFFFFFFu, 0x5u, 4, 4), 0xFFFFFF5Fu));
    CHECK(is(insert_bits(0x000000F0u, 0x3u, 4, 4), 0x00000030u));
}

TEST(insert_into_top_bits)
{
    CHECK(is(insert_bits(0x0u, 0xFu, 28, 4), 0xF0000000u));
    CHECK(is(insert_bits(0x0u, 1u, 31, 1), 0x80000000u));
    CHECK(is(insert_bits(kAll, 0u, 31, 1), 0x7FFFFFFFu));
}

TEST(insert_full_width_replaces_everything)
{
    CHECK(is(insert_bits(0x12345678u, 0xDEADBEEFu, 0, 32), 0xDEADBEEFu));
    CHECK(is(insert_bits(kAll, 0u, 0, 32), 0u));
}

TEST(insert_zero_count_is_identity)
{
    CHECK(is(insert_bits(0x12345678u, 0u, 0, 0), 0x12345678u));
    CHECK(is(insert_bits(0x12345678u, 0u, 16, 0), 0x12345678u));
}

TEST(insert_rejects_field_wider_than_count)
{
    CHECK(!insert_bits(0u, 0x10u, 0, 4)); // 0x10 needs 5 bits
    CHECK(!insert_bits(0u, 2u, 0, 1));
    CHECK(!insert_bits(0u, kAll, 8, 8));
    CHECK(!insert_bits(0u, 1u, 0, 0)); // nonzero field can't fit in zero bits
}

TEST(insert_rejects_out_of_range)
{
    CHECK(!insert_bits(0u, 0u, 0, 33));
    CHECK(!insert_bits(0u, 0u, 1, 32));
    CHECK(!insert_bits(0u, 0u, 31, 2));
    CHECK(!insert_bits(0u, 0u, 32, 1));
}

TEST(insert_rejects_start_plus_count_wraparound)
{
    unsigned huge = std::numeric_limits<unsigned>::max();
    CHECK(!insert_bits(0u, 0u, huge, 2));
    CHECK(!insert_bits(0u, 0u, 2, huge));
}

TEST(insert_then_extract_round_trips)
{
    for (unsigned start = 0; start < 32; ++start)
    {
        for (unsigned count = 1; start + count <= 32; ++count)
        {
            std::uint32_t max_field = count == 32 ? kAll : ((1u << count) - 1u);
            std::uint32_t field = 0xA5A5A5A5u & max_field;
            auto inserted = insert_bits(0x0F0F0F0Fu, field, start, count);
            CHECK(inserted.has_value());
            if (!inserted)
                continue;
            CHECK(is(extract_bits(*inserted, start, count), field));
        }
    }
}

TEST(insert_does_not_disturb_bits_outside_the_field)
{
    const std::uint32_t original = 0xC3A5F00Fu;
    for (unsigned start = 0; start < 32; ++start)
    {
        for (unsigned count = 1; start + count <= 32; ++count)
        {
            auto inserted = insert_bits(original, 0u, start, count);
            CHECK(inserted.has_value());
            if (!inserted)
                continue;
            std::uint32_t field_mask = count == 32 ? kAll : (((1u << count) - 1u) << start);
            CHECK_EQ(*inserted & ~field_mask, original & ~field_mask);
            CHECK_EQ(*inserted & field_mask, 0u);
        }
    }
}

TEST_MAIN()
