// Combined practice program for LearnCpp chapters 22 and 23
// (Move Semantics and Smart Pointers; Object Relationships).
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
    // TODO: Define a small "part" class (e.g. Engine) and a composed
    // "whole" class (e.g. Car) that owns its part through a
    // std::unique_ptr<Engine> member, modeling composition (chapter 23:
    // the Engine's lifetime is tied to the Car's). Give the whole class
    // an explicit move constructor and move assignment operator (chapter
    // 22) that transfer the unique_ptr, and delete (or explicitly omit)
    // copy construction/assignment since it uniquely owns its part.
    // Demonstrate constructing one instance, moving it into another with
    // std::move, and confirming the moved-from object's pointer is null
    // while the moved-to object now owns the part.

    // TODO: at least 5 assert() checks.

    std::cout << "ok\n";
    return 0;
}
