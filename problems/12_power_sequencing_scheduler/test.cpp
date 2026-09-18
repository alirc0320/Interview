#include "minitest.h"
#include "solution.h"

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - power_on_sequence() enables every rail exactly once, only after all
//     rails named in its depends_on have been enabled, and returns true.
//     The relative order of independent rails is not pinned down.
//   - A dependency cycle (including a rail depending on itself) or a
//     dependency on an unknown rail name returns false BEFORE enabling
//     anything (nothing to roll back).
//   - If a rail's enable() returns false, power_on_sequence() returns
//     false, calls disable() on every previously-enabled rail in REVERSE
//     enable order, does not call disable() on the failed rail (it never
//     came up) and does not call enable() on any rail afterwards.
//   - An empty rail list succeeds trivially.
// ---------------------------------------------------------------------

namespace
{

struct Log
{
    std::vector<std::string> enabled;
    std::vector<std::string> disabled;

    int index_of_enable(const std::string& name) const
    {
        auto it = std::find(enabled.begin(), enabled.end(), name);
        return it == enabled.end() ? -1 : static_cast<int>(it - enabled.begin());
    }
};

Rail make_rail(Log& log, std::string name, std::vector<std::string> deps = {},
               bool enable_succeeds = true)
{
    Rail r;
    r.name = name;
    r.depends_on = std::move(deps);
    r.enable = [&log, name, enable_succeeds]() {
        if (enable_succeeds)
            log.enabled.push_back(name);
        else
            log.enabled.push_back("!" + name); // record the attempt
        return enable_succeeds;
    };
    r.disable = [&log, name]() { log.disabled.push_back(name); };
    return r;
}

} // namespace

TEST(empty_list_succeeds)
{
    CHECK(power_on_sequence({}));
}

TEST(single_rail_is_enabled)
{
    Log log;
    CHECK(power_on_sequence({make_rail(log, "A")}));
    CHECK_EQ(log.enabled.size(), 1u);
    CHECK(log.enabled[0] == "A");
    CHECK(log.disabled.empty());
}

TEST(dependency_is_enabled_before_dependent)
{
    Log log;
    // Listed dependent-first on purpose: the input order must not matter.
    std::vector<Rail> rails = {make_rail(log, "CORE", {"VDD"}), make_rail(log, "VDD")};
    CHECK(power_on_sequence(rails));
    CHECK_EQ(log.enabled.size(), 2u);
    CHECK(log.index_of_enable("VDD") < log.index_of_enable("CORE"));
}

TEST(linear_chain_respects_order)
{
    Log log;
    std::vector<Rail> rails = {
        make_rail(log, "D", {"C"}),
        make_rail(log, "B", {"A"}),
        make_rail(log, "C", {"B"}),
        make_rail(log, "A"),
    };
    CHECK(power_on_sequence(rails));
    CHECK(log.enabled == (std::vector<std::string>{"A", "B", "C", "D"}));
}

TEST(diamond_dependency_enables_shared_rail_once)
{
    Log log;
    //      TOP
    //     /   \
    //    L     R
    //     \   /
    //      BASE
    std::vector<Rail> rails = {
        make_rail(log, "TOP", {"L", "R"}),
        make_rail(log, "L", {"BASE"}),
        make_rail(log, "R", {"BASE"}),
        make_rail(log, "BASE"),
    };
    CHECK(power_on_sequence(rails));
    CHECK_EQ(log.enabled.size(), 4u);
    CHECK(log.index_of_enable("BASE") < log.index_of_enable("L"));
    CHECK(log.index_of_enable("BASE") < log.index_of_enable("R"));
    CHECK(log.index_of_enable("L") < log.index_of_enable("TOP"));
    CHECK(log.index_of_enable("R") < log.index_of_enable("TOP"));
}

TEST(rail_with_multiple_dependencies)
{
    Log log;
    std::vector<Rail> rails = {
        make_rail(log, "PHY", {"A", "B", "C"}),
        make_rail(log, "C"),
        make_rail(log, "A"),
        make_rail(log, "B"),
    };
    CHECK(power_on_sequence(rails));
    CHECK_EQ(log.enabled.size(), 4u);
    CHECK_EQ(log.index_of_enable("PHY"), 3);
}

