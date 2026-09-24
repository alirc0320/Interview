#include "solution.h"

// TODO: implement the declarations from solution.h.

/**
 * Solution:
 *
 * GenerateID:
 * Increment a global uint64 Static ID and return a rvalue.
 *
 * add_event:
 * should use a priority queue of TUPLES, {Timestamp, Period, Callback, EventId}
 * The ID should be a uint64 that is incremented every time an event is added.
 * return the Event ID.
 *
 * cancel:
 * Global vector or map of IDs. Each Entry should contain a True/Valid or 1,
 * then marked False/Invalid or 0 upon cancel. Data structure should be checked
 * in the "run" function before running the callbacks from the queue.
 *
 * run:
 */

/**
 * Increment the global ID Number. This is unique to each object
 * instance and should never overflow
 * unless there are over 18 quintillion events.
 */
EventId EventLoop::GenerateID()
{
    return ID++;
}

/**
 * Schedules `callback` to run once `period` has elapsed from the moment it was added.
 * Returns a unique EventId that can later be passed to cancel().
 */
EventId EventLoop::add_event(Duration period, Callback callback)
{
    /*
     * Format of tuple in queue: <Timestamp, Period, EventId>
     */

    /*
     * Calculate Scheduled Time and Push That on the Queue.
     * The Min Heap will be sorted by this field.
     */
    auto scheduled_time = std::chrono::system_clock::now() + period;

    /*
     * Generate the Event ID.
     */
    EventId event_id = GenerateID();

    /*
     * Push onto Queue and adjust map so we can reference it using the ID to invoke the callback
     */
    events.push({scheduled_time, period, event_id});
    event_valid.insert({event_id, {true, callback}});

    return event_id;
}

/**
 * Prevents the event with the given id from running again.
 *
 * Cancelling an id that doesn't exist, has already finished, or gets cancelled twice, is a harmless
 * no-op.
 */
void EventLoop::cancel(EventId id)
{
    if (event_valid.find(id) != event_valid.end())
    {
        event_valid[id].first = false;
    }

    return;
}

/**
 * Blocks the calling thread, executing due callbacks strictly in
 * order of their next deadline, until no events remain.
 *
 * A callback returning `true` is periodic: it is rescheduled
 * `period` after its *previous* deadline (not after whatever time
 * it happened to actually run), so its cadence doesn't drift.
 *
 * A callback returning `false` is one-shot and is not rescheduled.
 *
 * Callbacks may call add_event()/cancel() on the loop while they
 * run, including cancelling themselves or another event.
 */
void EventLoop::run()
{
    while (true)
    {
        if (!events.empty())
        {

            auto event_tuple = events.top();

            // <Timestamp, Period, EventId>
            auto curr_event_time_stamp = std::get<0>(event_tuple);
            auto now = std::chrono::system_clock::now();
            bool event_time_elapsed = (now >= curr_event_time_stamp) ? true : false;

            //  <Timestamp, Period, EventId>
            // event id
            EventId event_id = std::get<2>(event_tuple);
            bool is_valid = event_valid[event_id].first;

            if (event_time_elapsed && is_valid)
            {

                events.pop();

                // call_back
                bool run_again = event_valid[event_id].second();

                if (run_again)
                {
                    // <Timestamp, Period, EventId>
                    Duration period = get<1>(event_tuple);
                    auto timestamp = get<0>(event_tuple);
                    get<0>(event_tuple) = timestamp + period;
                    events.push(event_tuple);
                }
                else
                {
                    event_valid.erase(event_id);
                }
            }
            else if (!is_valid)
            {
                events.pop();
                event_valid.erase(event_id);
                continue;
            }
            else
            {
                continue;
            }
        }
        else
        {
            return;
        }
    };

    return;
}