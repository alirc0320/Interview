// Combined practice program for LearnCpp chapter 11 and Chapter F
// (Function Overloading and Function Templates; Constexpr Functions).
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
    // TODO: Write a function template `T addValues(T a, T b)` (chapter
    // 11) and mark it constexpr (Chapter F) so it can be evaluated at
    // compile time. Instantiate it with at least two different types
    // (e.g. int and double). Also write a non-template overload for a
    // specific type that takes precedence over the template for that
    // type, and demonstrate calling both the template instantiation and
    // the overload, printing which one ran.

    // TODO: at least 5 assert() checks.

    std::cout << "ok\n";
    return 0;
}
