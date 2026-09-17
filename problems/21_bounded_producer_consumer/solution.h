#pragma once
#include <cstddef>
#include <optional>

// TODO: implement using std::mutex and std::condition_variable.
// Blocking push/pop, plus shutdown() so blocked threads exit
// cleanly. (This is a template, so definitions normally live in this
// header, not in solution.cpp.)
template <typename T> class BoundedQueue
{
  public:
    explicit BoundedQueue(std::size_t capacity);

    void push(T value);
    std::optional<T> pop(); // returns std::nullopt after shutdown + drained
    void shutdown();

  private:
    // TODO
};
