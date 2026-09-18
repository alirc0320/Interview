---
chapter: Chapter 7 — Scope, Duration, and Linkage
date_written: <YYYY-MM-DD>
---

# Chapter: Scope, Duration, and Linkage

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What are the three separate properties (scope, duration, linkage) that determine how an identifier can be used, and why can two variables with the same name in the same file behave completely differently depending on how these three combine (e.g. a global vs. a local vs. a static local)?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

namespace Foo
{
    int value{ 5 };
}

namespace Bar
{
    int value{ 10 };
}

int main()
{
    std::cout << value << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

int x{ 1 };

void increment()
{
    static int x{ 100 };
    ++x;
    std::cout << x << '\n';
}

int main()
{
    increment();
    increment();
    {
        int x{ 42 };
        std::cout << x << '\n';
    }
    std::cout << x << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A project has `constants.h` defining `int gravity{ 10 };` at file scope with no `inline` or `static`, and this header is `#include`d in both `physics.cpp` and `main.cpp`. Linking fails with duplicate symbol errors for `gravity`. Separately, a teammate "fixes" it by marking `gravity` as `static` in the header instead of `inline constexpr` or `inline`. What's actually going wrong in each version, and what's the correct fix?
A:

## 5. Implementation
Q: Write a small program with a function `int nextId()` that returns a unique incrementing ID each time it's called (starting at 1), implemented using a static local variable — no global variable allowed. Call it 4-5 times from `main()` and print each returned ID to prove it's incrementing correctly across calls.
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
