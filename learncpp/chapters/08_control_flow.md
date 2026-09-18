---
chapter: Chapter 8 — Control Flow
date_written: <YYYY-MM-DD>
---

# Chapter: Control Flow

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Why does switch fallthrough happen by default in C++ (as opposed to each case automatically breaking), and what's the idiomatic way to signal that a fallthrough is intentional rather than a bug?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int main()
{
    int x{ 2 };
    switch (x)
    {
    case 1:
        int y{ 10 };
        std::cout << y << '\n';
        break;
    case 2:
        std::cout << "two\n";
        break;
    default:
        break;
    }
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
    int i{ 0 };
    while (i < 5)
    {
        if (i == 2)
        {
            ++i;
            continue;
        }
        if (i == 4)
            break;
        std::cout << i << ' ';
        ++i;
    }
    std::cout << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This function is supposed to print "small", "medium", or "large" depending on `x`, but it always prints all three that apply and sometimes prints nothing useful for negative numbers:
```cpp
void classify(int x)
{
    if (x < 0)
        std::cout << "negative\n";
    if (x < 10);
        std::cout << "small\n";
    if (x < 100)
        std::cout << "medium\n";
    std::cout << "large\n";
}
```
Find the bug(s) and explain what's actually happening.
A:

## 5. Implementation
Q: Write a program that uses Random.h-style Mersenne Twister generation (or `<random>` directly) to generate 10 random integers between 1 and 6 (simulating die rolls), prints each roll, and uses a for loop with a running total plus a switch or if-chain to also print how many rolls came up as a 6.
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
