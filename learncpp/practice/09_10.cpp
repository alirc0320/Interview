// Combined practice program for LearnCpp chapters 9 and 10
// (Error Detection and Handling; Type Conversion, Type Aliases,
// and Type Deduction).
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
    // TODO: Write a function that takes a double (chapter 10: numeric
    // conversions) and safely converts/rounds it to an int, using
    // static_cast explicitly rather than relying on implicit narrowing.
    // Validate the input value against an expected range using assert
    // as a precondition check (chapter 9), and write a second function
    // that simulates "parsing" a value and returns whether it succeeded,
    // demonstrating error-flag-style handling instead of assert for a
    // recoverable error case.

    // TODO: at least 5 assert() checks.

    std::cout << "ok\n";
    return 0;
}
