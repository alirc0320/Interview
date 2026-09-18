// Combined practice program for LearnCpp chapters 6 (Operators) and O
// (Bit Manipulation, optional chapter).
// One program covering ideas from both chapters, 20-40 lines, at
// least 5 assert() checks.
//
// Compile:
//   clang++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
//     -g -O0 -fsanitize=address,undefined practice.cpp -o practice
// Run:
//   ./practice

#include <bitset>
#include <cassert>
#include <iostream>

int main()
{
    // TODO: Use arithmetic, relational, logical, and the conditional
    // operator on a few plain int/bool variables to compute some derived
    // values (e.g. min/max of a few numbers, a remainder check). Then,
    // separately, use a std::bitset<8> (or bitwise operators with named
    // mask constants) to set, clear, toggle, and test individual flag
    // bits, printing the final bit pattern.

    // TODO: at least 5 assert() checks covering both the operator results
    // and the final state of the bit flags.

    std::cout << "ok\n";
    return 0;
}
