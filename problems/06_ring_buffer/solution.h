#pragma once
#include <cstddef>
#include <optional>

// TODO: implement. Fixed-capacity ring buffer -- do not use
// std::queue. Clearly define full/empty behavior and handle index
// wraparound. (This is a template, so definitions normally live in
// this header, not in solution.cpp.)
template <typename T> class RingBuffer
{
  public:
    explicit RingBuffer(std::size_t capacity);

    bool push(const T& value);
    std::optional<T> pop();
    const T& front() const;
    std::size_t size() const;
    bool empty() const;
    bool full() const;

  private:
    // TODO
};
