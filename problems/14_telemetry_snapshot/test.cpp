#include "minitest.h"
#include "solution.h"

#include <atomic>
#include <cstdint>
#include <thread>

// ---------------------------------------------------------------------
// The invariant used to detect a torn snapshot: every update() writes a
// Telemetry whose fields are all derived from one counter `i`:
//     temperature_c = i,  power_mw = 2 * i,  uptime_ms = i
// A snapshot() that ever returns fields from two different updates
// violates power_mw == 2 * temperature_c or uptime_ms == temperature_c.
//
// Assumed contract: single writer thread, any number of reader threads;
// a fresh store's snapshot has all-zero fields (which satisfies the
// invariant above).
// ---------------------------------------------------------------------

namespace
{

Telemetry make(std::uint32_t i)
{
    Telemetry t{};
    t.temperature_c = i;
    t.power_mw = 2 * i;
    t.uptime_ms = i;
    return t;
}

bool consistent(const Telemetry& t)
{
    return t.power_mw == 2 * t.temperature_c && t.uptime_ms == t.temperature_c;
}

} // namespace

TEST(fresh_store_snapshot_is_zeroed_and_consistent)
{
    TelemetryStore store;
    Telemetry t = store.snapshot();
    CHECK_EQ(t.temperature_c, 0u);
    CHECK_EQ(t.power_mw, 0u);
    CHECK_EQ(t.uptime_ms, static_cast<std::uint64_t>(0));
}

TEST(snapshot_returns_last_update)
{
    TelemetryStore store;
    Telemetry t{};
    t.temperature_c = 55;
    t.power_mw = 12345;
    t.uptime_ms = 9876543210ull;
    store.update(t);

    Telemetry s = store.snapshot();
    CHECK_EQ(s.temperature_c, 55u);
    CHECK_EQ(s.power_mw, 12345u);
    CHECK_EQ(s.uptime_ms, static_cast<std::uint64_t>(9876543210ull));
}

TEST(later_update_replaces_earlier_one)
{
    TelemetryStore store;
    store.update(make(1));
    store.update(make(2));
    store.update(make(3));
    Telemetry s = store.snapshot();
    CHECK_EQ(s.temperature_c, 3u);
    CHECK_EQ(s.power_mw, 6u);
}

TEST(snapshot_does_not_consume_the_value)
{
    TelemetryStore store;
    store.update(make(7));
    Telemetry a = store.snapshot();
    Telemetry b = store.snapshot();
    CHECK_EQ(a.temperature_c, 7u);
    CHECK_EQ(b.temperature_c, 7u);
}

TEST(snapshot_is_a_copy_not_a_live_view)
{
    TelemetryStore store;
    store.update(make(10));
    Telemetry before = store.snapshot();
    store.update(make(20));
    CHECK_EQ(before.temperature_c, 10u); // earlier snapshot unaffected by later update
    CHECK_EQ(store.snapshot().temperature_c, 20u);
}

TEST(concurrent_reader_never_sees_a_torn_snapshot)
{
    TelemetryStore store;
    store.update(make(0));

    std::atomic<bool> stop{false};
    std::atomic<int> torn{0};
    std::atomic<std::uint64_t> reads{0};

    auto reader = [&]()
    {
        while (!stop.load(std::memory_order_relaxed))
        {
            Telemetry t = store.snapshot();
            if (!consistent(t))
                torn.fetch_add(1, std::memory_order_relaxed);
            reads.fetch_add(1, std::memory_order_relaxed);
        }
    };

    std::thread r1(reader);
    std::thread r2(reader);

    std::thread writer(
        [&]()
        {
            for (std::uint32_t i = 1; i <= 200000; ++i)
                store.update(make(i));
            stop.store(true);
        });

    writer.join();
    r1.join();
    r2.join();

    CHECK_EQ(torn.load(), 0);
    CHECK(reads.load() > 0);

    // After the writer finishes, the final value is visible and consistent.
    Telemetry last = store.snapshot();
    CHECK_EQ(last.temperature_c, 200000u);
    CHECK(consistent(last));
}

TEST(snapshots_never_go_backwards_in_time)
{
    TelemetryStore store;
    store.update(make(0));

    std::atomic<bool> stop{false};
    std::atomic<int> regressions{0};

    std::thread reader(
        [&]()
        {
            std::uint32_t last_seen = 0;
            while (!stop.load(std::memory_order_relaxed))
            {
                Telemetry t = store.snapshot();
                if (t.temperature_c < last_seen)
                    regressions.fetch_add(1, std::memory_order_relaxed);
                last_seen = t.temperature_c;
            }
        });

    std::thread writer(
        [&]()
        {
            for (std::uint32_t i = 1; i <= 100000; ++i)
                store.update(make(i));
            stop.store(true);
        });

    writer.join();
    reader.join();
    CHECK_EQ(regressions.load(), 0);
}

TEST_MAIN()
