// Combined practice program for LearnCpp chapters 20 and 21
// (Functions: function pointers, lambdas, lambda captures;
// Operator Overloading).
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
    // TODO: Define a small class (e.g. Employee with a name and a salary)
    // and overload operator< (or operator> ) so instances can be ordered
    // (chapter 21). Build a small std::vector of several instances, then
    // sort it using std::sort with a lambda comparator (chapter 20) that
    // captures nothing and calls your overloaded operator, plus a second
    // sort using a lambda that captures a flag by value to flip the sort
    // direction (ascending vs descending) to show capture-by-value in
    // action.

    // TODO: at least 5 assert() checks.

    std::cout << "ok\n";
    return 0;
}
