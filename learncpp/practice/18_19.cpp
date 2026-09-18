// Combined practice program for LearnCpp chapters 18 (Iterators and
// Algorithms) and 19 (Dynamic Allocation).
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
    // TODO: Dynamically allocate an int array with new[] (size chosen at
    // runtime, e.g. read or hardcoded), fill it with unsorted values, then
    // sort it in place two ways: once with your own manual sort using
    // iterator-like pointer traversal (as in 18.1's selection sort), and
    // once via std::sort with pointer iterators (std::begin/std::end or
    // arr, arr + n) from <algorithm>, as covered in 18.2/18.3. Free the
    // array with delete[] when done (matching new[]/delete[] from 19.1-19.2).

    // TODO: at least 5 assert() checks, e.g. that the array is sorted
    // (arr[i] <= arr[i + 1] for all i), that a known value can be found at
    // its expected sorted position via std::find, and that both the manual
    // sort and std::sort produce the same result on a copy of the original
    // data.

    std::cout << "ok\n";
    return 0;
}
