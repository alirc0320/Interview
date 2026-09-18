#include "minitest.h"
#include "solution.h"

// ---------------------------------------------------------------------
// Assumed contract (the guide leaves timeouts / retry limits / recovery
// open, so only the core rules are pinned here -- adjust if you design
// it differently on purpose):
//   - A new machine starts in Off.
//   - The happy path is strictly
//         Off -> Powering -> Resetting -> Initializing -> Ready
//     and transition_to() returns true for each step.
//   - Skipping a stage (e.g. Off -> Ready) or moving backwards along the
//     happy path is rejected: transition_to() returns false and the state
//     is unchanged.
//   - Failed is reachable from any in-progress boot state (Powering,
//     Resetting, Initializing) -- "any failure enters a safe state".
//   - Failed is safe: it cannot jump straight to a boot stage or Ready.
// ---------------------------------------------------------------------

namespace
{
// Drives a fresh machine up to `target` along the happy path.
void boot_to(BootStateMachine& sm, BootState target)
{
    const BootState path[] = {BootState::Powering, BootState::Resetting, BootState::Initializing,
                              BootState::Ready};
    for (BootState s : path)
    {
        if (sm.state() == target)
            return;
        CHECK(sm.transition_to(s));
    }
}
} // namespace

TEST(starts_in_off)
{
    BootStateMachine sm;
    CHECK(sm.state() == BootState::Off);
}

TEST(happy_path_reaches_ready)
{
    BootStateMachine sm;
    CHECK(sm.transition_to(BootState::Powering));
    CHECK(sm.state() == BootState::Powering);
    CHECK(sm.transition_to(BootState::Resetting));
    CHECK(sm.state() == BootState::Resetting);
    CHECK(sm.transition_to(BootState::Initializing));
    CHECK(sm.state() == BootState::Initializing);
    CHECK(sm.transition_to(BootState::Ready));
    CHECK(sm.state() == BootState::Ready);
}

TEST(cannot_skip_from_off)
{
    BootStateMachine sm;
    CHECK(!sm.transition_to(BootState::Resetting));
    CHECK(sm.state() == BootState::Off);
    CHECK(!sm.transition_to(BootState::Initializing));
    CHECK(sm.state() == BootState::Off);
    CHECK(!sm.transition_to(BootState::Ready));
    CHECK(sm.state() == BootState::Off);
}

TEST(cannot_skip_from_powering)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Powering);
    CHECK(!sm.transition_to(BootState::Initializing));
    CHECK(sm.state() == BootState::Powering);
    CHECK(!sm.transition_to(BootState::Ready));
    CHECK(sm.state() == BootState::Powering);
}

TEST(cannot_skip_from_resetting)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Resetting);
    CHECK(!sm.transition_to(BootState::Ready));
    CHECK(sm.state() == BootState::Resetting);
}

TEST(cannot_go_backwards_along_the_boot_path)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Initializing);
    CHECK(!sm.transition_to(BootState::Resetting));
    CHECK(sm.state() == BootState::Initializing);
    CHECK(!sm.transition_to(BootState::Powering));
    CHECK(sm.state() == BootState::Initializing);
    CHECK(!sm.transition_to(BootState::Off));
    CHECK(sm.state() == BootState::Initializing);
}

TEST(rejected_transition_leaves_machine_usable)
{
    BootStateMachine sm;
    CHECK(!sm.transition_to(BootState::Ready));
    CHECK(sm.transition_to(BootState::Powering)); // still able to proceed normally
    CHECK(sm.state() == BootState::Powering);
}

TEST(failure_from_powering_enters_failed)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Powering);
    CHECK(sm.transition_to(BootState::Failed));
    CHECK(sm.state() == BootState::Failed);
}

TEST(failure_from_resetting_enters_failed)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Resetting);
    CHECK(sm.transition_to(BootState::Failed));
    CHECK(sm.state() == BootState::Failed);
}

TEST(failure_from_initializing_enters_failed)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Initializing);
    CHECK(sm.transition_to(BootState::Failed));
    CHECK(sm.state() == BootState::Failed);
}

TEST(failed_cannot_jump_to_a_boot_stage_or_ready)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Resetting);
    CHECK(sm.transition_to(BootState::Failed));

    CHECK(!sm.transition_to(BootState::Ready));
    CHECK(sm.state() == BootState::Failed);
    CHECK(!sm.transition_to(BootState::Initializing));
    CHECK(sm.state() == BootState::Failed);
    CHECK(!sm.transition_to(BootState::Resetting));
    CHECK(sm.state() == BootState::Failed);
    CHECK(!sm.transition_to(BootState::Powering));
    CHECK(sm.state() == BootState::Failed);
}

TEST(state_reads_are_stable)
{
    BootStateMachine sm;
    boot_to(sm, BootState::Powering);
    CHECK(sm.state() == BootState::Powering);
    CHECK(sm.state() == BootState::Powering);
}

TEST_MAIN()
