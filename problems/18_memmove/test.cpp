#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

// ---------------------------------------------------------------------
// my_memmove must match std::memmove: correct for overlapping and
// non-overlapping ranges, returns `dst`, and n == 0 is a no-op.
//
// Overlap arrangements, with S = src range and D = dst range:
//   dst < src, overlapping  ->  a forward copy is safe
//   dst > src, overlapping  ->  a backward copy is required
// Both are covered explicitly and then exhaustively against std::memmove.
// ---------------------------------------------------------------------

namespace
{
std::vector<std::uint8_t> sequence(std::size_t n)
{
    std::vector<std::uint8_t> v(n);
    for (std::size_t i = 0; i < n; ++i)
        v[i] = static_cast<std::uint8_t>(i + 1);
    return v;
}
} // namespace

TEST(non_overlapping_copy)
{
    std::uint8_t src[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::uint8_t dst[8] = {};
    my_memmove(dst, src, sizeof(src));
    CHECK(std::memcmp(dst, src, sizeof(src)) == 0);
}

TEST(returns_dst_pointer)
{
    std::uint8_t src[4] = {1, 2, 3, 4};
    std::uint8_t dst[4] = {};
    CHECK(my_memmove(dst, src, 4) == dst);
    CHECK(my_memmove(dst, src, 0) == dst);
}

TEST(zero_length_leaves_dst_untouched)
{
    std::uint8_t src[4] = {1, 2, 3, 4};
    std::uint8_t dst[4] = {9, 9, 9, 9};
    my_memmove(dst, src, 0);
    CHECK(dst[0] == 9 && dst[1] == 9 && dst[2] == 9 && dst[3] == 9);
}

TEST(source_is_not_modified)
{
    std::uint8_t src[6] = {10, 20, 30, 40, 50, 60};
    std::uint8_t dst[6] = {};
    my_memmove(dst, src, sizeof(src));
    const std::uint8_t expected[6] = {10, 20, 30, 40, 50, 60};
    CHECK(std::memcmp(src, expected, sizeof(src)) == 0);
}

TEST(copy_only_n_bytes)
{
    std::uint8_t src[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::uint8_t dst[8] = {};
    my_memmove(dst, src, 3);
    const std::uint8_t expected[8] = {1, 2, 3, 0, 0, 0, 0, 0};
    CHECK(std::memcmp(dst, expected, sizeof(dst)) == 0);
}

TEST(same_pointer_is_a_noop)
{
    auto buf = sequence(16);
    auto orig = buf;
    my_memmove(buf.data(), buf.data(), buf.size());
    CHECK(buf == orig);
}

TEST(overlap_dst_before_src_forward_copy)
{
    // Shift left by 2: [1 2 3 4 5 6 7 8] -> [3 4 5 6 7 8 7 8]
    std::uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    my_memmove(buf, buf + 2, 6);
    const std::uint8_t expected[8] = {3, 4, 5, 6, 7, 8, 7, 8};
    CHECK(std::memcmp(buf, expected, sizeof(buf)) == 0);
}

TEST(overlap_dst_after_src_backward_copy)
{
    // Shift right by 2: [1 2 3 4 5 6 7 8] -> [1 2 1 2 3 4 5 6]
    // A naive forward copy would smear [1 2] across the whole buffer.
    std::uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    my_memmove(buf + 2, buf, 6);
    const std::uint8_t expected[8] = {1, 2, 1, 2, 3, 4, 5, 6};
    CHECK(std::memcmp(buf, expected, sizeof(buf)) == 0);
}

TEST(overlap_by_a_single_byte_each_direction)
{
    std::uint8_t a[5] = {1, 2, 3, 4, 5};
    my_memmove(a, a + 1, 4);
    const std::uint8_t ea[5] = {2, 3, 4, 5, 5};
    CHECK(std::memcmp(a, ea, sizeof(a)) == 0);

    std::uint8_t b[5] = {1, 2, 3, 4, 5};
    my_memmove(b + 1, b, 4);
    const std::uint8_t eb[5] = {1, 1, 2, 3, 4};
    CHECK(std::memcmp(b, eb, sizeof(b)) == 0);
}

TEST(adjacent_ranges_do_not_overlap)
{
    std::uint8_t buf[8] = {1, 2, 3, 4, 0, 0, 0, 0};
    my_memmove(buf + 4, buf, 4);
    const std::uint8_t expected[8] = {1, 2, 3, 4, 1, 2, 3, 4};
    CHECK(std::memcmp(buf, expected, sizeof(buf)) == 0);
}

TEST(dst_range_fully_inside_src_range)
{
    auto buf = sequence(10);
    auto ref = buf;
    my_memmove(buf.data() + 2, buf.data(), 6); // dst [2,8) inside/overlapping src [0,6)
    std::memmove(ref.data() + 2, ref.data(), 6);
    CHECK(buf == ref);
}

TEST(does_not_write_outside_the_destination_range)
{
    // Guard bytes on both sides must survive.
    std::uint8_t buf[12];
    std::memset(buf, 0xAA, sizeof(buf));
    std::uint8_t src[4] = {1, 2, 3, 4};
    my_memmove(buf + 4, src, 4);
    for (std::size_t i = 0; i < 4; ++i)
        CHECK(buf[i] == 0xAA);
    for (std::size_t i = 8; i < 12; ++i)
        CHECK(buf[i] == 0xAA);
    CHECK(buf[4] == 1 && buf[7] == 4);
}

TEST(handles_all_byte_values)
{
    std::uint8_t src[256];
    std::uint8_t dst[256] = {};
    for (int i = 0; i < 256; ++i)
        src[i] = static_cast<std::uint8_t>(i);
    my_memmove(dst, src, sizeof(src));
    CHECK(std::memcmp(dst, src, sizeof(src)) == 0);
}

TEST(large_non_overlapping_copy)
{
    auto src = sequence(100000);
    std::vector<std::uint8_t> dst(100000, 0);
    my_memmove(dst.data(), src.data(), src.size());
    CHECK(dst == src);
}

TEST(large_overlapping_copy_both_directions)
{
    auto a = sequence(50000);
    auto ref = a;
    my_memmove(a.data() + 1234, a.data(), 40000);
    std::memmove(ref.data() + 1234, ref.data(), 40000);
    CHECK(a == ref);

    auto b = sequence(50000);
    auto refb = b;
    my_memmove(b.data(), b.data() + 4321, 40000);
    std::memmove(refb.data(), refb.data() + 4321, 40000);
    CHECK(b == refb);
}

TEST(matches_std_memmove_for_every_offset_pair_and_length)
{
    constexpr std::size_t N = 24;
    for (std::size_t s = 0; s < N; ++s)
    {
        for (std::size_t d = 0; d < N; ++d)
        {
            for (std::size_t len = 0; len + s <= N && len + d <= N; ++len)
            {
                auto mine = sequence(N);
                auto ref = sequence(N);
                void* r = my_memmove(mine.data() + d, mine.data() + s, len);
                std::memmove(ref.data() + d, ref.data() + s, len);
                CHECK(r == mine.data() + d);
                CHECK(mine == ref);
            }
        }
    }
}

TEST_MAIN()
