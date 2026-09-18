// Practice program for LearnCpp chapter 28 — Input and Output (I/O).
// One program covering ideas from the chapter, 20-40 lines, at
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
    // TODO: Write a program that opens a file (e.g. via std::ofstream)
    // and writes a handful of records (e.g. a few lines of "name score"
    // pairs, or a small binary struct), closes it, then reopens it
    // (std::ifstream) and reads the data back. Validate the stream
    // state after each read (good()/fail()/eof()) and, deliberately,
    // attempt to read past the end of the data or read a malformed
    // value once to exercise stream failure + clear()/recovery.

    // TODO: at least 5 assert() checks (e.g. asserting values read back
    // match what was written, asserting stream state flags are set as
    // expected after a bad read, asserting clear() restores good()).

    std::cout << "ok\n";
    return 0;
}
