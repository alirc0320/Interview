#pragma once
#include <cstddef>

// TODO: implement. Aligned bump allocation over a caller-owned
// buffer, plus reset(); reject invalid alignment and arithmetic
// overflow.
class Arena
{
  public:
    Arena(void* buffer, std::size_t size);

    void* allocate(std::size_t size, std::size_t alignment);
    void reset();

  private:
    // TODO
};
