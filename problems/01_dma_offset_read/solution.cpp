#include "solution.h"
#include <array>
#include <cmath>
#include <vector>

// TODO: implement the declarations from solution.h.

using namespace std;

/**
 * Must accept arbitrary offset/size (not just 4096-aligned ones), issue an aligned dma_read into
 * temporary storage, and copy only the requested bytes into dst. Handle zero size, null pointers,
 * overflow, allocation failure, and DMA failure.
 */
bool read_bytes(void* dst, std::size_t size, std::size_t offset)
{
    if(dst == NULL)
    {
        return false;
    }
    
    if(offset > std::numeric_limits<std::size_t>::max() - size)
    {
        return false;
    }

    /*
     * example:
     * 8000 - 13000 (~5000 byte read)
     * In reality you read from these bounds using dma_read api
     * 4096 - 16,384
     */
    size_t aligned_start = (offset / 4096) * 4096;
    size_t aligned_end = ((size + offset + 4096 - 1) / 4096) * 4096;
    size_t aligned_size = aligned_end - aligned_start;
    std::vector<uint8_t> temp_buffer(aligned_size);

    if(!(dma_read(temp_buffer.data(), aligned_size, aligned_start)))
    {
        return false;
    }

    memcpy(dst, temp_buffer.data() + (offset - aligned_start), size);

    return true;
}