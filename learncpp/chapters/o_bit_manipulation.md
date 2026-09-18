---
chapter: Chapter O — Bit Manipulation (optional chapter)
date_written: <YYYY-MM-DD>
---

# Chapter: Bit Manipulation

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What is a "bit mask," and why is it useful to name individual mask values with constants (or a `std::bitset`) rather than sprinkling raw hex/binary literals through your code?
A:

## 2. Does this compile?
Q:
```cpp
#include <bitset>
#include <iostream>

int main()
{
    std::bitset<8> flags{ 0b0000'0101 };
    flags.set(3);
    flags <<= 2;
    std::cout << flags << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

int main()
{
    unsigned int x{ 0b0000'1100 };
    unsigned int mask{ 0b0000'0100 };
    std::cout << (x & mask) << '\n';
    std::cout << (x | mask) << '\n';
    std::cout << (x ^ mask) << '\n';
    std::cout << (~mask & 0b0000'1111) << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A programmer wants to check whether bit 3 (0-indexed) of an `int` is set, and writes `if (value & 3)`. This compiles and "usually seems to work" during quick testing but gives wrong answers for most inputs. What's the actual bug, and what should the mask have been?
A:

## 5. Implementation
Q: Write a small program that uses a `std::bitset<8>` (or manual bitwise operators with named mask constants) to represent 8 independent boolean flags, sets a few specific flags on, clears one, toggles another, and prints the final bit pattern plus whether a specific flag is set.
A:

---

## Corrections
Reopen the chapter. Fix anything wrong above (don't erase the original wrong
answer — strike it through or note what was wrong, so the mistake stays
visible on review).

-

## Missed on first pass?
- [ ] Yes — copy the missed question(s) into `review/missed_questions.md`
- [ ] No
