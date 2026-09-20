#include "minitest.h"
#include "solution.h"

#include <cstdint>
#include <limits>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - elapsed = now - start, computed modulo 2^32.
//   - expired() is true once elapsed >= timeout (the boundary itself
//     counts as expired). timeout == 0 is therefore expired immediately.
//   - timeout is assumed to be < 2^31 ("less than half the counter
//     range"); `now` is never earlier than `start` in real time.
// ---------------------------------------------------------------------

namespace
{
constexpr std::uint32_t kMax = std::numeric_limits<std::uint32_t>::max();
}

TEST(not_expired_before_timeout)
{
    CHECK(!expired(100, 150, 100));
    CHECK(!expired(0, 99, 100));
}

TEST(expired_after_timeout)
{
    CHECK(expired(100, 300, 100));
    CHECK(expired(0, 101, 100));
}

TEST(boundary_counts_as_expired)
{
    CHECK(expired(100, 200, 100));
    CHECK(!expired(100, 199, 100));
}

TEST(zero_elapsed_is_not_expired_for_positive_timeout)
{
    CHECK(!expired(500, 500, 1));
    CHECK(!expired(0, 0, 10));
}

TEST(zero_timeout_expires_immediately)
{
    CHECK(expired(500, 500, 0));
    CHECK(expired(500, 501, 0));
}

TEST(wraparound_not_yet_expired)
{
    // start is 16 ticks before the wrap, now is 16 ticks after: elapsed = 32.
    CHECK(!expired(0xFFFFFFF0u, 0x00000010u, 33));
    CHECK(!expired(0xFFFFFFF0u, 0x00000010u, 100));
}

TEST(wraparound_exactly_at_boundary)
{
    CHECK(expired(0xFFFFFFF0u, 0x00000010u, 32));
}

TEST(wraparound_expired)
{
    CHECK(expired(0xFFFFFFF0u, 0x00000010u, 31));
    CHECK(expired(0xFFFFFFF0u, 0x00000010u, 1));
    CHECK(expired(0xFFFFFFF0u, 0x00000064u, 50));
}

TEST(start_at_max_then_counter_wraps_to_zero)
{
    CHECK(!expired(kMax, 0, 2)); // elapsed 1
    CHECK(expired(kMax, 0, 1));  // elapsed 1, boundary
    CHECK(expired(kMax, 5, 3));  // elapsed 6
}

TEST(wrapped_now_is_smaller_than_start_but_not_expired)
{
    // A naive `now - start > timeout` in signed 64-bit, or `now > start + timeout`
    // computed without wrap handling, gets this wrong.
    std::uint32_t start = 0xFFFFFF00u;
    std::uint32_t now = 0x00000020u; // elapsed = 0x120 = 288
    CHECK(!expired(start, now, 1000));
    CHECK(expired(start, now, 200));
}

TEST(naive_start_plus_timeout_would_overflow)
{
    // start + timeout wraps to a small number; a naive `now >= start + timeout`
    // would report "expired" immediately.
    std::uint32_t start = 0xFFFFFFF0u;
    CHECK(!expired(start, start, 0x100));
    CHECK(!expired(start, start + 10, 0x100));
    CHECK(expired(start, start + 0x100, 0x100));
}

TEST(large_timeout_near_half_range)
{
    const std::uint32_t timeout = 0x7FFFFFFFu;
    CHECK(!expired(0, 0x7FFFFFFEu, timeout));
    CHECK(expired(0, 0x7FFFFFFFu, timeout));
    CHECK(!expired(0x80000000u, 0xFFFFFFFEu, timeout));
    CHECK(expired(0x80000000u, 0xFFFFFFFFu, timeout));
    CHECK(!expired(0xC0000000u, 0x3FFFFFFEu, timeout)); // spans the wrap
    CHECK(expired(0xC0000000u, 0x3FFFFFFFu, timeout));
}

TEST(result_is_consistent_for_every_start_across_the_wrap)
{
    const std::uint32_t timeout = 50;
    for (std::uint32_t offset = 0; offset < 200; ++offset)
    {
        std::uint32_t start = kMax - 99;    // 100 ticks before the wrap
        std::uint32_t now = start + offset; // wraps naturally
        CHECK_EQ(expired(start, now, timeout), offset >= timeout);
    }
}

TEST_MAIN()
