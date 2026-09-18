#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - receive_chunk(offset, data, length) returns true if the chunk was
//     accepted, false if rejected. A rejected chunk changes nothing.
//   - Rejected: data == nullptr with length > 0; offset + length past
//     image_size (including size_t overflow); a chunk that overlaps
//     already-received bytes with DIFFERENT contents (conflicting
//     duplicate).
//   - Accepted: chunks in any order; an exact duplicate of already
//     received data (same offset, same bytes) is accepted and idempotent;
//     partially overlapping chunks whose overlapping bytes agree.
//   - is_complete() is true exactly when every byte of [0, image_size)
//     has been received.
//   - Zero-length chunks and image_size == 0 aren't tested (ambiguous).
// ---------------------------------------------------------------------

namespace
{
// Deterministic "firmware image": byte i is (i * 7 + 3) & 0xFF.
std::vector<std::uint8_t> make_image(std::size_t n)
{
    std::vector<std::uint8_t> img(n);
    for (std::size_t i = 0; i < n; ++i)
        img[i] = static_cast<std::uint8_t>((i * 7 + 3) & 0xFF);
    return img;
}

bool send(FirmwareReceiver& rx, const std::vector<std::uint8_t>& img, std::size_t offset,
          std::size_t length)
{
    return rx.receive_chunk(offset, img.data() + offset, length);
}
} // namespace

TEST(fresh_receiver_is_not_complete)
{
    FirmwareReceiver rx(100);
    CHECK(!rx.is_complete());
}

TEST(single_chunk_covering_the_whole_image_completes)
{
    auto img = make_image(64);
    FirmwareReceiver rx(64);
    CHECK(send(rx, img, 0, 64));
    CHECK(rx.is_complete());
}

TEST(in_order_chunks_complete_only_after_the_last)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 25));
    CHECK(!rx.is_complete());
    CHECK(send(rx, img, 25, 25));
    CHECK(!rx.is_complete());
    CHECK(send(rx, img, 50, 25));
    CHECK(!rx.is_complete());
    CHECK(send(rx, img, 75, 25));
    CHECK(rx.is_complete());
}

TEST(out_of_order_chunks_are_accepted)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 50, 25));
    CHECK(send(rx, img, 0, 25));
    CHECK(send(rx, img, 75, 25));
    CHECK(!rx.is_complete()); // [25, 50) still missing
    CHECK(send(rx, img, 25, 25));
    CHECK(rx.is_complete());
}

TEST(reverse_order_delivery_completes)
{
    auto img = make_image(80);
    FirmwareReceiver rx(80);
    for (std::size_t off = 70;; off -= 10)
    {
        CHECK(send(rx, img, off, 10));
        if (off == 0)
            break;
    }
    CHECK(rx.is_complete());
}

TEST(a_hole_in_the_middle_blocks_completion)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 40));
    CHECK(send(rx, img, 60, 40));
    CHECK(!rx.is_complete());
}

TEST(missing_single_byte_blocks_completion)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 99));
    CHECK(!rx.is_complete());
    CHECK(send(rx, img, 99, 1));
    CHECK(rx.is_complete());
}

TEST(exact_duplicate_is_accepted_and_idempotent)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 50));
    CHECK(send(rx, img, 0, 50)); // duplicate
    CHECK(!rx.is_complete());    // duplicate must not count as extra coverage
    CHECK(send(rx, img, 50, 50));
    CHECK(rx.is_complete());
}

TEST(duplicates_do_not_fake_completion)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    for (int i = 0; i < 10; ++i)
        CHECK(send(rx, img, 0, 50));
    CHECK(!rx.is_complete()); // 500 bytes received, only 50 distinct
}

TEST(duplicate_after_completion_is_accepted_and_stays_complete)
{
    auto img = make_image(40);
    FirmwareReceiver rx(40);
    CHECK(send(rx, img, 0, 40));
    CHECK(rx.is_complete());
    CHECK(send(rx, img, 10, 10));
    CHECK(rx.is_complete());
}

