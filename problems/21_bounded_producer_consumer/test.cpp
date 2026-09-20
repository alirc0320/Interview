#include "minitest.h"
#include "solution.h"

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - push() blocks while the queue holds `capacity` items; pop() blocks
//     while it is empty. FIFO order.
//   - shutdown() wakes every blocked thread. After shutdown, pop() keeps
//     returning the items that were already queued (drain), then returns
//     std::nullopt forever. push() after / during shutdown must not
//     block; whether such a value is kept or dropped isn't pinned down.
//   - shutdown() is idempotent.
//
// Blocking behaviour is checked with short sleeps, so margins are
// generous; a watchdog aborts the run if something deadlocks.
// ---------------------------------------------------------------------

using namespace std::chrono_literals;

namespace
{
struct Watchdog
{
    Watchdog()
    {
        std::thread(
            []
            {
                std::this_thread::sleep_for(30s);
                std::cerr << "WATCHDOG: test run exceeded 30s -- likely deadlock\n";
                std::_Exit(2);
            })
            .detach();
    }
};
Watchdog watchdog;
} // namespace

TEST(fifo_order_single_thread)
{
    BoundedQueue<int> q(4);
    q.push(1);
    q.push(2);
    q.push(3);
    auto a = q.pop();
    auto b = q.pop();
    auto c = q.pop();
    CHECK(a && *a == 1);
    CHECK(b && *b == 2);
    CHECK(c && *c == 3);
}

TEST(can_fill_to_exactly_capacity_without_blocking)
{
    BoundedQueue<int> q(3);
    q.push(1);
    q.push(2);
    q.push(3); // would deadlock this thread if capacity were off by one
    CHECK(q.pop().value_or(-1) == 1);
}

TEST(pop_after_shutdown_on_empty_queue_returns_nullopt)
{
    BoundedQueue<int> q(2);
    q.shutdown();
    CHECK(!q.pop().has_value());
    CHECK(!q.pop().has_value());
}

TEST(shutdown_drains_remaining_items_before_returning_nullopt)
{
    BoundedQueue<int> q(4);
    q.push(10);
    q.push(20);
    q.shutdown();
    auto a = q.pop();
    auto b = q.pop();
    CHECK(a && *a == 10);
    CHECK(b && *b == 20);
    CHECK(!q.pop().has_value());
    CHECK(!q.pop().has_value());
}

TEST(shutdown_is_idempotent)
{
    BoundedQueue<int> q(2);
    q.push(1);
    q.shutdown();
    q.shutdown();
    q.shutdown();
    CHECK(q.pop().value_or(-1) == 1);
    CHECK(!q.pop().has_value());
}

TEST(blocked_consumer_is_woken_by_push)
{
    BoundedQueue<int> q(2);
    std::atomic<int> got{-1};
    std::thread consumer([&] { got = q.pop().value_or(-2); });

    std::this_thread::sleep_for(50ms);
    CHECK_EQ(got.load(), -1); // still blocked, nothing to pop
    q.push(99);
    consumer.join();
    CHECK_EQ(got.load(), 99);
}

TEST(blocked_producer_is_woken_by_pop)
{
    BoundedQueue<int> q(1);
    q.push(1);
    std::atomic<bool> pushed{false};
    std::thread producer(
        [&]
        {
            q.push(2); // blocks: queue is full
            pushed = true;
        });

    std::this_thread::sleep_for(50ms);
    CHECK(!pushed.load()); // still blocked
    CHECK(q.pop().value_or(-1) == 1);
    producer.join();
    CHECK(pushed.load());
    CHECK(q.pop().value_or(-1) == 2);
}

TEST(producer_never_gets_more_than_capacity_ahead_of_the_consumer)
{
    constexpr std::size_t kCap = 3;
    BoundedQueue<int> q(kCap);
    std::atomic<int> pushed{0};
    std::thread producer(
        [&]
        {
            for (int i = 0; i < 10; ++i)
            {
                q.push(i);
                ++pushed;
            }
        });

    std::this_thread::sleep_for(100ms);
    CHECK(pushed.load() <= static_cast<int>(kCap)); // nobody is popping yet

    for (int i = 0; i < 10; ++i)
        CHECK(q.pop().value_or(-1) == i);
    producer.join();
}

