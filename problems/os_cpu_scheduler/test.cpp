#include "minitest.h"
#include "solution.h"

#include <cstdint>
#include <map>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - The result is the execution timeline, in time order, one entry per
//     contiguous run of a process on the CPU. No entries for idle time.
//   - FCFS: run to completion in arrival order (ties: input order). The
//     CPU idles until the next arrival when the ready queue is empty.
//   - RoundRobin: FIFO ready queue, each dispatch runs
//     min(quantum, remaining). A process arriving at the exact moment
//     another's slice ends is queued BEFORE the preempted process.
//   - Whether back-to-back slices of the same process are reported as
//     separate entries or merged isn't pinned down, so RR comparisons
//     go through merged() which merges contiguous same-pid entries.
//   - Zero-burst processes aren't tested.
// ---------------------------------------------------------------------

using E = ScheduleEntry;

namespace
{

std::vector<E> merged(const std::vector<E>& in)
{
    std::vector<E> out;
    for (const E& e : in)
    {
        if (!out.empty() && out.back().pid == e.pid && out.back().end_time == e.start_time)
            out.back().end_time = e.end_time;
        else
            out.push_back(e);
    }
    return out;
}

bool same(const std::vector<E>& a, const std::vector<E>& b)
{
    if (a.size() != b.size())
        return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i].pid != b[i].pid || a[i].start_time != b[i].start_time ||
            a[i].end_time != b[i].end_time)
            return false;
    return true;
}

} // namespace

TEST(empty_input_gives_empty_timeline)
{
    CHECK(simulate_schedule({}, SchedulingPolicy::FCFS).empty());
    CHECK(simulate_schedule({}, SchedulingPolicy::RoundRobin, 3).empty());
}

TEST(fcfs_single_process)
{
    auto t = simulate_schedule({{1, 0, 5}}, SchedulingPolicy::FCFS);
    CHECK(same(t, {{1, 0, 5}}));
}

TEST(fcfs_runs_in_arrival_order_back_to_back)
{
    auto t = simulate_schedule({{1, 0, 5}, {2, 2, 3}, {3, 4, 1}}, SchedulingPolicy::FCFS);
    CHECK(same(t, {{1, 0, 5}, {2, 5, 8}, {3, 8, 9}}));
}

TEST(fcfs_sorts_by_arrival_not_input_order)
{
    auto t = simulate_schedule({{3, 6, 1}, {1, 0, 2}, {2, 3, 2}}, SchedulingPolicy::FCFS);
    CHECK(same(t, {{1, 0, 2}, {2, 3, 5}, {3, 6, 7}}));
}

TEST(fcfs_idles_until_next_arrival)
{
    auto t = simulate_schedule({{1, 0, 2}, {2, 10, 3}}, SchedulingPolicy::FCFS);
    CHECK(same(t, {{1, 0, 2}, {2, 10, 13}}));
}

TEST(fcfs_first_arrival_after_time_zero)
{
    auto t = simulate_schedule({{1, 7, 2}}, SchedulingPolicy::FCFS);
    CHECK(same(t, {{1, 7, 9}}));
}

TEST(fcfs_simultaneous_arrivals_use_input_order)
{
    auto t = simulate_schedule({{1, 0, 3}, {2, 0, 2}}, SchedulingPolicy::FCFS);
    CHECK(same(t, {{1, 0, 3}, {2, 3, 5}}));
}

TEST(fcfs_quantum_is_ignored)
{
    auto a = simulate_schedule({{1, 0, 4}, {2, 1, 2}}, SchedulingPolicy::FCFS, 1);
    CHECK(same(a, {{1, 0, 4}, {2, 4, 6}}));
}

TEST(fcfs_large_times_do_not_overflow_32_bits)
{
    const std::uint64_t big = 5000000000ull;
    auto t = simulate_schedule({{1, big, big}, {2, big, 1}}, SchedulingPolicy::FCFS);
    CHECK(same(t, {{1, big, 2 * big}, {2, 2 * big, 2 * big + 1}}));
}

