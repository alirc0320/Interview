#pragma once
#include <cstdint>
#include <optional>
#include <vector>

struct Command
{
    std::uint8_t opcode;
    std::uint8_t flags;
    std::uint64_t address;
    std::uint32_t length;
    std::uint32_t checksum;
};

// TODO: implement. Do not reinterpret_cast the struct directly onto
// the wire -- handle endianness explicitly and reject malformed
// input.
std::vector<std::uint8_t> serialize(const Command& cmd);
std::optional<Command> deserialize(const std::vector<std::uint8_t>& bytes);