TEST(shutdown_wakes_blocked_consumers)
{
    BoundedQueue<int> q(2);
    std::atomic<int> nullopts{0};
    std::vector<std::thread> consumers;
    for (int i = 0; i < 4; ++i)
        consumers.emplace_back(
            [&]
            {
                if (!q.pop().has_value())
                    ++nullopts;
            });

    std::this_thread::sleep_for(50ms);
    q.shutdown();
    for (auto& t : consumers)
        t.join();
    CHECK_EQ(nullopts.load(), 4);
}

TEST(shutdown_wakes_blocked_producers)
{
    BoundedQueue<int> q(1);
    q.push(1);
    std::atomic<int> returned{0};
    std::vector<std::thread> producers;
    for (int i = 0; i < 3; ++i)
        producers.emplace_back(
            [&, i]
            {
                q.push(100 + i); // all block: queue is full
                ++returned;
            });

    std::this_thread::sleep_for(50ms);
    CHECK_EQ(returned.load(), 0);
    q.shutdown();
    for (auto& t : producers)
        t.join(); // hangs (-> watchdog) if shutdown doesn't release producers
    CHECK_EQ(returned.load(), 3);
}

TEST(push_after_shutdown_does_not_block)
{
    BoundedQueue<int> q(1);
    q.shutdown();
    q.push(1);
    q.push(2);
    q.push(3);
    CHECK(true); // reaching here means push() returned
}

TEST(one_producer_one_consumer_transfers_everything_in_order)
{
    BoundedQueue<int> q(4);
    constexpr int kN = 5000;
    std::vector<int> received;

    std::thread consumer(
        [&]
        {
            while (auto v = q.pop())
                received.push_back(*v);
        });
    std::thread producer(
        [&]
        {
            for (int i = 0; i < kN; ++i)
                q.push(i);
            q.shutdown();
        });
    producer.join();
    consumer.join();

    CHECK_EQ(received.size(), static_cast<std::size_t>(kN));
    bool in_order = true;
    for (int i = 0; i < static_cast<int>(received.size()); ++i)
        in_order = in_order && received[static_cast<std::size_t>(i)] == i;
    CHECK(in_order);
}

TEST(many_producers_many_consumers_lose_and_duplicate_nothing)
{
    BoundedQueue<int> q(8);
    constexpr int kProducers = 4;
    constexpr int kPerProducer = 2000;
    constexpr int kConsumers = 3;

    std::atomic<long long> sum{0};
    std::atomic<int> count{0};

    std::vector<std::thread> consumers;
    for (int c = 0; c < kConsumers; ++c)
        consumers.emplace_back(
            [&]
            {
                while (auto v = q.pop())
                {
                    sum += *v;
                    ++count;
                }
            });

    std::vector<std::thread> producers;
    for (int p = 0; p < kProducers; ++p)
        producers.emplace_back(
            [&, p]
            {
                for (int i = 0; i < kPerProducer; ++i)
                    q.push(p * kPerProducer + i);
            });

    for (auto& t : producers)
        t.join();
    q.shutdown();
    for (auto& t : consumers)
        t.join();

    const long long n = kProducers * kPerProducer;
    CHECK_EQ(count.load(), static_cast<int>(n));
    CHECK_EQ(sum.load(), n * (n - 1) / 2);
}

TEST(works_with_move_only_and_non_trivial_types)
{
    BoundedQueue<std::unique_ptr<std::string>> q(2);
    q.push(std::make_unique<std::string>("hello"));
    q.push(std::make_unique<std::string>("world"));
    auto a = q.pop();
    auto b = q.pop();
    CHECK(a && *a && **a == "hello");
    CHECK(b && *b && **b == "world");
}

TEST_MAIN()
