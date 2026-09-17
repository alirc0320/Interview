#include "minitest.h"
#include "solution.h"

#include <cstring>

// Compares `actual` against `expected` byte-for-byte over sizeof(expected),
// so tests can check a whole region's contents with one CHECK.
#define CHECK_BYTES(actual, expected) CHECK(memcmp((actual), (expected), sizeof(expected)) == 0)

namespace
{

// The layout used by most tests below: three regions of capacity 5, 5, and
// 2 bytes (12 bytes total), matching the worked example in solution.h.
struct ThreeRegionFixture
{
    uint8_t region0[5]{};
    uint8_t region1[5]{};
    uint8_t region2[2]{};
    vector<DestRegion> dest{{region0, 5}, {region1, 5}, {region2, 2}};
};

} // namespace

TEST(fills_across_two_feed_calls)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk_0[8] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB};
    uint8_t chunk_1[4] = {0xBC, 0xBD, 0xBE, 0xBF};

    CHECK_EQ(sgw.is_complete(), false);
    CHECK_EQ(sgw.feed(chunk_0, 8), static_cast<size_t>(8));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(8));
    CHECK_EQ(sgw.feed(chunk_1, 4), static_cast<size_t>(4));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(12));
    CHECK_EQ(sgw.is_complete(), true);
}

TEST(fills_one_byte_at_a_time)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    // Same 8 bytes as the test above, but each fed in its own single-byte
    // call, to exercise every region-boundary crossing one step at a time.
    uint8_t bytes[8] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB};
    uint8_t chunk_1[4] = {0xBC, 0xBD, 0xBE, 0xBF};

    CHECK_EQ(sgw.is_complete(), false);
    for (uint8_t& b : bytes)
    {
        CHECK_EQ(sgw.feed(&b, 1), static_cast<size_t>(1));
    }
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(8));
    CHECK_EQ(sgw.feed(chunk_1, 4), static_cast<size_t>(4));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(12));
    CHECK_EQ(sgw.is_complete(), true);
}

TEST(single_feed_call_spans_all_regions)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk[12] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF};

    CHECK_EQ(sgw.is_complete(), false);
    CHECK_EQ(sgw.feed(chunk, 12), static_cast<size_t>(12));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(12));
    CHECK_EQ(sgw.is_complete(), true);
    CHECK(memcmp(f.region0, chunk, sizeof(f.region0)) == 0);
    CHECK(memcmp(f.region1, chunk + 5, sizeof(f.region1)) == 0);
    CHECK(memcmp(f.region2, chunk + 10, sizeof(f.region2)) == 0);
}

TEST(content_matches_after_full_fill)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk_0[8] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB};
    uint8_t chunk_1[4] = {0xBC, 0xBD, 0xBE, 0xBF};

    CHECK_EQ(sgw.feed(chunk_0, 8), static_cast<size_t>(8));
    CHECK_EQ(sgw.feed(chunk_1, 4), static_cast<size_t>(4));
    CHECK_EQ(sgw.is_complete(), true);

    uint8_t expected_0[5] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};
    uint8_t expected_1[5] = {0xAF, 0xBA, 0xBB, 0xBC, 0xBD};
    uint8_t expected_2[2] = {0xBE, 0xBF};

    CHECK_BYTES(f.region0, expected_0);
    CHECK_BYTES(f.region1, expected_1);
    CHECK_BYTES(f.region2, expected_2);
}

TEST(partial_last_region_is_not_complete)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk_0[8] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB};
    uint8_t chunk_1[3] = {0xBC, 0xBD, 0xBE};

    CHECK_EQ(sgw.feed(chunk_0, 8), static_cast<size_t>(8));
    // Leaves the last region (capacity 2) with only 1 of 2 bytes filled.
    CHECK_EQ(sgw.feed(chunk_1, 3), static_cast<size_t>(3));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(11));
    CHECK_EQ(sgw.is_complete(), false);

    uint8_t expected_0[5] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};
    uint8_t expected_1[5] = {0xAF, 0xBA, 0xBB, 0xBC, 0xBD};
    uint8_t expected_2[1] = {0xBE};

    CHECK_BYTES(f.region0, expected_0);
    CHECK_BYTES(f.region1, expected_1);
    CHECK_BYTES(f.region2, expected_2);
}

TEST(overflow_by_one_byte_after_complete)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk_0[8] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB};
    uint8_t chunk_1[4] = {0xBC, 0xBD, 0xBE, 0xBF};
    CHECK_EQ(sgw.feed(chunk_0, 8), static_cast<size_t>(8));
    CHECK_EQ(sgw.feed(chunk_1, 4), static_cast<size_t>(4));
    CHECK_EQ(sgw.is_complete(), true);

    uint8_t extra[1] = {0xCA};
    CHECK_EQ(sgw.feed(extra, 1), static_cast<size_t>(0));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(12));
    CHECK_EQ(sgw.is_complete(), true);
}

TEST(overflow_by_many_bytes_after_complete)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk_0[8] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB};
    uint8_t chunk_1[4] = {0xBC, 0xBD, 0xBE, 0xBF};
    CHECK_EQ(sgw.feed(chunk_0, 8), static_cast<size_t>(8));
    CHECK_EQ(sgw.feed(chunk_1, 4), static_cast<size_t>(4));
    CHECK_EQ(sgw.is_complete(), true);

    uint8_t extra[30] = {0xCA};
    CHECK_EQ(sgw.feed(extra, 30), static_cast<size_t>(0));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(12));
    CHECK_EQ(sgw.is_complete(), true);
}

