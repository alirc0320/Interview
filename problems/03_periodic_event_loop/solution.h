#pragma once
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <unordered_map>

using namespace std;

/*
 * Periodic Event Loop
 * ====================
 *
 * As asked in an Etched onsite coding screen (Glassdoor, Software
 * Engineer, San Jose, reported Jun 2026), verbatim:
 *
 *   "Question was to design an event loop where events are registered
 *   anytime. Events must be executed periodically according to the
 *   event config. Each event has an exec function that can return a
 *   bool to indicate event execution must proceed periodically."
 *
 * Concretely: design a single-threaded event loop that runs scheduled
 * callbacks in order of their next deadline -- the same idea behind
 * setInterval / setTimeout style schedulers (e.g. Node.js timers,
 * Python asyncio's call_later, Tornado's PeriodicCallback).
 *
 * add_event(period, callback)
 *   - Schedules `callback` to run once `period` has elapsed from the
 *     moment it was added.
 *   - Returns a unique EventId that can later be passed to cancel().
 *
 * cancel(id)
 *   - Prevents the event with the given id from running again.
 *   - Cancelling an id that doesn't exist, has already finished, or
 *     gets cancelled twice, is a harmless no-op.
 *
 * run()
 *   - Blocks the calling thread, executing due callbacks strictly in
 *     order of their next deadline, until no events remain.
 *   - A callback returning `true` is periodic: it is rescheduled
 *     `period` after its *previous* deadline (not after whatever time
 *     it happened to actually run), so its cadence doesn't drift.
 *   - A callback returning `false` is one-shot and is not rescheduled.
 *   - Callbacks may call add_event()/cancel() on the loop while they
 *     run, including cancelling themselves or another event.
 *
 * Example 1 -- ordering is by deadline, not by insertion order:
 *
 *     loop.add_event(60ms, []{ record('C'); return false; });
 *     loop.add_event(20ms, []{ record('A'); return false; });
 *     loop.add_event(40ms, []{ record('B'); return false; });
 *     loop.run();
 *     // recorded order: A, B, C   (20ms, 40ms, 60ms, not insertion order)
 *
 * Example 2 -- a periodic and a one-shot event interleaving:
 *
 *     // A fires every 20ms and stops itself after its 3rd run.
 *     // B fires once, at 50ms.
 *     loop.add_event(20ms, []{ record('A'); return ++a_count < 3; });
 *     loop.add_event(50ms, []{ record('B'); return false; });
 *     loop.run();
 *     // recorded order: A(20ms), A(40ms), B(50ms), A(60ms) -> "AABA"
 */

using EventId = std::uint64_t;
using Duration = std::chrono::milliseconds;
using Period = std::chrono::milliseconds;
using Timestamp = std::chrono::system_clock::time_point;
using Elapsed = std::chrono::milliseconds;
using Callback = std::function<bool()>; // return true to reschedule

class EventLoop
{
  public:
    EventId add_event(Duration period, Callback callback);
    void cancel(EventId id);
    void run();
    EventId GenerateID();

  private:
    // TODO
    /*
     * Priority Queue of tuples
     */
    using EventTuple = tuple<Timestamp, Period, EventId>;
    using EventPair = pair<bool, Callback>;

    priority_queue<EventTuple, std::vector<EventTuple>, std::greater<EventTuple>> events;
    std::unordered_map<EventId, EventPair> event_valid;

    EventId ID = 0;
};
