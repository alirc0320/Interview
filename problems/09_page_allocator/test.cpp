#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <limits>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - allocate(n) returns the first page index of n contiguous free
//     pages, or std::nullopt if n == 0, n > total, or no contiguous run
//     of n free pages exists. Which run is chosen (first-fit, best-fit,
//     ...) is NOT pinned down, so tests only check contiguity, in-range
//     and non-overlap -- never specific indices (except on a
//     completely empty/full allocator where only one answer exists).
//   - free(start, n) returns true iff every page in [start, start + n)
//     is in range AND currently allocated; otherwise it returns false
//     and changes nothing (all-or-nothing).
// ---------------------------------------------------------------------

namespace
{
// True if [a, a+an) and [b, b+bn) overlap.
bool overlaps(std::size_t a, std::size_t an, std::size_t b, std::size_t bn)
{
    return a < b + bn && b < a + an;
}
} // namespace

TEST(allocate_all_pages_from_empty_starts_at_zero)
{
    PageAllocator pa(16);
    auto r = pa.allocate(16);
    CHECK(r.has_value());
    CHECK(r && *r == 0u);
}

TEST(allocate_single_page)
{
    PageAllocator pa(8);
    auto r = pa.allocate(1);
    CHECK(r.has_value());
    CHECK(r && *r < 8u);
}

TEST(allocate_zero_pages_fails)
{
    PageAllocator pa(8);
    CHECK(!pa.allocate(0).has_value());
}

TEST(allocate_more_than_total_fails)
{
    PageAllocator pa(8);
    CHECK(!pa.allocate(9).has_value());
    CHECK(!pa.allocate(std::numeric_limits<std::size_t>::max()).has_value());
}

TEST(zero_page_allocator_cannot_allocate)
{
    PageAllocator pa(0);
    CHECK(!pa.allocate(1).has_value());
}

TEST(exhaustion_then_failure)
{
    PageAllocator pa(4);
    CHECK(pa.allocate(4).has_value());
    CHECK(!pa.allocate(1).has_value());
}

TEST(allocations_never_overlap_and_stay_in_range)
{
    PageAllocator pa(32);
    struct Run
    {
        std::size_t start;
        std::size_t n;
    };
    std::vector<Run> runs;
    for (std::size_t n : {3u, 1u, 5u, 2u, 8u, 4u, 1u, 6u})
    {
        auto r = pa.allocate(n);
        CHECK(r.has_value());
        if (!r)
            continue;
        CHECK(*r + n <= 32u);
        for (const Run& other : runs)
            CHECK(!overlaps(*r, n, other.start, other.n));
        runs.push_back({*r, n});
    }
}

TEST(fills_exactly_with_many_single_page_allocations)
{
    PageAllocator pa(10);
    std::vector<bool> seen(10, false);
    for (int i = 0; i < 10; ++i)
    {
        auto r = pa.allocate(1);
        CHECK(r.has_value());
        if (!r)
            continue;
        CHECK(*r < 10u);
        CHECK(!seen[*r]);
        seen[*r] = true;
    }
    CHECK(!pa.allocate(1).has_value());
}

TEST(freed_pages_can_be_reallocated)
{
    PageAllocator pa(4);
    auto r = pa.allocate(4);
    CHECK(r.has_value());
    CHECK(pa.free(*r, 4));
    CHECK(pa.allocate(4).has_value());
}

TEST(free_returns_pages_to_the_pool)
{
    PageAllocator pa(8);
    auto a = pa.allocate(4);
    auto b = pa.allocate(4);
    CHECK(a && b);
    CHECK(!pa.allocate(1).has_value());
    CHECK(pa.free(*a, 4));
    auto c = pa.allocate(4);
    CHECK(c.has_value());
    CHECK(c && a && *c == *a); // only one 4-page hole exists
}

TEST(fragmentation_prevents_large_contiguous_allocation)
{
    PageAllocator pa(4);
    // Take every page one at a time so page indices are known: {0,1,2,3}
    // in some order. Free two NON-adjacent ones.
    std::size_t page[4];
    for (auto& p : page)
    {
        auto r = pa.allocate(1);
        CHECK(r.has_value());
        p = r.value_or(0);
    }
    // Find two non-adjacent pages among the four (0 and 2, or 1 and 3).
    std::size_t x = 0, y = 2;
    CHECK(pa.free(x, 1));
    CHECK(pa.free(y, 1));

    CHECK(!pa.allocate(2).has_value()); // 2 pages free, but not contiguous
    CHECK(pa.allocate(1).has_value());
    CHECK(pa.allocate(1).has_value());
    CHECK(!pa.allocate(1).has_value());
}

TEST(adjacent_frees_coalesce_into_one_run)
{
    PageAllocator pa(8);
    std::vector<std::size_t> pages;
    for (int i = 0; i < 8; ++i)
    {
        auto r = pa.allocate(1);
        CHECK(r.has_value());
        pages.push_back(r.value_or(0));
    }
    for (std::size_t i = 0; i < 8; ++i)
        CHECK(pa.free(i, 1));
    auto r = pa.allocate(8);
    CHECK(r.has_value());
}

TEST(double_free_is_detected)
{
    PageAllocator pa(8);
    auto r = pa.allocate(4);
    CHECK(r.has_value());
    CHECK(pa.free(*r, 4));
    CHECK(!pa.free(*r, 4));
}

TEST(freeing_never_allocated_pages_fails)
{
    PageAllocator pa(8);
    CHECK(!pa.free(0, 1));
    CHECK(!pa.free(3, 2));
}

TEST(free_past_the_end_of_the_aperture_fails)
{
    PageAllocator pa(8);
    CHECK(pa.allocate(8).has_value());
    CHECK(!pa.free(8, 1));
    CHECK(!pa.free(7, 2));
    CHECK(!pa.free(100, 1));
}

TEST(free_range_overflow_fails)
{
    PageAllocator pa(8);
    CHECK(pa.allocate(8).has_value());
    const std::size_t max = std::numeric_limits<std::size_t>::max();
    CHECK(!pa.free(max, 2));
    CHECK(!pa.free(2, max));
    CHECK(!pa.free(max, max));
}

TEST(failed_free_changes_nothing)
{
    PageAllocator pa(4);
    // Allocate pages one at a time, then free one so the range [0,4)
    // is only partially allocated.
    for (int i = 0; i < 4; ++i)
        CHECK(pa.allocate(1).has_value());
    CHECK(pa.free(1, 1));

    CHECK(!pa.free(0, 4)); // page 1 is not allocated -> whole call must fail

    // Pages 0, 2, 3 must still be allocated: exactly one free page remains.
    CHECK(pa.allocate(1).has_value());
    CHECK(!pa.allocate(1).has_value());
}

TEST(free_of_a_sub_range_of_an_allocation_is_all_or_nothing_on_validity)
{
    PageAllocator pa(8);
    auto r = pa.allocate(4);
    CHECK(r.has_value());
    // Freeing a range that starts inside the allocation but runs past its
    // end into unallocated pages is invalid.
    CHECK(!pa.free(*r + 2, 4));
}

TEST(allocate_after_partial_free_reuses_the_hole)
{
    PageAllocator pa(6);
    auto a = pa.allocate(2);
    auto b = pa.allocate(2);
    auto c = pa.allocate(2);
    CHECK(a && b && c);
    CHECK(pa.free(*b, 2));
    auto d = pa.allocate(2);
    CHECK(d && b && *d == *b); // the only 2-page hole
    CHECK(!pa.allocate(1).has_value());
}

TEST_MAIN()
