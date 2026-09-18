#include "minitest.h"
#include "solution.h"

#include <algorithm>
#include <cstdint>
#include <vector>

// ---------------------------------------------------------------------
// The wire format isn't specified beyond "explicit endianness, reject
// malformed input", so these tests avoid pinning byte offsets or which
// endianness you pick. What IS checked:
//   - round-trip fidelity for edge-case field values
//   - deserialize() rejects empty / truncated input
//   - the encoding is deterministic and distinguishes different commands
//   - multi-byte fields are laid out contiguously in ONE consistent byte
//     order (all big-endian or all little-endian), independent of the
//     host -- catches memcpy/reinterpret_cast of the struct
//   - the encoded size holds at least the 18 bytes of real field data
//     (1 + 1 + 8 + 4 + 4)
// Add tests for your own chosen layout below the marked line.
// ---------------------------------------------------------------------

namespace
{

Command make(std::uint8_t opcode, std::uint8_t flags, std::uint64_t address, std::uint32_t length,
             std::uint32_t checksum)
{
    Command c{};
    c.opcode = opcode;
    c.flags = flags;
    c.address = address;
    c.length = length;
    c.checksum = checksum;
    return c;
}

bool same(const Command& a, const Command& b)
{
    return a.opcode == b.opcode && a.flags == b.flags && a.address == b.address &&
           a.length == b.length && a.checksum == b.checksum;
}

bool round_trips(const Command& c)
{
    auto bytes = serialize(c);
    auto back = deserialize(bytes);
    return back.has_value() && same(*back, c);
}

// Looks for `pattern` as a contiguous subsequence of `bytes`.
bool contains(const std::vector<std::uint8_t>& bytes, const std::vector<std::uint8_t>& pattern)
{
    return std::search(bytes.begin(), bytes.end(), pattern.begin(), pattern.end()) != bytes.end();
}

} // namespace

TEST(round_trip_typical_command)
{
    CHECK(round_trips(make(0x12, 0x34, 0x1122334455667788ull, 4096, 0xCAFEBABEu)));
}

TEST(round_trip_all_zero)
{
    CHECK(round_trips(make(0, 0, 0, 0, 0)));
}

TEST(round_trip_all_ones)
{
    CHECK(round_trips(make(0xFF, 0xFF, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFu, 0xFFFFFFFFu)));
}

TEST(round_trip_each_field_independently)
{
    CHECK(round_trips(make(0x7F, 0, 0, 0, 0)));
    CHECK(round_trips(make(0, 0x81, 0, 0, 0)));
    CHECK(round_trips(make(0, 0, 0x8000000000000001ull, 0, 0)));
    CHECK(round_trips(make(0, 0, 0, 0x80000001u, 0)));
    CHECK(round_trips(make(0, 0, 0, 0, 0x80000001u)));
}

TEST(round_trip_asymmetric_byte_patterns)
{
    // Every byte differs so a byte-order or field-order bug can't cancel out.
    CHECK(round_trips(make(0xA1, 0xB2, 0x0102030405060708ull, 0x090A0B0Cu, 0x0D0E0F10u)));
    CHECK(round_trips(make(0x01, 0x02, 0x8070605040302010ull, 0xF0E0D0C0u, 0x0F1E2D3Cu)));
}

TEST(serialize_is_deterministic)
{
    Command c = make(1, 2, 0x1122334455667788ull, 0x99AABBCCu, 0xDDEEFF00u);
    CHECK(serialize(c) == serialize(c));
}

TEST(encoding_holds_all_field_data)
{
    auto bytes = serialize(make(1, 2, 3, 4, 5));
    CHECK(bytes.size() >= 18u);
}

TEST(encoding_size_does_not_depend_on_values)
{
    auto a = serialize(make(0, 0, 0, 0, 0));
    auto b = serialize(make(0xFF, 0xFF, ~0ull, ~0u, ~0u));
    CHECK_EQ(a.size(), b.size());
}

TEST(different_commands_encode_differently)
{
    Command base = make(1, 2, 3, 4, 5);
    auto b = serialize(base);
    CHECK(b != serialize(make(9, 2, 3, 4, 5)));
    CHECK(b != serialize(make(1, 9, 3, 4, 5)));
    CHECK(b != serialize(make(1, 2, 9, 4, 5)));
    CHECK(b != serialize(make(1, 2, 3, 9, 5)));
    CHECK(b != serialize(make(1, 2, 3, 4, 9)));
}

