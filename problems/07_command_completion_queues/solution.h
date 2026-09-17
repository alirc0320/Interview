#pragma once
#include <cstddef>
#include <cstdint>
#include <optional>

using CommandId = std::uint64_t;

struct Command
{
    CommandId id;
    // TODO: add payload fields
};

struct Completion
{
    CommandId id;
    // TODO: add result fields
};

// TODO: implement submission, device processing, completion
// retrieval, queue-full behavior, and monotonically increasing
// command IDs.
class CommandQueue
{
  public:
    explicit CommandQueue(std::size_t capacity);

    std::optional<CommandId> submit(Command cmd);
    void process_one(); // simulates the device processing one command
    std::optional<Completion> poll_completion();

  private:
    // TODO
};