TEST(single_feed_call_overflowing_past_last_region)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    // 13 bytes into 12 bytes of total capacity, all in one call -- the
    // overflow is detected mid-call, not on a follow-up call after
    // is_complete() is already true.
    uint8_t chunk[13] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA,
                         0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xCA};

    CHECK_EQ(sgw.feed(chunk, 13), static_cast<size_t>(12));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(12));
    CHECK_EQ(sgw.is_complete(), true);

    uint8_t expected_0[5] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};
    uint8_t expected_1[5] = {0xAF, 0xBA, 0xBB, 0xBC, 0xBD};
    uint8_t expected_2[2] = {0xBE, 0xBF};

    CHECK_BYTES(f.region0, expected_0);
    CHECK_BYTES(f.region1, expected_1);
    CHECK_BYTES(f.region2, expected_2);
}

TEST(zero_byte_feed_before_any_write_is_noop)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk[12] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF};

    CHECK_EQ(sgw.is_complete(), false);
    CHECK_EQ(sgw.feed(chunk, 0), static_cast<size_t>(0));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(0));
    CHECK_EQ(sgw.is_complete(), false);
}

TEST(zero_byte_feed_interleaved_with_full_write)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk[12] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF};

    CHECK_EQ(sgw.feed(chunk, 0), static_cast<size_t>(0));
    CHECK_EQ(sgw.feed(chunk, 0), static_cast<size_t>(0));
    CHECK_EQ(sgw.feed(chunk, 12), static_cast<size_t>(12));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(12));
    CHECK_EQ(sgw.is_complete(), true);
}

TEST(exact_fill_of_first_region_alone_is_not_complete)
{
    ThreeRegionFixture f;
    ScatterGatherWriter sgw(f.dest);

    uint8_t chunk[5] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};

    CHECK_EQ(sgw.feed(chunk, 5), static_cast<size_t>(5));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(5));
    // Region 0 is exactly full, but regions 1 and 2 are still empty.
    CHECK_EQ(sgw.is_complete(), false);
}

TEST(single_region_destination_completes_on_full_fill)
{
    uint8_t region0[5] = {};
    vector<DestRegion> dest{{region0, 5}};
    ScatterGatherWriter sgw(dest);

    uint8_t chunk[5] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};

    CHECK_EQ(sgw.is_complete(), false);
    CHECK_EQ(sgw.feed(chunk, 5), static_cast<size_t>(5));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(5));
    CHECK_EQ(sgw.is_complete(), true);
    CHECK(memcmp(region0, chunk, sizeof(chunk)) == 0);
}

TEST(skips_over_single_zero_capacity_region)
{
    uint8_t region0[5] = {};
    uint8_t empty_region[1] = {}; // capacity 0; never actually written to
    uint8_t region2[2] = {};
    vector<DestRegion> dest{{region0, 5}, {empty_region, 0}, {region2, 2}};
    ScatterGatherWriter sgw(dest);

    uint8_t chunk[7] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0};

    CHECK_EQ(sgw.feed(chunk, 7), static_cast<size_t>(7));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(7));
    CHECK_EQ(sgw.is_complete(), true);

    uint8_t expected_0[5] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};
    uint8_t expected_2[2] = {0xAF, 0xB0};
    CHECK_BYTES(region0, expected_0);
    CHECK_BYTES(region2, expected_2);
}

TEST(skips_over_consecutive_zero_capacity_regions)
{
    uint8_t empty_a[1] = {}; // capacity 0
    uint8_t empty_b[1] = {}; // capacity 0
    uint8_t region2[5] = {};
    vector<DestRegion> dest{{empty_a, 0}, {empty_b, 0}, {region2, 5}};
    ScatterGatherWriter sgw(dest);

    uint8_t chunk[5] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE};

    CHECK_EQ(sgw.feed(chunk, 5), static_cast<size_t>(5));
    CHECK_EQ(sgw.bytes_written(), static_cast<size_t>(5));
    CHECK_EQ(sgw.is_complete(), true);
    CHECK(memcmp(region2, chunk, sizeof(chunk)) == 0);
}

TEST(handles_region_capacity_larger_than_a_byte)
{
    constexpr size_t kCapacity = 300; // exceeds what an 8-bit offset could index
    vector<uint8_t> region(kCapacity, 0);
    vector<DestRegion> dest{{region.data(), kCapacity}};
    ScatterGatherWriter sgw(dest);

    vector<uint8_t> chunk(kCapacity);
    for (size_t i = 0; i < kCapacity; ++i)
    {
        chunk[i] = static_cast<uint8_t>(i);
    }

    CHECK_EQ(sgw.feed(chunk.data(), kCapacity), kCapacity);
    CHECK_EQ(sgw.bytes_written(), kCapacity);
    CHECK_EQ(sgw.is_complete(), true);
    CHECK(memcmp(region.data(), chunk.data(), kCapacity) == 0);
}

TEST_MAIN()