TEST(overlapping_chunks_with_matching_bytes_are_accepted)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 60));
    CHECK(send(rx, img, 40, 60)); // overlaps [40, 60), same bytes
    CHECK(rx.is_complete());
}

TEST(conflicting_duplicate_is_rejected)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 50));

    std::vector<std::uint8_t> bad(img.begin(), img.begin() + 50);
    bad[10] ^= 0xFF;
    CHECK(!rx.receive_chunk(0, bad.data(), bad.size()));
}

TEST(conflicting_overlap_is_rejected_and_none_of_it_is_kept)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 50));

    // Chunk [40, 100): the first 10 bytes overlap [40, 50) and one of them
    // is wrong; the rest (50..100) would complete the image if wrongly kept.
    std::vector<std::uint8_t> bad(img.begin() + 40, img.end());
    bad[5] ^= 0xFF; // corrupts byte 45, inside the overlap
    CHECK(!rx.receive_chunk(40, bad.data(), bad.size()));
    CHECK(!rx.is_complete()); // rejected chunk must not contribute coverage

    // The genuine tail is still accepted afterwards.
    CHECK(send(rx, img, 50, 50));
    CHECK(rx.is_complete());
}

TEST(rejection_does_not_prevent_later_valid_chunks)
{
    auto img = make_image(30);
    FirmwareReceiver rx(30);
    CHECK(send(rx, img, 0, 10));
    std::vector<std::uint8_t> bad(10, 0xEE);
    CHECK(!rx.receive_chunk(0, bad.data(), bad.size()));
    CHECK(send(rx, img, 10, 10));
    CHECK(send(rx, img, 20, 10));
    CHECK(rx.is_complete());
}

TEST(chunk_past_end_of_image_is_rejected)
{
    auto img = make_image(200);
    FirmwareReceiver rx(100);
    CHECK(!send(rx, img, 90, 20)); // [90, 110) runs past 100
    CHECK(!send(rx, img, 100, 1));
    CHECK(!send(rx, img, 150, 10));
    CHECK(!rx.is_complete());
}

TEST(rejected_out_of_range_chunk_does_not_count_toward_completion)
{
    auto img = make_image(200);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 0, 50));
    CHECK(!send(rx, img, 50, 100)); // oversized; would "complete" if the valid part were kept
    CHECK(!rx.is_complete());
}

TEST(chunk_ending_exactly_at_image_end_is_accepted)
{
    auto img = make_image(100);
    FirmwareReceiver rx(100);
    CHECK(send(rx, img, 90, 10));
}

TEST(offset_plus_length_overflow_is_rejected)
{
    FirmwareReceiver rx(100);
    std::uint8_t byte = 0;
    const std::size_t max = std::numeric_limits<std::size_t>::max();
    CHECK(!rx.receive_chunk(max, &byte, 1));
    CHECK(!rx.receive_chunk(max - 5, &byte, 10));
    CHECK(!rx.receive_chunk(1, &byte, max));
    CHECK(!rx.is_complete());
}

TEST(null_data_with_nonzero_length_is_rejected)
{
    FirmwareReceiver rx(100);
    CHECK(!rx.receive_chunk(0, nullptr, 10));
    CHECK(!rx.is_complete());
}

TEST(single_byte_image)
{
    FirmwareReceiver rx(1);
    std::uint8_t b = 0x5A;
    CHECK(!rx.is_complete());
    CHECK(rx.receive_chunk(0, &b, 1));
    CHECK(rx.is_complete());
}

TEST(many_single_byte_chunks_shuffled)
{
    const std::size_t n = 257;
    auto img = make_image(n);
    FirmwareReceiver rx(n);
    // Visit offsets in a scrambled but complete order (stride 100 is
    // coprime with 257, so every offset is hit exactly once).
    std::size_t off = 0;
    for (std::size_t i = 0; i < n; ++i)
    {
        CHECK(!rx.is_complete());
        CHECK(send(rx, img, off, 1));
        off = (off + 100) % n;
    }
    CHECK(rx.is_complete());
}

TEST_MAIN()
