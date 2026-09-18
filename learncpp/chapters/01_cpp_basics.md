---
chapter: Chapter 1 — C++ Basics
date_written: <YYYY-MM-DD>
---

# Chapter: C++ Basics

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What is the difference between a variable's definition, its initialization, and a later assignment to it — and why does the distinction matter for understanding what "uninitialized" means?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int main()
{
    int x = 5
    std::cout << "x is " << x << std::endl;
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
    int a{ 4 };
    int b{ 7 };
    std::cout << a << " " << b << '\n';
    a = b;
    b = 2;
    std::cout << a << " " << b << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A program declares `int total;` (no initializer), never assigns it a value, and then does `std::cout << total;`. What category of bug is this, why does the compiler usually not stop you, and what could you observe when you actually run it?
A:

## 5. Implementation
Q: Write a program that declares three int variables representing the price, quantity, and total cost of an item (initialize price and quantity with literals, compute total cost via multiplication and assignment), then prints all three values with clear labels using `std::cout`, each on its own line.
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
