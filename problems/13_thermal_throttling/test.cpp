#include "minitest.h"
#include "solution.h"

#include <chrono>
#include <cmath>
#include <limits>

// ---------------------------------------------------------------------
// Thresholds used throughout: warn 70C, throttle 85C, shutdown 100C,
// recover 60C.
//
// Assumed contract (the guide only says "use hysteresis"; adjust these
// tests if you design it differently on purpose):
//   - Starts in Normal.
//   - A reading >= warn (and < throttle) -> Warning; >= throttle ->
//     Throttled; >= shutdown -> Shutdown.
//   - Hysteresis: once out of Normal, the controller only returns to
//     Normal after a reading at or below `recover_c` -- cooling to just
//     under a trigger threshold must NOT immediately clear it.
//   - Samples with a NaN temperature, or a timestamp older than the
//     previous accepted sample ("stale"), are rejected: on_sample()
//     returns the unchanged current state and the sample has no effect.
//   - on_sample() returns the new state, equal to state() afterwards.
//   - Not pinned down (so not tested): whether Shutdown is latched, and
//     whether cooling steps down one level at a time or drops straight to
//     Normal only at recover_c (we only assert the endpoint at recover_c).
// ---------------------------------------------------------------------

using Clock = std::chrono::steady_clock;
using namespace std::chrono_literals;

namespace
{

const Clock::time_point kT0 = Clock::time_point{} + 1000s;

struct Fixture
{
    ThermalController ctl{70.0, 85.0, 100.0, 60.0};
    int seconds = 0;

    ThermalState feed(double temp_c)
    {
        return ctl.on_sample(Sample{kT0 + std::chrono::seconds(seconds++), temp_c});
    }
};

} // namespace

TEST(starts_normal)
{
    Fixture f;
    CHECK(f.ctl.state() == ThermalState::Normal);
}

TEST(cool_readings_stay_normal)
{
    Fixture f;
    CHECK(f.feed(25.0) == ThermalState::Normal);
    CHECK(f.feed(50.0) == ThermalState::Normal);
    CHECK(f.feed(69.9) == ThermalState::Normal);
    CHECK(f.ctl.state() == ThermalState::Normal);
}

TEST(reaching_warn_threshold_enters_warning)
{
    Fixture f;
    CHECK(f.feed(72.0) == ThermalState::Warning);
    CHECK(f.ctl.state() == ThermalState::Warning);
}

TEST(reaching_throttle_threshold_enters_throttled)
{
    Fixture f;
    CHECK(f.feed(90.0) == ThermalState::Throttled);
    CHECK(f.ctl.state() == ThermalState::Throttled);
}

TEST(reaching_shutdown_threshold_enters_shutdown)
{
    Fixture f;
    CHECK(f.feed(105.0) == ThermalState::Shutdown);
    CHECK(f.ctl.state() == ThermalState::Shutdown);
}

TEST(escalates_through_each_level)
{
    Fixture f;
    CHECK(f.feed(72.0) == ThermalState::Warning);
    CHECK(f.feed(88.0) == ThermalState::Throttled);
    CHECK(f.feed(101.0) == ThermalState::Shutdown);
}

TEST(exact_threshold_values_trigger)
{
    {
        Fixture f;
        CHECK(f.feed(70.0) == ThermalState::Warning);
    }
    {
        Fixture f;
        CHECK(f.feed(85.0) == ThermalState::Throttled);
    }
    {
        Fixture f;
        CHECK(f.feed(100.0) == ThermalState::Shutdown);
    }
}

TEST(warning_persists_below_warn_until_recover)
{
    Fixture f;
    CHECK(f.feed(72.0) == ThermalState::Warning);
    // Cooled to just under warn (70) but still above recover (60): hysteresis
    // keeps the warning up.
    CHECK(f.feed(68.0) == ThermalState::Warning);
    CHECK(f.feed(62.0) == ThermalState::Warning);
}

TEST(returns_to_normal_at_recover_threshold)
{
    Fixture f;
    CHECK(f.feed(72.0) == ThermalState::Warning);
    CHECK(f.feed(60.0) == ThermalState::Normal);
    CHECK(f.ctl.state() == ThermalState::Normal);
}

