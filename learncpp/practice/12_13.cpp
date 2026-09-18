// Combined practice program for LearnCpp chapters 12 and 13
// (Compound Types: References and Pointers; Compound Types: Enums and Structs).
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
    // TODO: Define an enum class for a small set of categories (e.g. shape
    // kind: circle/square/triangle) and a struct that holds a member of that
    // enum type plus some numeric data (e.g. a dimension). Write a function
    // that takes the struct by const reference and returns its computed
    // "area" by value, and a separate function that takes a pointer to the
    // struct and modifies one of its members through the pointer (e.g.
    // scales the dimension). Exercise both pass-by-reference and
    // pass-by-address on struct objects.

    // TODO: at least 5 assert() checks covering: correct area computation
    // for at least two different enum categories, that the pointer-based
    // mutation actually changed the original struct (not a copy), and that
    // a reference to a struct member reflects changes made via the struct.

    std::cout << "ok\n";
    return 0;
}
