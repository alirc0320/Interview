// Combined practice program for LearnCpp chapters 14 and 15
// (Introduction to Classes; More on Classes).
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
    // TODO: Define a small class (e.g. a Stack or Counter) with a
    // constructor that uses a member initializer list, at least one const
    // member function, a static member variable that tracks how many
    // instances currently exist (incremented in the constructor, decremented
    // in the destructor), and a non-const member function that returns
    // *this by reference so calls can be chained. Construct several
    // instances in nested scopes so the destructor runs and the static
    // count changes visibly.

    // TODO: at least 5 assert() checks covering: correct behavior of the
    // core member function(s), the static instance count increasing on
    // construction and decreasing after a scope ends, and that a chained
    // call sequence produces the expected final state.

    std::cout << "ok\n";
    return 0;
}