TEST(rr_single_process_runs_to_completion)
{
    auto t = simulate_schedule({{1, 0, 5}}, SchedulingPolicy::RoundRobin, 2);
    CHECK(same(merged(t), {{1, 0, 5}}));
}

TEST(rr_burst_shorter_than_quantum_finishes_in_one_slice)
{
    auto t = simulate_schedule({{1, 0, 2}, {2, 0, 3}}, SchedulingPolicy::RoundRobin, 10);
    CHECK(same(t, {{1, 0, 2}, {2, 2, 5}}));
}

TEST(rr_alternates_between_two_processes)
{
    // Both arrive at 0, quantum 2: 1(0-2) 2(2-4) 1(4-6) 2(6-7)
    auto t = simulate_schedule({{1, 0, 4}, {2, 0, 3}}, SchedulingPolicy::RoundRobin, 2);
    CHECK(same(merged(t), {{1, 0, 2}, {2, 2, 4}, {1, 4, 6}, {2, 6, 7}}));
}

TEST(rr_late_arrival_joins_the_queue_without_tie)
{
    // P2 arrives at 1 (mid-slice), queued ahead of the preempted P1.
    auto t = simulate_schedule({{1, 0, 4}, {2, 1, 2}}, SchedulingPolicy::RoundRobin, 3);
    CHECK(same(merged(t), {{1, 0, 3}, {2, 3, 5}, {1, 5, 6}}));
}

TEST(rr_arrival_at_slice_end_is_queued_before_the_preempted_process)
{
    // Classic textbook example, quantum 2:
    //   P1 (arr 0, burst 5), P2 (arr 1, burst 3), P3 (arr 2, burst 1)
    //   P3 arrives exactly when P1's first slice ends.
    auto t = simulate_schedule({{1, 0, 5}, {2, 1, 3}, {3, 2, 1}}, SchedulingPolicy::RoundRobin, 2);
    CHECK(same(merged(t), {{1, 0, 2}, {2, 2, 4}, {3, 4, 5}, {1, 5, 7}, {2, 7, 8}, {1, 8, 9}}));
}

TEST(rr_idles_between_disjoint_processes)
{
    auto t = simulate_schedule({{1, 0, 1}, {2, 5, 2}}, SchedulingPolicy::RoundRobin, 2);
    CHECK(same(merged(t), {{1, 0, 1}, {2, 5, 7}}));
}

TEST(rr_quantum_one_interleaves_every_tick)
{
    auto t = simulate_schedule({{1, 0, 2}, {2, 0, 2}}, SchedulingPolicy::RoundRobin, 1);
    CHECK(same(t, {{1, 0, 1}, {2, 1, 2}, {1, 2, 3}, {2, 3, 4}}));
}

TEST(rr_timeline_invariants_hold_for_a_busy_workload)
{
    std::vector<Process> procs = {{1, 0, 7}, {2, 2, 4}, {3, 3, 9}, {4, 6, 2}, {5, 20, 3}};
    auto t = simulate_schedule(procs, SchedulingPolicy::RoundRobin, 3);

    std::map<int, std::uint64_t> ran;
    std::uint64_t prev_end = 0;
    for (const E& e : t)
    {
        CHECK(e.end_time > e.start_time);
        CHECK(e.start_time >= prev_end);     // never overlaps the previous slice
        CHECK(e.end_time - e.start_time <= 3); // never exceeds the quantum
        for (const Process& p : procs)
            if (p.pid == e.pid)
                CHECK(e.start_time >= p.arrival_time); // never runs before arriving
        ran[e.pid] += e.end_time - e.start_time;
        prev_end = e.end_time;
    }
    for (const Process& p : procs)
        CHECK_EQ(ran[p.pid], p.burst_time); // every process gets exactly its burst
}

TEST_MAIN()
