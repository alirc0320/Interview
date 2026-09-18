#include "minitest.h"
#include "solution.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

// ---------------------------------------------------------------------
// Mock DMA engine.
//
// dma_read() is declared in solution.h and is meant to be backed by real
// hardware; here it's backed by an in-memory buffer so read_bytes() can be
// exercised without any actual device. Real hardware only supports
// 4096-aligned offset/size, so this mock enforces that contract with
// CHECK() -- if read_bytes() ever calls dma_read() with a misaligned
// offset or size, the test fails right here instead of silently reading
// garbage.
// ---------------------------------------------------------------------
namespace mock_dma
{

inline std::vector<std::uint8_t>& backing_store()
{
    static std::vector<std::uint8_t> store(1u << 20, 0); // 1 MiB of fake device memory
    return store;
}

inline bool& force_failure()
{
    static bool fail = false;
    return fail;
}

// Call at the start of every test that uses the mock, so state from a
// previous test can't leak in.
inline void reset()
{
    std::fill(backing_store().begin(), backing_store().end(), std::uint8_t{0});
    force_failure() = false;
}

// Fills device memory with a repeating 0..255 pattern so tests can verify
// that the bytes landing in dst came from the right offset.
inline void fill_sequential()
{
    auto& store = backing_store();
    for (std::size_t i = 0; i < store.size(); ++i)
        store[i] = static_cast<std::uint8_t>(i & 0xFF);
}

} // namespace mock_dma

bool dma_read(void* dst, std::size_t size, std::size_t offset)
{
    CHECK(offset % 4096 == 0);
    CHECK(size % 4096 == 0);

    if (mock_dma::force_failure())
        return false;

    if (size == 0)
        return true;

    auto& store = mock_dma::backing_store();
    if (offset > store.size() || size > store.size() - offset)
        return false; // would read past the end of mock device memory

    std::memcpy(dst, store.data() + offset, size);
    return true;
}

// ---------------------------------------------------------------------
// read_bytes() tests
//
// Assumed contract (the problem statement leaves these unspecified, so
// pick the standard defensive choices -- adjust these tests if you design
// read_bytes() differently on purpose):
//   - size == 0            -> returns true, dst untouched
//   - dst == nullptr, size > 0  -> returns false
//   - offset + size overflows  -> returns false
//   - dma_read() fails          -> returns false
// ---------------------------------------------------------------------

TEST(reads_unaligned_range_within_a_single_page)
{
    mock_dma::reset();
    mock_dma::fill_sequential();

    std::uint8_t dst[50]{};
    CHECK(read_bytes(dst, sizeof(dst), 100));

    std::uint8_t expected[50];
    for (std::size_t i = 0; i < sizeof(expected); ++i)
        expected[i] = static_cast<std::uint8_t>((100 + i) & 0xFF);
    CHECK(std::memcmp(dst, expected, sizeof(dst)) == 0);
}

TEST(reads_range_spanning_a_page_boundary)
{
    mock_dma::reset();
    mock_dma::fill_sequential();

    std::uint8_t dst[20]{};
    CHECK(read_bytes(dst, sizeof(dst), 4090)); // 4090..4109, crosses the 4096 boundary

    std::uint8_t expected[20];
    for (std::size_t i = 0; i < sizeof(expected); ++i)
        expected[i] = static_cast<std::uint8_t>((4090 + i) & 0xFF);
    CHECK(std::memcmp(dst, expected, sizeof(dst)) == 0);
}

TEST(reads_range_deep_into_device_memory)
{
    mock_dma::reset();
    mock_dma::fill_sequential();

    std::uint8_t dst[300]{};
    CHECK(read_bytes(dst, sizeof(dst), 10007));

    std::uint8_t expected[300];
    for (std::size_t i = 0; i < sizeof(expected); ++i)
        expected[i] = static_cast<std::uint8_t>((10007 + i) & 0xFF);
    CHECK(std::memcmp(dst, expected, sizeof(dst)) == 0);
}

TEST(zero_size_succeeds_without_touching_dst)
{
    mock_dma::reset();

    std::uint8_t dst[4] = {0xAB, 0xAB, 0xAB, 0xAB};
    CHECK(read_bytes(dst, 0, 1234));
    CHECK(dst[0] == 0xAB && dst[1] == 0xAB && dst[2] == 0xAB && dst[3] == 0xAB);
}

TEST(null_dst_with_nonzero_size_fails)
{
    mock_dma::reset();
    CHECK(!read_bytes(nullptr, 10, 0));
}

TEST(overflowing_offset_plus_size_fails)
{
    mock_dma::reset();

    std::uint8_t dst[100]{};
    std::size_t offset = std::numeric_limits<std::size_t>::max() - 10;
    CHECK(!read_bytes(dst, sizeof(dst), offset));
}

TEST(dma_failure_propagates_as_false)
{
    mock_dma::reset();
    mock_dma::force_failure() = true;

    std::uint8_t dst[50]{};
    CHECK(!read_bytes(dst, sizeof(dst), 0));
}

TEST_MAIN()
