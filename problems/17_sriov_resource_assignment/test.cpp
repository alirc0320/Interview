#include "minitest.h"
#include "solution.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <set>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - assign(vf_id, n_queues, mem) returns the VF's resources, or
//     std::nullopt on failure. On failure NOTHING is consumed (no partial
//     allocation).
//   - Success: result.vf_id == vf_id; queue_ids holds exactly n_queues
//     distinct ids, each < num_queues; the memory window is
//     [memory_window_offset, offset + memory_window_size) with
//     size == memory_size and offset + size <= total_memory.
//   - Isolation: no queue id and no memory byte is ever given to two live
//     VFs at once.
//   - Assigning a vf_id that is already live fails.
//   - Not enough queues or not enough (contiguous) memory fails.
//   - release(vf_id) returns true and returns that VF's queues and memory
//     to the pool; releasing an unknown / already-released VF returns
//     false.
//   - Zero-queue / zero-memory requests aren't tested (ambiguous).
// ---------------------------------------------------------------------

namespace
{

bool windows_overlap(const VfResources& a, const VfResources& b)
{
    return a.memory_window_offset < b.memory_window_offset + b.memory_window_size &&
           b.memory_window_offset < a.memory_window_offset + a.memory_window_size;
}

bool queues_overlap(const VfResources& a, const VfResources& b)
{
    for (std::uint32_t q : a.queue_ids)
        if (std::find(b.queue_ids.begin(), b.queue_ids.end(), q) != b.queue_ids.end())
            return true;
    return false;
}

bool well_formed(const VfResources& r, std::uint32_t vf, std::size_t nq, std::size_t mem,
                 std::size_t total_queues, std::size_t total_mem)
{
    if (r.vf_id != vf || r.queue_ids.size() != nq || r.memory_window_size != mem)
        return false;
    std::set<std::uint32_t> unique(r.queue_ids.begin(), r.queue_ids.end());
    if (unique.size() != nq)
        return false;
    for (std::uint32_t q : r.queue_ids)
        if (q >= total_queues)
            return false;
    return r.memory_window_offset + r.memory_window_size <= total_mem;
}

} // namespace

TEST(assign_returns_well_formed_resources)
{
    SriovResourceManager mgr(16, 1 << 20);
    auto r = mgr.assign(1, 4, 4096);
    CHECK(r.has_value());
    CHECK(r && well_formed(*r, 1, 4, 4096, 16, 1 << 20));
}

TEST(two_vfs_get_disjoint_queues_and_memory)
{
    SriovResourceManager mgr(16, 1 << 20);
    auto a = mgr.assign(1, 4, 8192);
    auto b = mgr.assign(2, 4, 8192);
    CHECK(a && b);
    if (!(a && b))
        return;
    CHECK(well_formed(*a, 1, 4, 8192, 16, 1 << 20));
    CHECK(well_formed(*b, 2, 4, 8192, 16, 1 << 20));
    CHECK(!queues_overlap(*a, *b));
    CHECK(!windows_overlap(*a, *b));
}

TEST(many_vfs_never_share_resources)
{
    SriovResourceManager mgr(32, 32 * 4096);
    std::vector<VfResources> live;
    for (std::uint32_t vf = 0; vf < 8; ++vf)
    {
        auto r = mgr.assign(vf, 4, 4 * 4096);
        CHECK(r.has_value());
        if (r)
            live.push_back(*r);
    }
    CHECK_EQ(live.size(), 8u);
    for (std::size_t i = 0; i < live.size(); ++i)
    {
        for (std::size_t j = i + 1; j < live.size(); ++j)
        {
            CHECK(!queues_overlap(live[i], live[j]));
            CHECK(!windows_overlap(live[i], live[j]));
        }
    }
}

TEST(exhausting_queues_fails_further_assignments)
{
    SriovResourceManager mgr(8, 1 << 20);
    CHECK(mgr.assign(1, 8, 4096).has_value());
    CHECK(!mgr.assign(2, 1, 4096).has_value());
}

TEST(exhausting_memory_fails_further_assignments)
{
    SriovResourceManager mgr(64, 8192);
    CHECK(mgr.assign(1, 1, 8192).has_value());
    CHECK(!mgr.assign(2, 1, 1).has_value());
}

