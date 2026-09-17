#include "solution.h"

#include <unordered_map>

std::vector<int> twoSum(const std::vector<int>& nums, int target)
{
    std::unordered_map<int, int> seen; // value -> index
    for (std::size_t i = 0; i < nums.size(); ++i)
    {
        int index = static_cast<int>(i);
        int need = target - nums[i];
        auto it = seen.find(need);
        if (it != seen.end())
        {
            return {it->second, index};
        }
        seen[nums[i]] = index;
    }
    return {};
}
