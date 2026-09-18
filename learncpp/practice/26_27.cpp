// Combined practice program for LearnCpp chapters 26 and 27
// (Templates and Classes; Exceptions).
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
    // TODO: Write a small class template (e.g. a fixed-capacity
    // `Stack<T, int Capacity>` or a `SafeArray<T, int Size>`) whose
    // operations can fail (e.g. push on a full stack, pop on an empty
    // stack, or out-of-bounds indexing) and have those operations throw
    // a custom exception type (or std::out_of_range / std::runtime_error)
    // instead of invoking undefined behavior. Instantiate it with at
    // least one concrete type, exercise normal operation, then
    // deliberately trigger the failure condition inside a try/catch
    // block and confirm the exception is caught with the expected
    // message/type.

    // TODO: at least 5 assert() checks (mix of normal-path assertions
    // and a boolean flag asserting the catch block actually ran).

    std::cout << "ok\n";
    return 0;
}
