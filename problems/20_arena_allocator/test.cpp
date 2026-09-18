#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - allocate(size, alignment) returns a pointer inside the caller's
//     buffer, aligned to `alignment`, not overlapping any earlier live
//     allocation, or nullptr on failure.
//   - Failure cases: alignment == 0 or not a power of two; not enough
//     room left (including room lost to alignment padding); arithmetic
//     overflow in size/padding computations.
//   - A failed allocation consumes nothing.
//   - reset() releases everything: the arena behaves like new again.
//   - size == 0 allocations aren't tested (ambiguous).
// ---------------------------------------------------------------------

namespace
{
bool aligned(const void* p, std::size_t a)
{
    return reinterpret_cast<std::uintptr_t>(p) % a == 0;
}

bool inside(const void* p, std::size_t n, const std::uint8_t* base, std::size_t cap)
{
    auto* b = static_cast<const std::uint8_t*>(p);
    return b >= base && b + n <= base + cap;
}
} // namespace

TEST(first_allocation_starts_at_buffer_start)
{
    alignas(64) std::uint8_t buf[256];
    Arena arena(buf, sizeof(buf));
    void* p = arena.allocate(16, 1);
    CHECK(p == buf);
}

TEST(allocation_is_inside_buffer)
{
    alignas(64) std::uint8_t buf[256];
    Arena arena(buf, sizeof(buf));
    void* p = arena.allocate(100, 8);
    CHECK(p != nullptr);
    CHECK(inside(p, 100, buf, sizeof(buf)));
}

TEST(allocations_are_distinct_and_do_not_overlap)
{
    alignas(64) std::uint8_t buf[256];
    Arena arena(buf, sizeof(buf));
    auto* a = static_cast<std::uint8_t*>(arena.allocate(10, 1));
    auto* b = static_cast<std::uint8_t*>(arena.allocate(20, 1));
    auto* c = static_cast<std::uint8_t*>(arena.allocate(30, 1));
    CHECK(a && b && c);
    if (!(a && b && c))
        return;
    CHECK(b >= a + 10);
    CHECK(c >= b + 20);
    std::memset(a, 0x11, 10);
    std::memset(b, 0x22, 20);
    std::memset(c, 0x33, 30);
    CHECK(a[9] == 0x11 && a[0] == 0x11);
    CHECK(b[0] == 0x22 && b[19] == 0x22);
    CHECK(c[0] == 0x33 && c[29] == 0x33);
}

TEST(returned_pointers_honour_alignment)
{
    alignas(64) std::uint8_t buf[1024];
    Arena arena(buf, sizeof(buf));
    for (std::size_t align : {1u, 2u, 4u, 8u, 16u, 32u, 64u})
    {
        arena.allocate(3, 1); // knock the bump pointer off any natural alignment
        void* p = arena.allocate(5, align);
        CHECK(p != nullptr);
        CHECK(aligned(p, align));
    }
}

TEST(alignment_padding_is_inserted_after_an_odd_sized_allocation)
{
    alignas(64) std::uint8_t buf[256];
    Arena arena(buf, sizeof(buf));
    auto* a = static_cast<std::uint8_t*>(arena.allocate(1, 1));
    auto* b = static_cast<std::uint8_t*>(arena.allocate(8, 8));
    CHECK(a == buf);
    CHECK(b != nullptr);
    CHECK(aligned(b, 8));
    CHECK(b >= a + 1);
}

TEST(unaligned_buffer_start_is_handled)
{
    alignas(64) std::uint8_t backing[256];
    std::uint8_t* buf = backing + 1; // deliberately misaligned base
    Arena arena(buf, 200);
    void* p = arena.allocate(16, 16);
    CHECK(p != nullptr);
    CHECK(aligned(p, 16));
    CHECK(inside(p, 16, buf, 200));
}

TEST(exact_fit_succeeds_then_arena_is_full)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(64, 1) == buf);
    CHECK(arena.allocate(1, 1) == nullptr);
}

TEST(request_larger_than_arena_fails)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(65, 1) == nullptr);
}

