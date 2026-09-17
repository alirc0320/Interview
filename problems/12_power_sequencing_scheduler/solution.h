#pragma once
#include <functional>
#include <string>
#include <vector>

struct Rail
{
    std::string name;
    std::vector<std::string> depends_on;
    std::function<bool()> enable;  // returns success/failure
    std::function<void()> disable; // used for rollback
};

// TODO: implement. Determine a valid power-on order, detect cycles,
// execute the sequence, and roll back already-enabled rails if a
// step fails.
bool power_on_sequence(const std::vector<Rail>& rails);
