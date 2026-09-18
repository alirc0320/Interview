#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <vector>

// Contract: return the number of page faults when servicing `pages`
// in order with `num_frames` frames, starting from empty frames (so the
// first touch of any page is a fault). FIFO evicts the page that has been
// resident longest; LRU evicts the least recently referenced page.
// num_frames == 0 isn't tested.

using P = std::vector<int>;
constexpr auto FIFO = ReplacementPolicy::FIFO;
constexpr auto LRU = ReplacementPolicy::LRU;

TEST(empty_reference_string_has_no_faults)
{
    CHECK_EQ(simulate_page_replacement({}, 3, FIFO), 0u);
    CHECK_EQ(simulate_page_replacement({}, 3, LRU), 0u);
}

TEST(first_touches_always_fault)
{
    CHECK_EQ(simulate_page_replacement(P{1, 2, 3}, 3, FIFO), 3u);
    CHECK_EQ(simulate_page_replacement(P{1, 2, 3}, 3, LRU), 3u);
}

TEST(repeated_page_hits_after_first_fault)
{
    CHECK_EQ(simulate_page_replacement(P{5, 5, 5, 5}, 1, FIFO), 1u);
    CHECK_EQ(simulate_page_replacement(P{5, 5, 5, 5}, 1, LRU), 1u);
}

TEST(enough_frames_means_faults_equal_distinct_pages)
{
    P refs = {1, 2, 3, 1, 2, 3, 4, 1, 4, 2};
    CHECK_EQ(simulate_page_replacement(refs, 4, FIFO), 4u);
    CHECK_EQ(simulate_page_replacement(refs, 4, LRU), 4u);
    CHECK_EQ(simulate_page_replacement(refs, 10, FIFO), 4u);
    CHECK_EQ(simulate_page_replacement(refs, 10, LRU), 4u);
}

TEST(one_frame_faults_on_every_change)
{
    P refs = {1, 1, 2, 2, 1, 3};
    CHECK_EQ(simulate_page_replacement(refs, 1, FIFO), 4u);
    CHECK_EQ(simulate_page_replacement(refs, 1, LRU), 4u);
}

TEST(textbook_reference_string_three_frames)
{
    P refs = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    CHECK_EQ(simulate_page_replacement(refs, 3, FIFO), 15u);
    CHECK_EQ(simulate_page_replacement(refs, 3, LRU), 12u);
}

TEST(beladys_anomaly_under_fifo)
{
    P refs = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    CHECK_EQ(simulate_page_replacement(refs, 3, FIFO), 9u);
    CHECK_EQ(simulate_page_replacement(refs, 4, FIFO), 10u); // MORE frames, MORE faults
}

TEST(lru_has_no_beladys_anomaly_on_the_same_string)
{
    P refs = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    CHECK_EQ(simulate_page_replacement(refs, 3, LRU), 10u);
    CHECK_EQ(simulate_page_replacement(refs, 4, LRU), 8u);
}

TEST(fifo_and_lru_diverge_when_a_page_is_re_referenced)
{
    // Two frames. FIFO evicts 1 despite its recent use; LRU keeps it.
    P refs = {1, 2, 1, 3, 1};
    CHECK_EQ(simulate_page_replacement(refs, 2, FIFO), 4u);
    CHECK_EQ(simulate_page_replacement(refs, 2, LRU), 3u);
}

TEST(hit_does_not_refresh_fifo_order)
{
    // 1 2 | hit 1 | 3 evicts 1 (oldest resident) -> 1 faults again.
    P refs = {1, 2, 1, 3, 1};
    CHECK_EQ(simulate_page_replacement(refs, 2, FIFO), 4u);
}

TEST(sequential_scan_larger_than_memory_thrashes_both_policies)
{
    P refs;
    for (int round = 0; round < 5; ++round)
        for (int p = 0; p < 4; ++p)
            refs.push_back(p);
    CHECK_EQ(simulate_page_replacement(refs, 3, FIFO), 20u);
    CHECK_EQ(simulate_page_replacement(refs, 3, LRU), 20u);
}

TEST(negative_and_large_page_numbers)
{
    P refs = {-1, 1000000, -1, 2147483647, 1000000};
    CHECK_EQ(simulate_page_replacement(refs, 2, LRU), 4u);
    CHECK_EQ(simulate_page_replacement(refs, 3, LRU), 3u);
}

TEST(faults_never_exceed_references)
{
    P refs;
    for (int i = 0; i < 500; ++i)
        refs.push_back((i * 31 + 7) % 13);
    for (std::size_t frames = 1; frames <= 13; ++frames)
    {
        CHECK(simulate_page_replacement(refs, frames, FIFO) <= refs.size());
        CHECK(simulate_page_replacement(refs, frames, LRU) <= refs.size());
    }
    // With 13 frames every distinct page fits.
    CHECK_EQ(simulate_page_replacement(refs, 13, FIFO), 13u);
    CHECK_EQ(simulate_page_replacement(refs, 13, LRU), 13u);
}

TEST_MAIN()
