#include "minitest.h"
#include "solution.h"

#include <chrono>
#include <cstdlib>
#include <thread>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - notify() records a completion event and wakes a waiter. It's
//     "sticky": a notify() that happens BEFORE wait() is not lost -- the
//     next wait() returns Completed immediately (a real interrupt can
//     fire before the driver starts waiting).
//   - Each notify() is consumed by one successful wait(): after a
//     Completed return, another wait() with no further notify() times out.
//   - wait(timeout) returns TimedOut once `timeout` elapses with no
//     event, and never returns much earlier than the timeout.
//   - WaitResult::Spurious (wakeup with no event) can't be provoked
//     through the public API, so it's not asserted; but a correct wait()
//     must never report Completed when no notify() happened.
//   - Timing margins are generous; a watchdog aborts the run if
//     something deadlocks.
// ---------------------------------------------------------------------

using namespace std::chrono_literals;
using Clock = std::chrono::steady_clock;

namespace
{
// Kills the process (instead of hanging CI forever) if a test deadlocks.
struct Watchdog
{
    Watchdog()
    {
        std::thread([] {
            std::this_thread::sleep_for(30s);
            std::cerr << "WATCHDOG: test run exceeded 30s -- likely deadlock\n";
            std::_Exit(2);
        }).detach();
    }
};
Watchdog watchdog;
} // namespace

TEST(wait_with_no_notify_times_out)
{
    CompletionSource src;
    CHECK(src.wait(20ms) == WaitResult::TimedOut);
}

TEST(timeout_actually_waits)
{
    CompletionSource src;
    auto start = Clock::now();
    CHECK(src.wait(80ms) == WaitResult::TimedOut);
    auto elapsed = Clock::now() - start;
    CHECK(elapsed >= 70ms);   // slack for clock granularity
    CHECK(elapsed < 2000ms);  // and didn't sleep wildly long
}

TEST(zero_timeout_returns_immediately)
{
    CompletionSource src;
    auto start = Clock::now();
    CHECK(src.wait(0ms) == WaitResult::TimedOut);
    CHECK(Clock::now() - start < 500ms);
}

TEST(notify_before_wait_is_not_lost)
{
    CompletionSource src;
    src.notify();
    auto start = Clock::now();
    CHECK(src.wait(5000ms) == WaitResult::Completed);
    CHECK(Clock::now() - start < 1000ms); // returned promptly, not after the timeout
}

TEST(event_is_consumed_by_a_completed_wait)
{
    CompletionSource src;
    src.notify();
    CHECK(src.wait(1000ms) == WaitResult::Completed);
    CHECK(src.wait(20ms) == WaitResult::TimedOut);
}

TEST(notify_from_another_thread_wakes_the_waiter_early)
{
    CompletionSource src;
    std::thread irq([&] {
        std::this_thread::sleep_for(50ms);
        src.notify();
    });

    auto start = Clock::now();
    WaitResult r = src.wait(5000ms);
    auto elapsed = Clock::now() - start;
    irq.join();

    CHECK(r == WaitResult::Completed);
    CHECK(elapsed < 2000ms); // woke on notify, not on timeout
}

TEST(notify_arriving_after_timeout_is_seen_by_the_next_wait)
{
    CompletionSource src;
    CHECK(src.wait(10ms) == WaitResult::TimedOut);
    src.notify();
    CHECK(src.wait(1000ms) == WaitResult::Completed);
}

TEST(repeated_notify_wait_cycles)
{
    CompletionSource src;
    for (int i = 0; i < 100; ++i)
    {
        src.notify();
        CHECK(src.wait(1000ms) == WaitResult::Completed);
    }
    CHECK(src.wait(10ms) == WaitResult::TimedOut);
}

TEST(ping_pong_between_two_threads)
{
    CompletionSource to_device, to_host;
    std::thread device([&] {
        for (int i = 0; i < 50; ++i)
        {
            if (to_device.wait(5000ms) != WaitResult::Completed)
                return;
            to_host.notify();
        }
    });

    int completed = 0;
    for (int i = 0; i < 50; ++i)
    {
        to_device.notify();
        if (to_host.wait(5000ms) == WaitResult::Completed)
            ++completed;
    }
    device.join();
    CHECK_EQ(completed, 50);
}

TEST(wait_never_reports_completed_without_a_notify)
{
    CompletionSource src;
    for (int i = 0; i < 20; ++i)
        CHECK(src.wait(1ms) != WaitResult::Completed);
}

TEST_MAIN()
