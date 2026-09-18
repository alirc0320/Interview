// Combined practice program for LearnCpp chapters 16 (Dynamic arrays:
// std::vector) and 17 (Fixed-size arrays: std::array and C-style arrays).
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
    // TODO: Build a std::vector<int> of N values with push_back, then copy
    // its contents into a std::array<int, N> and into a raw C-style
    // int[N] array with the same values. Write a function that takes the
    // C-style array by pointer plus an explicit length parameter (to sum
    // it), and a separate function/template that takes the std::array by
    // const reference (no explicit length needed). Demonstrate array decay
    // by passing the C-style array into a function and checking sizeof
    // there vs. sizeof in main().

    // TODO: at least 5 assert() checks, e.g. that the vector, std::array,
    // and C-style array all contain the same values, that the sums
    // computed via all three containers agree, and that sizeof(the
    // C-style array) in main() differs from sizeof of the decayed pointer
    // inside the function it's passed to.

    std::cout << "ok\n";
    return 0;
}
