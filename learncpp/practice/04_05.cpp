// Combined practice program for LearnCpp chapters 4 (Fundamental Data
// Types) and 5 (Constants and Strings).
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
#include <string>
#include <string_view>

int main()
{
    // TODO: Declare variables covering several fundamental types (int,
    // unsigned int, double, bool, char), a constexpr constant used in at
    // least one calculation, a std::string built via concatenation, and a
    // std::string_view over a literal or over part of the string. Use
    // static_cast at least once to convert between two of the fundamental
    // types and print the before/after values.

    // TODO: at least 5 assert() checks covering type conversions, the
    // constexpr value, and string/string_view content or length.

    std::cout << "ok\n";
    return 0;
}