TEST(request_larger_than_total_fails)
{
    SriovResourceManager mgr(8, 4096);
    CHECK(!mgr.assign(1, 9, 1024).has_value()); // too many queues
    CHECK(!mgr.assign(1, 1, 8192).has_value()); // too much memory
}

TEST(failed_assignment_consumes_nothing)
{
    SriovResourceManager mgr(8, 8192);
    // Enough queues, too much memory -> must fail without keeping the queues.
    CHECK(!mgr.assign(1, 8, 16384).has_value());
    // Enough memory, too many queues -> must fail without keeping the memory.
    CHECK(!mgr.assign(1, 9, 8192).has_value());
    // Everything is still available.
    CHECK(mgr.assign(1, 8, 8192).has_value());
}

TEST(duplicate_vf_id_is_rejected)
{
    SriovResourceManager mgr(16, 1 << 20);
    CHECK(mgr.assign(5, 2, 4096).has_value());
    CHECK(!mgr.assign(5, 2, 4096).has_value());
}

TEST(duplicate_vf_id_attempt_does_not_leak_resources)
{
    SriovResourceManager mgr(4, 4 * 4096);
    CHECK(mgr.assign(1, 2, 2 * 4096).has_value());
    CHECK(!mgr.assign(1, 2, 2 * 4096).has_value());
    // The remaining half is still free for a different VF.
    CHECK(mgr.assign(2, 2, 2 * 4096).has_value());
}

TEST(release_unknown_vf_returns_false)
{
    SriovResourceManager mgr(8, 4096);
    CHECK(!mgr.release(42));
}

TEST(release_returns_true_once)
{
    SriovResourceManager mgr(8, 1 << 20);
    CHECK(mgr.assign(3, 2, 4096).has_value());
    CHECK(mgr.release(3));
    CHECK(!mgr.release(3));
}

TEST(released_resources_can_be_reassigned)
{
    SriovResourceManager mgr(4, 4096);
    CHECK(mgr.assign(1, 4, 4096).has_value());
    CHECK(!mgr.assign(2, 1, 1).has_value());
    CHECK(mgr.release(1));
    auto r = mgr.assign(2, 4, 4096);
    CHECK(r.has_value());
    CHECK(r && well_formed(*r, 2, 4, 4096, 4, 4096));
}

TEST(vf_id_can_be_reused_after_release)
{
    SriovResourceManager mgr(8, 1 << 20);
    CHECK(mgr.assign(9, 2, 4096).has_value());
    CHECK(mgr.release(9));
    CHECK(mgr.assign(9, 2, 4096).has_value());
}

TEST(releasing_one_vf_does_not_disturb_another)
{
    SriovResourceManager mgr(8, 4 * 4096);
    auto a = mgr.assign(1, 4, 2 * 4096);
    auto b = mgr.assign(2, 4, 2 * 4096);
    CHECK(a && b);
    CHECK(mgr.release(1));

    // A new VF may reuse A's resources, but must never collide with B's.
    auto c = mgr.assign(3, 4, 2 * 4096);
    CHECK(c.has_value());
    if (b && c)
    {
        CHECK(!queues_overlap(*b, *c));
        CHECK(!windows_overlap(*b, *c));
    }
    // ...and B is still live: releasing it works, releasing A again doesn't.
    CHECK(mgr.release(2));
    CHECK(!mgr.release(1));
}

TEST(assign_release_churn_keeps_isolation)
{
    SriovResourceManager mgr(16, 16 * 4096);
    std::vector<VfResources> live;
    for (std::uint32_t round = 0; round < 40; ++round)
    {
        if (live.size() == 4)
        {
            CHECK(mgr.release(live.front().vf_id));
            live.erase(live.begin());
        }
        auto r = mgr.assign(round, 4, 4 * 4096);
        CHECK(r.has_value());
        if (!r)
            continue;
        for (const VfResources& other : live)
        {
            CHECK(!queues_overlap(*r, other));
            CHECK(!windows_overlap(*r, other));
        }
        live.push_back(*r);
    }
}

TEST_MAIN()
