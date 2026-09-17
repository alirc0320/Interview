#pragma once
#include <cstddef>
#include <optional>

// TODO: implement. Bitmap-based allocator over a contiguous
// device-memory aperture divided into 4 KiB pages. Detect
// double-free and invalid ranges.
class PageAllocator
{
  public:
    explicit PageAllocator(std::size_t total_pages);

    // Returns the starting page index of a contiguous run, or
    // std::nullopt if allocation fails.
    std::optional<std::size_t> allocate(std::size_t num_pages);

    // Returns false on double-free or an invalid range.
    bool free(std::size_t start_page, std::size_t num_pages);

  private:
    // TODO
};
