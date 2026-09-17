#pragma once
#include <cstdint>
#include <vector>

struct Process
{
    int pid;
    std::uint64_t arrival_time;
    std::uint64_t burst_time;
};

struct ScheduleEntry
{
    int pid;
    std::uint64_t start_time;
    std::uint64_t end_time;
};

enum class SchedulingPolicy
{
    FCFS,
    RoundRobin,
};

// TODO: implement. Produce the execution timeline for the given
// processes under the given policy. `quantum` is only used for
// RoundRobin.
std::vector<ScheduleEntry> simulate_schedule(const std::vector<Process>& processes,
                                             SchedulingPolicy policy, std::uint64_t quantum = 0);
