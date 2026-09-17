#pragma once
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

struct VfResources
{
    std::uint32_t vf_id;
    std::vector<std::uint32_t> queue_ids;
    std::size_t memory_window_offset;
    std::size_t memory_window_size;
};

// TODO: implement. Allocate/release queue IDs and device-memory
// windows per virtual function; one VF must never be able to access
// another's resources.
class SriovResourceManager
{
  public:
    SriovResourceManager(std::size_t num_queues, std::size_t total_memory);

    std::optional<VfResources> assign(std::uint32_t vf_id, std::size_t num_queues,
                                      std::size_t memory_size);
    bool release(std::uint32_t vf_id);

  private:
    // TODO
};
