#pragma once
#include <cstddef>
#include <vector>

enum class ReplacementPolicy
{
    FIFO,
    LRU,
};

// TODO: implement. Simulate page replacement over `pages` (a sequence
// of page-number references) with `num_frames` physical frames.
// Return the number of page faults.
std::size_t simulate_page_replacement(const std::vector<int>& pages, std::size_t num_frames,
                                      ReplacementPolicy policy);
