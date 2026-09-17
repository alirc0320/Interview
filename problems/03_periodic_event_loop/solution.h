#pragma once
#include <chrono>
#include <cstdint>
#include <functional>

// Duration/EventId types were not pinned down in the prompt --
// adjust as you like.
using EventId = std::uint64_t;
using Duration = std::chrono::milliseconds;
using Callback = std::function<bool()>; // return true to reschedule

// TODO: implement. Events run in next-deadline order. Consider what
// data structure gives you fast "next deadline" lookup, how
// cancellation interacts with it, and whether the next deadline is
// based on the previous deadline or the current time.
class EventLoop
{
  public:
    EventId add_event(Duration period, Callback callback);
    void cancel(EventId id);
    void run();

  private:
    // TODO
};
