#pragma once
#include <cstddef>

// Low-level primitive: reads `size` bytes at `offset` into `dst`.
// size and offset must both be multiples of 4096. Assumed to be
// provided elsewhere (e.g. by hardware, or a mock you write in
// test.cpp) -- do not implement it here.
bool dma_read(void* dst, std::size_t size, std::size_t offset);

// TODO: implement. Must accept arbitrary offset/size (not just
// 4096-aligned ones), issue an aligned dma_read into temporary
// storage, and copy only the requested bytes into dst. Handle zero
// size, null pointers, overflow, allocation failure, and DMA failure.
bool read_bytes(void* dst, std::size_t size, std::size_t offset);
