#pragma once
#include <cstddef>
#include <cstdint>

// TODO: implement. Chunks may arrive duplicated or out of order;
// validate offset/length, track received regions, reject conflicting
// duplicates, and declare completion only when the full image is
// present.
class FirmwareReceiver
{
  public:
    explicit FirmwareReceiver(std::size_t image_size);

    bool receive_chunk(std::size_t offset, const std::uint8_t* data, std::size_t length);
    bool is_complete() const;

  private:
    // TODO
};
