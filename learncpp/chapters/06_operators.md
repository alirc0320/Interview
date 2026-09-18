---
chapter: Chapter 6 — Operators
date_written: <YYYY-MM-DD>
---

# Chapter: Operators

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Why is comparing two floating-point numbers with `==` generally unsafe, and what does this chapter suggest doing instead?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int main()
{
    int x{ 5 };
    int y{ x++ + ++x };
    std::cout << y << '\n';
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
    int a{ 7 };
    int b{ 2 };
    std::cout << (a % b) << '\n';
    std::cout << (a > b ? "bigger" : "smaller") << '\n';
    bool result{ (a > 0) && (b > 0) || (a == b) };
    std::cout << result << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A programmer writes `if (x = 5)` when they meant `if (x == 5)` inside a condition that's supposed to only sometimes be true. What actually happens when this code runs, and why doesn't the compiler always warn about it?
A:

## 5. Implementation
Q: Write a small program that takes two hard-coded integer variables and prints: their sum, difference, product, integer quotient, and remainder, then uses the conditional operator to print whichever of the two is larger (or "equal" if they're the same).
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
