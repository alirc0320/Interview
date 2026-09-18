// Combined practice program for LearnCpp chapters 24 and 25
// (Inheritance; Virtual Functions).
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
    // TODO: Build a small polymorphic hierarchy, e.g. a base class
    // `Shape` (with a virtual destructor and a pure or regular virtual
    // `area()` / `name()` function) and two derived classes constructed
    // through the base's constructor (exercising chapter 24's
    // construction order / base-initialization rules). Store several
    // objects as base-class pointers (e.g. Shape*, or unique_ptr<Shape>)
    // and call the virtual functions through those base pointers to
    // exercise dynamic dispatch (chapter 25), then delete/let them go
    // out of scope to prove the virtual destructor runs the correct
    // derived cleanup.

    // TODO: at least 5 assert() checks (e.g. asserting computed areas,
    // asserting dynamic_cast succeeds/fails as expected, asserting a
    // side-effect counter incremented by each derived destructor).

    std::cout << "ok\n";
    return 0;
}
