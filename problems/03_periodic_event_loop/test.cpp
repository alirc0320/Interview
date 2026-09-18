#include "minitest.h"
#include "solution.h"

#include <chrono>
#include <string>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (the problem statement leaves these unspecified --
// adjust these tests if you design EventLoop differently on purpose):
//   - run() blocks, executing events in next-deadline order, and returns
//     once no events remain (all returned false or were cancelled).
//   - A callback returning true is rescheduled `period` after its
//     previous deadline; returning false removes it.
//   - add_event() returns a unique id; cancel() of an unknown or already
//     finished id is a harmless no-op.
//   - Callbacks may call add_event()/cancel() on the loop re-entrantly.
//
// These tests use real (short) sleeps, so timing margins are generous.
// ---------------------------------------------------------------------

using namespace std::chrono_literals;
using Clock = std::chrono::steady_clock;

TEST(run_with_no_events_returns_immediately)
{
    EventLoop loop;
    loop.run();
    CHECK(true); // reaching here means run() didn't hang
}

TEST(one_shot_event_runs_exactly_once)
{
    EventLoop loop;
    int count = 0;
    loop.add_event(5ms, [&]() {
        ++count;
        return false;
    });
    loop.run();
    CHECK_EQ(count, 1);
}

TEST(periodic_event_repeats_until_it_returns_false)
{
    EventLoop loop;
    int count = 0;
    loop.add_event(5ms, [&]() { return ++count < 4; });
    loop.run();
    CHECK_EQ(count, 4);
}

TEST(event_does_not_fire_before_its_period_elapses)
{
    EventLoop loop;
    auto start = Clock::now();
    Clock::time_point fired_at{};
    loop.add_event(50ms, [&]() {
        fired_at = Clock::now();
        return false;
    });
    loop.run();
    CHECK(fired_at - start >= 45ms); // small slack for clock granularity
}

TEST(periodic_event_keeps_its_cadence)
{
    EventLoop loop;
    auto start = Clock::now();
    int count = 0;
    loop.add_event(20ms, [&]() { return ++count < 5; });
    loop.run();
    auto elapsed = Clock::now() - start;
    CHECK_EQ(count, 5);
    CHECK(elapsed >= 95ms);  // 5 * 20ms
    CHECK(elapsed < 500ms);  // generous upper bound, catches busy-wrong scheduling
}

TEST(events_run_in_deadline_order)
{
    EventLoop loop;
    std::string order;
    // Registered in the "wrong" order on purpose: the 60ms event is added
    // first but must run last.
    loop.add_event(60ms, [&]() {
        order += 'C';
        return false;
    });
    loop.add_event(20ms, [&]() {
        order += 'A';
        return false;
    });
    loop.add_event(40ms, [&]() {
        order += 'B';
        return false;
    });
    loop.run();
    CHECK(order == "ABC");
}

TEST(periodic_events_interleave_by_deadline)
{
    EventLoop loop;
    std::string order;
    int a = 0;
    // A fires at 20, 40, 60. B fires once at 50. Expected: A A B A.
    loop.add_event(20ms, [&]() {
        order += 'A';
        return ++a < 3;
    });
    loop.add_event(50ms, [&]() {
        order += 'B';
        return false;
    });
    loop.run();
    CHECK(order == "AABA");
}

TEST(event_ids_are_unique)
{
    EventLoop loop;
    EventId a = loop.add_event(1ms, []() { return false; });
    EventId b = loop.add_event(1ms, []() { return false; });
    EventId c = loop.add_event(1ms, []() { return false; });
    CHECK(a != b);
    CHECK(b != c);
    CHECK(a != c);
    loop.run();
}

TEST(cancel_before_run_prevents_execution)
{
    EventLoop loop;
    int cancelled_runs = 0;
    int kept_runs = 0;
    EventId id = loop.add_event(5ms, [&]() {
        ++cancelled_runs;
        return false;
    });
    loop.add_event(10ms, [&]() {
        ++kept_runs;
        return false;
    });
    loop.cancel(id);
    loop.run();
    CHECK_EQ(cancelled_runs, 0);
    CHECK_EQ(kept_runs, 1);
}

TEST(cancel_unknown_id_is_a_noop)
{
    EventLoop loop;
    int count = 0;
    loop.add_event(5ms, [&]() {
        ++count;
        return false;
    });
    loop.cancel(987654321);
    loop.run();
    CHECK_EQ(count, 1);
}

TEST(cancel_same_id_twice_is_harmless)
{
    EventLoop loop;
    int count = 0;
    EventId id = loop.add_event(5ms, [&]() {
        ++count;
        return false;
    });
    loop.cancel(id);
    loop.cancel(id);
    loop.run();
    CHECK_EQ(count, 0);
}

TEST(event_can_cancel_another_event_from_its_callback)
{
    EventLoop loop;
    int victim_runs = 0;
    EventId victim = loop.add_event(60ms, [&]() {
        ++victim_runs;
        return false;
    });
    loop.add_event(10ms, [&]() {
        loop.cancel(victim);
        return false;
    });
    loop.run();
    CHECK_EQ(victim_runs, 0);
}

TEST(event_can_cancel_itself_while_returning_true)
{
    EventLoop loop;
    int count = 0;
    EventId self = 0;
    self = loop.add_event(5ms, [&]() {
        ++count;
        loop.cancel(self);
        return true; // asked to reschedule, but was cancelled -> must not run again
    });
    loop.run();
    CHECK_EQ(count, 1);
}

TEST(cancelling_a_periodic_event_stops_it_mid_stream)
{
    EventLoop loop;
    int periodic_runs = 0;
    EventId periodic = loop.add_event(10ms, [&]() {
        ++periodic_runs;
        return true; // would run forever if never cancelled
    });
    loop.add_event(55ms, [&]() {
        loop.cancel(periodic);
        return false;
    });
    loop.run();
    CHECK(periodic_runs >= 3);
    CHECK(periodic_runs <= 6);
}

TEST(callback_can_add_a_new_event)
{
    EventLoop loop;
    std::string order;
    loop.add_event(10ms, [&]() {
        order += 'A';
        loop.add_event(10ms, [&]() {
            order += 'B';
            return false;
        });
        return false;
    });
    loop.run();
    CHECK(order == "AB");
}

TEST(many_events_all_run)
{
    EventLoop loop;
    int total = 0;
    for (int i = 0; i < 200; ++i)
    {
        loop.add_event(std::chrono::milliseconds(1 + i % 5), [&]() {
            ++total;
            return false;
        });
    }
    loop.run();
    CHECK_EQ(total, 200);
}

TEST_MAIN()
