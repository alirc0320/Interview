#include "solution.h"

// TODO: implement the declarations from solution.h.

size_t ScatterGatherWriter::bytes_written() const
{
    return total_bytes_written;
}

/**
 * True once every destination region is completely full.
 */
bool ScatterGatherWriter::is_complete() const
{
    return completely_filled;
}

/**
 * Writes as many bytes from [chunk, chunk + chunk_size) as fit
 * into the remaining destination space, preserving order. Returns
 * the number of bytes actually written, which may be less than
 * chunk_size if the destinations filled up.
 */
size_t ScatterGatherWriter::feed(const std::uint8_t* chunk, std::size_t chunk_size)
{
    if (is_complete() == true)
    {
        return 0;
    }

    size_t curr_chunk_idx = 0;
    size_t chunks_left = chunk_size;

    /*
     * Record the Capacity of the current destination Region
     */
    size_t capacity = dest_buf[curr_dest_buf_idx].capacity;

    while (chunks_left != 0)
    {

        /*
         * If the current region is full, reset the offset within
         * the region and move to next dest region.
         */
        while (curr_data_region_idx >= capacity && chunks_left != 0)
        {
            /*
             * No destination left to advance into: everything is full.
             * Check bounds BEFORE indexing dest_buf, not after.
             */
            if (curr_dest_buf_idx + 1 >= dest_buf.size())
            {
                total_bytes_written = total_bytes_written + (chunk_size - chunks_left);
                completely_filled = true;
                return chunk_size - chunks_left;
            }

            curr_dest_buf_idx++;
            curr_data_region_idx = 0;
            capacity = dest_buf[curr_dest_buf_idx].capacity;
        }

        /*
         * Index into the current offset within the current region and add the chunks data.
         */
        dest_buf[curr_dest_buf_idx].data[curr_data_region_idx] = chunk[curr_chunk_idx];

        /*
         * On every iteration, we must increase the offset within the region, decrement a byte off
         * the chunk we have written from, and increase the chunl idx we are on.
         */
        curr_data_region_idx++;
        chunks_left--;
        curr_chunk_idx++;
    }

    /*
     * If we made it here, we have written all the bytes requested.
     */
    total_bytes_written = total_bytes_written + chunk_size;

    if (dest_buf.size() - 1 <= curr_dest_buf_idx &&
        (curr_data_region_idx >= dest_buf[curr_dest_buf_idx].capacity))
    {
        completely_filled = true;
    }

    return chunk_size;
}