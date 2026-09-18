---
chapter: Chapter F — Constexpr Functions
date_written: <YYYY-MM-DD>
---

# Chapter: Constexpr Functions

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What's the difference between `constexpr` and `consteval` on a function, and why does a `constexpr` function still need to be usable at runtime (unlike `consteval`) even though it's allowed to be evaluated at compile time?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

consteval int square(int x)
{
    return x * x;
}

int main()
{
    int n{};
    std::cin >> n;
    std::cout << square(n) << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

constexpr int factorial(int n)
{
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

int main()
{
    constexpr int a{ factorial(5) };
    int x{ 4 };
    int b{ factorial(x) };
    std::cout << a << ' ' << b << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A teammate writes this function expecting it to always be evaluated at compile time since it's marked `constexpr`, and is confused when a profiler shows it running at runtime in one part of the program:
```cpp
constexpr int cube(int x)
{
    return x * x * x;
}

void report(int userInput)
{
    int result{ cube(userInput) };
    std::cout << result << '\n';
}
```
Explain why `cube` doesn't necessarily run at compile time here, and what would force it to.
A:

## 5. Implementation
Q: Write a `constexpr` function `bool isPrime(int n)` that determines whether `n` is prime using a simple loop (no library calls). In `main()`, use it in a `constexpr` context (e.g. `static_assert` or a `constexpr` variable) for at least one compile-time check, and also call it with a non-constant runtime variable to show it works at runtime too.
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
