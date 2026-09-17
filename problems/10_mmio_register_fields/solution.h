#pragma once
#include <cstdint>

// TODO: implement read/set/clear/update helpers for 32-bit MMIO
// registers without corrupting unrelated bits. Think about why
// `volatile` might be necessary but insufficient, and how you'd mock
// registers in a unit test (hint: these take a pointer, so tests can
// pass a plain local variable instead of real hardware).
std::uint32_t read_field(volatile std::uint32_t* reg, unsigned start, unsigned count);

void set_field(volatile std::uint32_t* reg, unsigned start, unsigned count, std::uint32_t value);

void clear_field(volatile std::uint32_t* reg, unsigned start, unsigned count);