TEST(exhaustion_returns_nullptr)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(32, 1) != nullptr);
    CHECK(arena.allocate(32, 1) != nullptr);
    CHECK(arena.allocate(1, 1) == nullptr);
}

TEST(padding_can_push_an_allocation_over_the_end)
{
    alignas(64) std::uint8_t buf[16];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(9, 1) != nullptr);
    // 7 bytes remain, but an 8-aligned 8-byte block needs offset 16 -> 24 > 16.
    CHECK(arena.allocate(8, 8) == nullptr);
}

TEST(failed_allocation_consumes_nothing)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(40, 1) != nullptr);
    CHECK(arena.allocate(40, 1) == nullptr); // doesn't fit
    CHECK(arena.allocate(24, 1) != nullptr); // exactly what's left still fits
}

TEST(zero_alignment_is_rejected)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(8, 0) == nullptr);
}

TEST(non_power_of_two_alignment_is_rejected)
{
    alignas(64) std::uint8_t buf[256];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(8, 3) == nullptr);
    CHECK(arena.allocate(8, 6) == nullptr);
    CHECK(arena.allocate(8, 12) == nullptr);
    CHECK(arena.allocate(8, 100) == nullptr);
}

TEST(rejected_alignment_consumes_nothing)
{
    alignas(64) std::uint8_t buf[16];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(8, 3) == nullptr);
    CHECK(arena.allocate(16, 1) == buf);
}

TEST(alignment_larger_than_the_arena_fails)
{
    alignas(4096) std::uint8_t buf[64];
    Arena arena(buf + 1, 63);
    CHECK(arena.allocate(1, 4096) == nullptr);
}

TEST(size_overflow_is_rejected)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    const std::size_t max = std::numeric_limits<std::size_t>::max();
    CHECK(arena.allocate(max, 1) == nullptr);
    CHECK(arena.allocate(max - 8, 16) == nullptr);
    CHECK(arena.allocate(max, 8) == nullptr);
    // Arena still usable afterwards.
    CHECK(arena.allocate(8, 8) != nullptr);
}

TEST(huge_alignment_does_not_overflow_padding_math)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    const std::size_t big_pow2 = std::size_t{1} << (sizeof(std::size_t) * 8 - 1);
    CHECK(arena.allocate(1, big_pow2) == nullptr);
    CHECK(arena.allocate(8, 8) != nullptr);
}

TEST(reset_makes_the_whole_arena_available_again)
{
    alignas(64) std::uint8_t buf[64];
    Arena arena(buf, sizeof(buf));
    CHECK(arena.allocate(64, 1) == buf);
    CHECK(arena.allocate(1, 1) == nullptr);
    arena.reset();
    CHECK(arena.allocate(64, 1) == buf);
}

TEST(reset_restarts_at_the_beginning)
{
    alignas(64) std::uint8_t buf[128];
    Arena arena(buf, sizeof(buf));
    void* first = arena.allocate(10, 8);
    arena.allocate(20, 8);
    arena.reset();
    CHECK(arena.allocate(10, 8) == first);
}

TEST(reset_on_a_fresh_arena_is_harmless)
{
    alignas(64) std::uint8_t buf[32];
    Arena arena(buf, sizeof(buf));
    arena.reset();
    arena.reset();
    CHECK(arena.allocate(32, 1) == buf);
}

TEST(zero_sized_arena_cannot_allocate)
{
    std::uint8_t byte;
    Arena arena(&byte, 0);
    CHECK(arena.allocate(1, 1) == nullptr);
}

TEST(many_small_aligned_allocations_fill_the_arena)
{
    alignas(64) std::uint8_t buf[256];
    Arena arena(buf, sizeof(buf));
    int count = 0;
    while (void* p = arena.allocate(8, 8))
    {
        CHECK(aligned(p, 8));
        CHECK(inside(p, 8, buf, sizeof(buf)));
        std::memset(p, 0xEE, 8);
        ++count;
        if (count > 1000)
            break;
    }
    CHECK_EQ(count, 32);
}

TEST_MAIN()