TEST(address_bytes_are_contiguous_in_one_consistent_byte_order)
{
    auto bytes = serialize(make(0, 0, 0x0102030405060708ull, 0, 0));
    const std::vector<std::uint8_t> big = {1, 2, 3, 4, 5, 6, 7, 8};
    const std::vector<std::uint8_t> little = {8, 7, 6, 5, 4, 3, 2, 1};
    CHECK(contains(bytes, big) || contains(bytes, little));
}

TEST(length_bytes_are_contiguous_in_one_consistent_byte_order)
{
    auto bytes = serialize(make(0, 0, 0, 0x0A0B0C0Du, 0));
    const std::vector<std::uint8_t> big = {0x0A, 0x0B, 0x0C, 0x0D};
    const std::vector<std::uint8_t> little = {0x0D, 0x0C, 0x0B, 0x0A};
    CHECK(contains(bytes, big) || contains(bytes, little));
}

TEST(checksum_bytes_are_contiguous_in_one_consistent_byte_order)
{
    auto bytes = serialize(make(0, 0, 0, 0, 0x11223344u));
    const std::vector<std::uint8_t> big = {0x11, 0x22, 0x33, 0x44};
    const std::vector<std::uint8_t> little = {0x44, 0x33, 0x22, 0x11};
    CHECK(contains(bytes, big) || contains(bytes, little));
}

TEST(all_multibyte_fields_use_the_same_endianness)
{
    auto bytes = serialize(make(0, 0, 0x0102030405060708ull, 0x090A0B0Cu, 0x0D0E0F10u));
    const std::vector<std::uint8_t> a_be = {1, 2, 3, 4, 5, 6, 7, 8};
    const std::vector<std::uint8_t> l_be = {9, 10, 11, 12};
    const std::vector<std::uint8_t> c_be = {13, 14, 15, 16};
    const std::vector<std::uint8_t> a_le = {8, 7, 6, 5, 4, 3, 2, 1};
    const std::vector<std::uint8_t> l_le = {12, 11, 10, 9};
    const std::vector<std::uint8_t> c_le = {16, 15, 14, 13};

    bool all_big = contains(bytes, a_be) && contains(bytes, l_be) && contains(bytes, c_be);
    bool all_little = contains(bytes, a_le) && contains(bytes, l_le) && contains(bytes, c_le);
    CHECK(all_big || all_little);
}

TEST(deserialize_rejects_empty_input)
{
    CHECK(!deserialize({}).has_value());
}

TEST(deserialize_rejects_every_truncation)
{
    auto bytes = serialize(make(0x12, 0x34, 0x1122334455667788ull, 4096, 0xCAFEBABEu));
    for (std::size_t len = 0; len < bytes.size(); ++len)
    {
        std::vector<std::uint8_t> cut(bytes.begin(), bytes.begin() + static_cast<long>(len));
        CHECK(!deserialize(cut).has_value());
    }
}

TEST(deserialize_does_not_read_past_a_short_buffer)
{
    // Under ASan an over-read of these tiny heap buffers would be reported.
    for (std::size_t len : {1u, 2u, 7u, 17u})
    {
        std::vector<std::uint8_t> tiny(len, 0xFF);
        tiny.shrink_to_fit();
        CHECK(!deserialize(tiny).has_value());
    }
}

TEST(deserialize_of_serialize_output_is_stable_across_repeats)
{
    Command c = make(3, 4, 0xDEADBEEFCAFEF00Dull, 77, 88);
    auto once = deserialize(serialize(c));
    CHECK(once.has_value());
    if (!once)
        return;
    auto twice = deserialize(serialize(*once));
    CHECK(twice.has_value());
    CHECK(twice && same(*twice, c));
}

// ---- add tests for your own wire layout below this line --------------
// e.g. TEST(opcode_is_the_first_byte) { ... }
//      TEST(rejects_trailing_garbage) { ... }
//      TEST(known_good_golden_bytes) { ... }

TEST_MAIN()
