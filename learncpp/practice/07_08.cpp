// Combined practice program for LearnCpp chapters 7 and 8
// (Scope, Duration, and Linkage; Control Flow).
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
    // TODO: Write a function that uses a static local variable to count
    // how many times it has been called (chapter 7: static duration).
    // Call it inside a for loop several times, and inside the loop use
    // an if/switch to skip (continue) or stop early (break) on specific
    // iterations (chapter 8: loops, break/continue). Also declare a
    // variable at global scope and a same-named local variable in main
    // to demonstrate shadowing, printing both to show they're distinct.

    // TODO: at least 5 assert() checks.

    std::cout << "ok\n";
    return 0;
}
