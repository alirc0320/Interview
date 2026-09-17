#pragma once
#include <cstdint>
#include <vector>

struct Descriptor
{
    std::uint64_t src;
    std::uint64_t dst;
    std::uint32_t length;
};

// TODO: implement. Reject zero-length transfers, address+length
// overflow, prohibited regions, overlap when unsupported, and
// excessive total transfer size.
bool validate_descriptors(const std::vector<Descriptor>& descriptors);
