---
chapter: Chapter 11 — Function Overloading and Function Templates
date_written: <YYYY-MM-DD>
---

# Chapter: Function Overloading and Function Templates

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: How does the compiler pick which overload to call when multiple overloads could match a given call (the ranking of exact match, promotion, conversion), and under what circumstances does this process fail with an "ambiguous call" error instead of just picking the best one?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

void print(int x)
{
    std::cout << "int: " << x << '\n';
}

void print(double x)
{
    std::cout << "double: " << x << '\n';
}

int main()
{
    print('a');
    print(5.0f);
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

template <typename T>
T maxOf(T a, T b)
{
    return (a > b) ? a : b;
}

int maxOf(int a, int b)
{
    std::cout << "non-template used\n";
    return (a > b) ? a : b;
}

int main()
{
    std::cout << maxOf(3, 7) << '\n';
    std::cout << maxOf(3.5, 7.5) << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A project declares a function template in a header and calls it from two different `.cpp` files:
```cpp
// mathutils.h
template <typename T>
T square(T x);

// mathutils.cpp
template <typename T>
T square(T x) { return x * x; }
```
`main.cpp` includes `mathutils.h` and calls `square(5)`. The build fails at the link stage with an "undefined reference to square<int>" error even though everything compiles individually. What's happening, and what's the fix?
A:

## 5. Implementation
Q: Write a function template `T clampValue(T value, T lo, T hi)` that returns `value` clamped into the range `[lo, hi]`. Use a non-type template parameter or a default argument to make the clamping behavior optionally invertible (your choice), and demonstrate it in `main()` with at least two different types (e.g. `int` and `double`).
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
