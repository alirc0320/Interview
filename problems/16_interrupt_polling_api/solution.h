#pragma once
#include <chrono>

enum class WaitResult
{
    Completed,
    TimedOut,
    Spurious,
};

// TODO: implement a common wait API usable by both interrupt-driven
// and polling-driven completion, with timeout and spurious-wakeup
// handling.
class CompletionSource
{
  public:
    void notify(); // called from an "interrupt" context
    WaitResult wait(std::chrono::milliseconds timeout);

  private:
    // TODO
};
