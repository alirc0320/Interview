---
chapter: Chapter 20 — Functions
date_written: <YYYY-MM-DD>
---

# Chapter: Functions

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: A function pointer's type must match the signature of the function it points to. Explain what "matching" means here (return type, parameter types, and any qualifiers), and describe a concrete case where a function pointer assignment or call would fail to compile because of a mismatch. Then explain how a lambda with no captures can be assigned to a plain function pointer, but a lambda with captures cannot.
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int add(int x, int y) { return x + y; }
int multiply(int x, int y) { return x * y; }

int main()
{
    int (*opFunc)(int, int){ add };
    std::cout << opFunc(2, 3) << '\n';

    opFunc = multiply;
    std::cout << opFunc(2, 3) << '\n';

    double (*badFunc)(int, int){ add };
    std::cout << badFunc(2, 3) << '\n';

    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

int fib(int n)
{
    if (n <= 1)
        return n;
    std::cout << "computing fib(" << n << ")\n";
    return fib(n - 1) + fib(n - 2);
}

int main()
{
    int x{ 10 };
    auto addX = [x](int y) { return x + y; };
    x = 100;

    std::cout << addX(1) << '\n';
    std::cout << fib(5) << '\n';

    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A developer writes a function that returns a lambda meant to capture a local counter by reference so callers can increment a "shared" counter across calls:
```cpp
#include <functional>

std::function<int()> makeCounter()
{
    int count{ 0 };
    auto increment = [&count]() { return ++count; };
    return increment;
}
```
The caller does `auto counter{ makeCounter() }; std::cout << counter() << counter() << counter();` and gets garbage/UB instead of `1`, `2`, `3`. Explain the bug and how you'd fix it.
A:

## 5. Implementation
Q: Write a small command-line-driven calculator: define four functions `add`, `subtract`, `multiply`, `divide` (each `double(double, double)`), and a function `getOp(char symbol)` that returns the matching function pointer (or `nullptr` for an unknown symbol) using a lambda-free lookup (an if/else chain or array of pairs is fine). In `main`, take two numbers and an operator symbol as command line arguments (`argc`/`argv`), look up the function pointer, and print the result — handling divide-by-zero and an unrecognized operator gracefully instead of crashing.
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
