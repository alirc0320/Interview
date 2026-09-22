// Practice program for learncpp/chapters/12b_pointers_advanced_embedded.md
// (custom addition — not a real LearnCpp chapter pairing, so this is solo
// rather than combined with a neighboring chapter).
// 20-40 lines, at least 5 assert() checks, heavy on pointer arithmetic /
// byte-buffer parsing / function-pointer dispatch, embedded-flavored.
//
// Compile:
//   clang++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
//     -g -O0 -fsanitize=address,undefined practice.cpp -o practice
// Run:
//   ./practice

#include <cassert>
#include <cstdint>
#include <iostream>

int main()
{
    // TODO: write a small little-endian byte-buffer parser using raw
    // pointer arithmetic over a std::uint8_t* (no reinterpret_cast to a
    // struct pointer) that decodes a 2-byte ID and a 4-byte length from a
    // hand-built byte array, mirroring 12b question 8.

    // TODO: write a fixed-size array of function pointers (or captureless
    // lambdas) that dispatches on an opcode byte, mirroring 12b question 9,
    // and call it for a couple of opcodes including one out-of-range value
    // that should be safely rejected.

    // TODO: at least 5 assert() checks covering: correct decode of the
    // parsed header fields, that an undersized buffer is rejected, that
    // in-range dispatch calls the right handler, and that an out-of-range
    // opcode does not call anything and reports failure.

    std::cout << "ok\n";
    return 0;
}