TEST(returns_to_normal_below_recover_threshold)
{
    Fixture f;
    CHECK(f.feed(72.0) == ThermalState::Warning);
    CHECK(f.feed(40.0) == ThermalState::Normal);
}

TEST(noisy_readings_around_warn_do_not_flap)
{
    Fixture f;
    CHECK(f.feed(71.0) == ThermalState::Warning);
    for (int i = 0; i < 10; ++i)
    {
        // Jitter across the 70C trigger, never reaching recover (60C).
        CHECK(f.feed(69.0) == ThermalState::Warning);
        CHECK(f.feed(71.0) == ThermalState::Warning);
    }
}

TEST(throttled_does_not_fall_to_normal_until_recover)
{
    Fixture f;
    CHECK(f.feed(90.0) == ThermalState::Throttled);
    // Below throttle but well above recover: must not be Normal.
    CHECK(f.feed(80.0) != ThermalState::Normal);
    CHECK(f.feed(65.0) != ThermalState::Normal);
    CHECK(f.feed(55.0) == ThermalState::Normal);
}

TEST(noisy_readings_around_throttle_do_not_drop_to_normal)
{
    Fixture f;
    CHECK(f.feed(86.0) == ThermalState::Throttled);
    for (int i = 0; i < 10; ++i)
    {
        CHECK(f.feed(84.0) != ThermalState::Normal);
        CHECK(f.feed(86.0) == ThermalState::Throttled);
    }
}

TEST(re_escalation_works_after_recovery)
{
    Fixture f;
    CHECK(f.feed(90.0) == ThermalState::Throttled);
    CHECK(f.feed(50.0) == ThermalState::Normal);
    CHECK(f.feed(72.0) == ThermalState::Warning);
    CHECK(f.feed(90.0) == ThermalState::Throttled);
}

TEST(nan_sample_is_rejected)
{
    Fixture f;
    CHECK(f.feed(90.0) == ThermalState::Throttled);
    CHECK(f.feed(std::numeric_limits<double>::quiet_NaN()) == ThermalState::Throttled);
    CHECK(f.ctl.state() == ThermalState::Throttled);
}

TEST(nan_sample_does_not_escalate_from_normal)
{
    Fixture f;
    CHECK(f.feed(std::numeric_limits<double>::quiet_NaN()) == ThermalState::Normal);
    CHECK(f.ctl.state() == ThermalState::Normal);
}

TEST(stale_sample_is_rejected)
{
    ThermalController ctl(70.0, 85.0, 100.0, 60.0);
    CHECK(ctl.on_sample(Sample{kT0 + 10s, 40.0}) == ThermalState::Normal);
    // Older timestamp than the last accepted sample: must be ignored, even
    // though its temperature would otherwise trigger Throttled.
    CHECK(ctl.on_sample(Sample{kT0 + 5s, 95.0}) == ThermalState::Normal);
    CHECK(ctl.state() == ThermalState::Normal);
}

TEST(stale_sample_does_not_cool_an_active_state)
{
    ThermalController ctl(70.0, 85.0, 100.0, 60.0);
    CHECK(ctl.on_sample(Sample{kT0 + 10s, 90.0}) == ThermalState::Throttled);
    CHECK(ctl.on_sample(Sample{kT0 + 3s, 20.0}) == ThermalState::Throttled);
    CHECK(ctl.state() == ThermalState::Throttled);
}

TEST(a_newer_sample_after_a_rejected_one_is_processed)
{
    ThermalController ctl(70.0, 85.0, 100.0, 60.0);
    CHECK(ctl.on_sample(Sample{kT0 + 10s, 40.0}) == ThermalState::Normal);
    CHECK(ctl.on_sample(Sample{kT0 + 5s, 95.0}) == ThermalState::Normal); // stale
    CHECK(ctl.on_sample(Sample{kT0 + 11s, 90.0}) == ThermalState::Throttled);
}

TEST(state_matches_last_on_sample_result)
{
    Fixture f;
    ThermalState s = f.feed(75.0);
    CHECK(s == f.ctl.state());
    s = f.feed(90.0);
    CHECK(s == f.ctl.state());
}

TEST_MAIN()
