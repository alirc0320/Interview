---
chapter: Chapter 2 — C++ Basics: Functions and Files
date_written: <YYYY-MM-DD>
---

# Chapter: C++ Basics: Functions and Files

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Why do we need forward declarations at all — what problem do they solve when a program is split across multiple files or when functions call each other out of definition order?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int add(int x, int y)
{
    return x + y;
}

int main()
{
    std::cout << add(3, 4) << '\n';
    std::cout << multiply(3, 4) << '\n';
    return 0;
}

int multiply(int x, int y)
{
    return x * y;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

int value{ 10 };

void modify()
{
    int value{ 99 };
    std::cout << value << '\n';
}

int main()
{
    modify();
    std::cout << value << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A project has `mathstuff.h` with function declarations but no header guard. Both `main.cpp` and `helper.cpp` `#include "mathstuff.h"`, and `helper.cpp` also `#include`s a third header that itself includes `mathstuff.h`. The build fails with redefinition errors. What's happening, and what single fix resolves it?
A:

## 5. Implementation
Q: Create a two-file program (plus a header): a header file declaring a function `int cube(int x);`, a `.cpp` file defining it, and a `main.cpp` that includes the header and prints the cube of a few different numbers. Use a header guard.
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
