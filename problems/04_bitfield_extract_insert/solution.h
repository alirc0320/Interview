#pragma once
#include <cstdint>
#include <optional>

// TODO: implement. Handle count == 0, count == 32, invalid ranges
// (start + count > 32), and shifts that would otherwise be UB.
std::optional<std::uint32_t> extract_bits(std::uint32_t value, unsigned start, unsigned count);

std::optional<std::uint32_t> insert_bits(std::uint32_t value, std::uint32_t field, unsigned start,
                                         unsigned count);
