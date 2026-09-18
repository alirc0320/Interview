#include "minitest.h"
#include "solution.h"

#include <cstdint>
#include <limits>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract. The problem statement mentions "prohibited regions",
// "overlap when unsupported" and a "total transfer size limit" without
// giving concrete values, so those policy knobs can't be tested here.
// What IS tested is the part every sensible policy shares:
//   - zero-length descriptors are rejected
//   - src + length or dst + length wrapping past 2^64 is rejected
//   - the total size of a batch is bounded: two descriptors of ~4 GiB
//     each (8 GiB total) are assumed to be over any reasonable limit,
//     and the total must be accumulated in a wide-enough type
//   - ordinary small, well-separated transfers are accepted
//   - one bad descriptor rejects the whole batch
// Add tests for your own prohibited-region / overlap / limit constants
// below the marked line.
// ---------------------------------------------------------------------

namespace
{
constexpr std::uint64_t kMax64 = std::numeric_limits<std::uint64_t>::max();
constexpr std::uint32_t kMax32 = std::numeric_limits<std::uint32_t>::max();

Descriptor good(std::uint64_t src, std::uint64_t dst, std::uint32_t len)
{
    return Descriptor{src, dst, len};
}
} // namespace

TEST(single_small_transfer_is_valid)
{
    CHECK(validate_descriptors({good(0x10000, 0x20000, 4096)}));
}

TEST(several_separated_transfers_are_valid)
{
    std::vector<Descriptor> d = {
        good(0x10000, 0x80000, 4096),
        good(0x20000, 0x90000, 4096),
        good(0x30000, 0xA0000, 512),
    };
    CHECK(validate_descriptors(d));
}

TEST(zero_length_is_rejected)
{
    CHECK(!validate_descriptors({good(0x1000, 0x2000, 0)}));
}

TEST(zero_length_anywhere_in_the_batch_rejects_the_batch)
{
    std::vector<Descriptor> d = {
        good(0x10000, 0x80000, 4096),
        good(0x20000, 0x90000, 0),
        good(0x30000, 0xA0000, 4096),
    };
    CHECK(!validate_descriptors(d));
}

TEST(src_plus_length_overflow_is_rejected)
{
    CHECK(!validate_descriptors({good(kMax64 - 5, 0x1000, 10)}));
    CHECK(!validate_descriptors({good(kMax64, 0x1000, 1)}));
}

TEST(dst_plus_length_overflow_is_rejected)
{
    CHECK(!validate_descriptors({good(0x1000, kMax64 - 5, 10)}));
    CHECK(!validate_descriptors({good(0x1000, kMax64, 1)}));
}

TEST(overflow_with_max_length_is_rejected)
{
    CHECK(!validate_descriptors({good(kMax64 - 100, 0x1000, kMax32)}));
    CHECK(!validate_descriptors({good(0x1000, kMax64 - 100, kMax32)}));
}

TEST(overflow_in_a_later_descriptor_rejects_the_batch)
{
    std::vector<Descriptor> d = {
        good(0x10000, 0x80000, 4096),
        good(kMax64 - 1, 0x90000, 16),
    };
    CHECK(!validate_descriptors(d));
}

TEST(total_transfer_size_over_32_bits_is_rejected)
{
    // Sum of lengths is ~8 GiB. A validator that accumulates in a uint32_t
    // would wrap to a tiny number and wrongly accept this.
    std::vector<Descriptor> d = {
        good(0x100000000ull, 0x400000000ull, kMax32),
        good(0x900000000ull, 0xC00000000ull, kMax32),
    };
    CHECK(!validate_descriptors(d));
}

TEST(many_small_descriptors_dont_wrap_the_total)
{
    // 70,000 descriptors of 65,536 bytes = ~4.4 GiB, again past 2^32 total.
    std::vector<Descriptor> d;
    d.reserve(70000);
    for (std::uint64_t i = 0; i < 70000; ++i)
        d.push_back(good(0x1000000000ull + i * 65536, 0x8000000000ull + i * 65536, 65536));
    CHECK(!validate_descriptors(d));
}

TEST(validation_is_repeatable_and_does_not_mutate_input)
{
    std::vector<Descriptor> d = {good(0x10000, 0x80000, 4096)};
    CHECK(validate_descriptors(d));
    CHECK(validate_descriptors(d));
    CHECK_EQ(d.size(), 1u);
    CHECK_EQ(d[0].length, 4096u);
}

// ---- add your own policy-specific tests below this line --------------
// e.g. TEST(rejects_prohibited_region) { ... }
//      TEST(rejects_overlapping_dst_ranges_when_unsupported) { ... }

TEST_MAIN()
