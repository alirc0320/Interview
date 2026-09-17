#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

using namespace std;

// Scatter/gather DMA stream writer
//
// A source stream produces bytes in chunks of varying size, delivered
// one at a time. Write them sequentially across a fixed list of
// destination memory regions that also vary in capacity. Preserve
// byte order, and handle a chunk that only partially fills the
// remaining space in a destination, or that spans multiple
// destination regions.
//
// Additional cases in scope:
//  - A destination region may have capacity 0 (including two or more
//    zero-capacity regions back to back); such regions must be
//    skipped without ever being written to.
//  - A single feed() call may by itself exceed all remaining
//    destination capacity, not just across multiple calls.
//  - feed() may be called with chunk_size 0, or again after
//    is_complete() is already true; both must be no-ops that return 0.
//  - The destination list may contain exactly one region.
//  - A region's capacity is not bounded to a small/byte-sized range.

// Example: destinations of capacity 5, 5, and 2 bytes. Feeding an
// 8-byte chunk then a 4-byte chunk fills all three regions exactly,
// with the first chunk split across regions 0 and 1, and the second
// chunk split across regions 1 and 2.

struct DestRegion
{
    std::uint8_t* data;
    std::size_t capacity;
};

class ScatterGatherWriter
{
  public:
    /**
     * destinations are filled strictly in order: region 0 must be
     * completely full before any byte is written to region 1, and so
     * on. The writer does not own or copy these buffers -- they must
     * outlive the writer.
     */

    /**
     * Writes as many bytes from [chunk, chunk + chunk_size) as fit
     * into the remaining destination space, preserving order. Returns
     * the number of bytes actually written, which may be less than
     * chunk_size if the destinations filled up.
     */
    ScatterGatherWriter(std::vector<DestRegion> destinations) : dest_buf(destinations)
    {
        total_bytes_written = 0;
        curr_dest_buf_idx = 0;
        curr_data_region_idx = 0;
        completely_filled = false;
    };

    /**
     * Writes as many bytes from [chunk, chunk + chunk_size) as fit
     * into the remaining destination space, preserving order. Returns
     * the number of bytes actually written, which may be less than
     * chunk_size if the destinations filled up.
     */
    std::size_t feed(const std::uint8_t* chunk, std::size_t chunk_size);

    /**
     * Total bytes written across all destinations so far.
     */
    std::size_t bytes_written() const;

    /**
     * True once every destination region is completely full.
     */
    bool is_complete() const;

  private:
    /* TODO */
    vector<DestRegion> dest_buf;
    size_t total_bytes_written;

    /* which data region we are currently on */
    size_t curr_dest_buf_idx;

    /* where in that data region we are currently */
    size_t curr_data_region_idx;

    /* how much space we have left in the data region. calculated by
     * capacity of region - curr_data_region_idx */
    bool completely_filled;
};
