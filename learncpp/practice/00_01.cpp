// Combined practice program for LearnCpp chapters 0 (Introduction / Getting
// Started) and 1 (C++ Basics).
// One program covering ideas from both chapters, 20-40 lines, at
// least 5 assert() checks.
//
// Compile:
//   clang++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
//     -g -O0 -fsanitize=address,undefined practice.cpp -o practice
// Run:
//   ./practice

#include <cassert>
#include <iostream>

int main()
{
    // TODO: Declare a handful of variables (at least an int, a double, and
    // a bool) using proper initialization (not left uninitialized), assign
    // to at least one of them after declaring it, and print each variable's
    // value to std::cout with a descriptive label on its own line. The
    // whole file must compile cleanly with -Wall -Wextra -Wpedantic
    // -Wconversion and no warnings (this is the chapter 0 goal: a clean,
    // warning-free build under a real language standard).

    // TODO: at least 5 assert() checks verifying the values/relationships
    // between the variables you declared above (e.g. that an arithmetic
    // result matches what you expect).

    std::cout << "ok\n";
    return 0;
}
