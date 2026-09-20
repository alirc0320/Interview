#pragma once
#include <cstddef>
#include <optional>
#include <vector>

/*
 * TODO: implement. Fixed-capacity ring buffer. Do not use
 * std::queue. Clearly define full/empty behavior and handle index
 * wraparound. (This is a template, so definitions normally live in
 * this header, not in solution.cpp.)
 */
template <typename T> class RingBuffer
{
  public:
    /*
     * Empty buffer with room for _capacity elements.
     */
    explicit RingBuffer(std::size_t _capacity) : capacity(_capacity)
    {
        head = 0;
        tail = 0;
        count = 0;
        ring_buffer.resize(capacity);
    };

    /*
     * Add to the back. Return false (no overwrite) if full.
     */
    bool push(const T& value)
    {
        if (!full())
        {
            ring_buffer[tail] = value;
            tail = (tail + 1) % capacity;
            count++;
            return true;
        }
        else
        {
            return false;
        }
    }

    /*
     * Remove and return the oldest element, or nullopt if empty.
     */
    std::optional<T> pop()
    {
        if (!empty())
        {
            T curr_val = ring_buffer[head];
            head = (head + 1) % capacity;
            count--;
            return curr_val;
        }
        else
        {
            return std::nullopt;
        }
    }

    /*
     * Oldest element, without removing it.
     */
    const T& front() const
    {
        return ring_buffer[head];
    }

    /*
     * Number of stored elements.
     */
    size_t size() const
    {
        return count;
    }

    /*
     * True if no elements.
     */
    bool empty() const
    {
        return count == 0 ? true : false;
    }

    /*
     * True if at capacity.
     */
    bool full() const
    {
        return capacity == count ? true : false;
    }

  private:
    size_t count;
    size_t head;
    size_t tail;
    size_t capacity;

    std::vector<T> ring_buffer;
};
