#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - `capacity` bounds the number of submitted-but-not-yet-processed
//     commands. submit() on a full queue returns std::nullopt.
//   - submit() assigns and returns a command id; ids are strictly
//     increasing across submissions (the starting value isn't pinned).
//     The id inside the `Command` passed in is ignored/overwritten.
//   - process_one() simulates the device handling the oldest pending
//     command (FIFO), producing a Completion whose id matches it.
//     Calling it with nothing pending is a harmless no-op.
//   - poll_completion() returns completions in the order they were
//     produced, or std::nullopt if none are ready. Each completion is
//     returned exactly once.
//   - In tests that check slot reuse, completions are polled before
//     resubmitting, so it doesn't matter whether unpolled completions
//     also occupy queue space.
// ---------------------------------------------------------------------

TEST(poll_on_fresh_queue_returns_nullopt)
{
    CommandQueue q(4);
    CHECK(!q.poll_completion().has_value());
}

TEST(submit_returns_an_id)
{
    CommandQueue q(4);
    auto id = q.submit(Command{});
    CHECK(id.has_value());
}

TEST(ids_are_strictly_increasing)
{
    CommandQueue q(8);
    CommandId prev = 0;
    bool first = true;
    for (int i = 0; i < 8; ++i)
    {
        auto id = q.submit(Command{});
        CHECK(id.has_value());
        if (!id)
            continue;
        if (!first)
            CHECK(*id > prev);
        prev = *id;
        first = false;
    }
}

TEST(ids_are_unique)
{
    CommandQueue q(16);
    std::vector<CommandId> ids;
    for (int i = 0; i < 16; ++i)
    {
        auto id = q.submit(Command{});
        CHECK(id.has_value());
        if (id)
            ids.push_back(*id);
    }
    for (std::size_t i = 0; i < ids.size(); ++i)
        for (std::size_t j = i + 1; j < ids.size(); ++j)
            CHECK(ids[i] != ids[j]);
}

TEST(submit_beyond_capacity_fails)
{
    CommandQueue q(3);
    CHECK(q.submit(Command{}).has_value());
    CHECK(q.submit(Command{}).has_value());
    CHECK(q.submit(Command{}).has_value());
    CHECK(!q.submit(Command{}).has_value());
    CHECK(!q.submit(Command{}).has_value());
}

TEST(processing_frees_a_slot)
{
    CommandQueue q(2);
    CHECK(q.submit(Command{}).has_value());
    CHECK(q.submit(Command{}).has_value());
    CHECK(!q.submit(Command{}).has_value());

    q.process_one();
    CHECK(q.poll_completion().has_value());

    CHECK(q.submit(Command{}).has_value());
    CHECK(!q.submit(Command{}).has_value());
}

TEST(completion_id_matches_submitted_command)
{
    CommandQueue q(4);
    auto id = q.submit(Command{});
    CHECK(id.has_value());
    q.process_one();
    auto c = q.poll_completion();
    CHECK(c.has_value());
    CHECK(c && id && c->id == *id);
}

TEST(no_completion_before_device_processes)
{
    CommandQueue q(4);
    q.submit(Command{});
    CHECK(!q.poll_completion().has_value());
}

TEST(completions_come_back_in_submission_order)
{
    CommandQueue q(4);
    auto a = q.submit(Command{});
    auto b = q.submit(Command{});
    auto c = q.submit(Command{});
    CHECK(a && b && c);
    q.process_one();
    q.process_one();
    q.process_one();

    auto ca = q.poll_completion();
    auto cb = q.poll_completion();
    auto cc = q.poll_completion();
    CHECK(ca && a && ca->id == *a);
    CHECK(cb && b && cb->id == *b);
    CHECK(cc && c && cc->id == *c);
}

TEST(each_completion_is_delivered_once)
{
    CommandQueue q(4);
    q.submit(Command{});
    q.process_one();
    CHECK(q.poll_completion().has_value());
    CHECK(!q.poll_completion().has_value());
    CHECK(!q.poll_completion().has_value());
}

TEST(process_one_yields_one_completion_at_a_time)
{
    CommandQueue q(4);
    q.submit(Command{});
    q.submit(Command{});
    q.process_one();
    CHECK(q.poll_completion().has_value());
    CHECK(!q.poll_completion().has_value()); // second command not processed yet
    q.process_one();
    CHECK(q.poll_completion().has_value());
}

TEST(process_one_with_nothing_pending_is_a_noop)
{
    CommandQueue q(4);
    q.process_one();
    q.process_one();
    CHECK(!q.poll_completion().has_value());

    auto id = q.submit(Command{});
    q.process_one();
    auto c = q.poll_completion();
    CHECK(c && id && c->id == *id);
}

TEST(ids_keep_increasing_after_wraparound_of_the_queue)
{
    CommandQueue q(2);
    CommandId prev = 0;
    bool first = true;
    for (int i = 0; i < 20; ++i)
    {
        auto id = q.submit(Command{});
        CHECK(id.has_value());
        if (!id)
            continue;
        if (!first)
            CHECK(*id > prev);
        prev = *id;
        first = false;

        q.process_one();
        auto c = q.poll_completion();
        CHECK(c && c->id == *id);
    }
}

TEST(interleaved_submit_and_process)
{
    CommandQueue q(2);
    auto a = q.submit(Command{});
    auto b = q.submit(Command{});
    q.process_one();
    auto ca = q.poll_completion();
    auto c = q.submit(Command{});
    CHECK(a && b && c && ca);
    CHECK(ca && a && ca->id == *a);

    q.process_one();
    q.process_one();
    auto cb = q.poll_completion();
    auto cc = q.poll_completion();
    CHECK(cb && b && cb->id == *b);
    CHECK(cc && c && cc->id == *c);
    CHECK(!q.poll_completion().has_value());
}

TEST(capacity_one)
{
    CommandQueue q(1);
    auto a = q.submit(Command{});
    CHECK(a.has_value());
    CHECK(!q.submit(Command{}).has_value());
    q.process_one();
    CHECK(q.poll_completion().has_value());
    auto b = q.submit(Command{});
    CHECK(b.has_value());
    CHECK(a && b && *b > *a);
}

TEST_MAIN()