TEST(independent_rails_are_all_enabled)
{
    Log log;
    std::vector<Rail> rails = {make_rail(log, "X"), make_rail(log, "Y"), make_rail(log, "Z")};
    CHECK(power_on_sequence(rails));
    CHECK_EQ(log.enabled.size(), 3u);
    CHECK(log.index_of_enable("X") >= 0);
    CHECK(log.index_of_enable("Y") >= 0);
    CHECK(log.index_of_enable("Z") >= 0);
}

TEST(two_node_cycle_is_rejected_without_enabling_anything)
{
    Log log;
    std::vector<Rail> rails = {make_rail(log, "A", {"B"}), make_rail(log, "B", {"A"})};
    CHECK(!power_on_sequence(rails));
    CHECK(log.enabled.empty());
    CHECK(log.disabled.empty());
}

TEST(longer_cycle_is_rejected_without_enabling_anything)
{
    Log log;
    std::vector<Rail> rails = {
        make_rail(log, "A", {"C"}),
        make_rail(log, "B", {"A"}),
        make_rail(log, "C", {"B"}),
        make_rail(log, "FREE"), // acyclic bystander must not be enabled either
    };
    CHECK(!power_on_sequence(rails));
    CHECK(log.enabled.empty());
    CHECK(log.disabled.empty());
}

TEST(self_dependency_is_a_cycle)
{
    Log log;
    CHECK(!power_on_sequence({make_rail(log, "A", {"A"})}));
    CHECK(log.enabled.empty());
}

TEST(unknown_dependency_is_rejected)
{
    Log log;
    std::vector<Rail> rails = {make_rail(log, "A", {"DOES_NOT_EXIST"}), make_rail(log, "B")};
    CHECK(!power_on_sequence(rails));
    CHECK(log.enabled.empty());
    CHECK(log.disabled.empty());
}

TEST(failure_rolls_back_enabled_rails_in_reverse_order)
{
    Log log;
    std::vector<Rail> rails = {
        make_rail(log, "A"),
        make_rail(log, "B", {"A"}),
        make_rail(log, "C", {"B"}, /*enable_succeeds=*/false),
        make_rail(log, "D", {"C"}),
    };
    CHECK(!power_on_sequence(rails));
    // A and B came up; C's enable was attempted and failed; D never tried.
    CHECK(log.enabled == (std::vector<std::string>{"A", "B", "!C"}));
    CHECK(log.disabled == (std::vector<std::string>{"B", "A"}));
}

TEST(failure_of_first_rail_needs_no_rollback)
{
    Log log;
    std::vector<Rail> rails = {make_rail(log, "A", {}, false), make_rail(log, "B", {"A"})};
    CHECK(!power_on_sequence(rails));
    CHECK(log.enabled == (std::vector<std::string>{"!A"}));
    CHECK(log.disabled.empty());
}

TEST(failure_never_disables_rails_that_were_not_enabled)
{
    Log log;
    std::vector<Rail> rails = {
        make_rail(log, "A"),
        make_rail(log, "B", {"A"}, false),
        make_rail(log, "C", {"B"}),
        make_rail(log, "D", {"C"}),
    };
    CHECK(!power_on_sequence(rails));
    for (const std::string& d : log.disabled)
    {
        CHECK(d != "B");
        CHECK(d != "C");
        CHECK(d != "D");
    }
    CHECK(log.disabled == (std::vector<std::string>{"A"}));
}

TEST(success_never_calls_disable)
{
    Log log;
    std::vector<Rail> rails = {make_rail(log, "A"), make_rail(log, "B", {"A"})};
    CHECK(power_on_sequence(rails));
    CHECK(log.disabled.empty());
}

TEST(sequence_can_be_run_twice)
{
    Log log;
    std::vector<Rail> rails = {make_rail(log, "A"), make_rail(log, "B", {"A"})};
    CHECK(power_on_sequence(rails));
    CHECK(power_on_sequence(rails));
    CHECK_EQ(log.enabled.size(), 4u);
}

TEST_MAIN()
