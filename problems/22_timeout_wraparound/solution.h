#pragma once
#include <cstdint>

// 32-bit hardware counter wraps naturally. Assume timeout is less
// than half the counter range.
// TODO: implement, correct across wraparound.
bool expired(std::uint32_t start, std::uint32_t now, std::uint32_t timeout);
