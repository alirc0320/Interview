---
chapter: Chapter 12 — Compound Types: References and Pointers
date_written: <YYYY-MM-DD>
---

# Chapter: Compound Types: References and Pointers

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Explain the difference between pass by const lvalue reference and pass by address (const pointer) for a function parameter that should not be modified. Why does LearnCpp recommend the former for most non-fundamental types, and when would you still reach for pass by address instead?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int& getRef()
{
    int x{ 5 };
    return x;
}

int main()
{
    int& r{ getRef() };
    std::cout << r << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

void increment(int* ptr)
{
    if (ptr)
        ++(*ptr);
}

int main()
{
    int x{ 5 };
    int* p{ &x };
    increment(p);
    increment(nullptr);
    std::cout << x << '\n';

    int y{ 10 };
    int& ry{ x };
    ry = y;
    std::cout << x << ' ' << y << ' ' << &ry << ' ' << &x << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A teammate writes this helper to "optionally" give back a pointer to the largest of two ints, but the caller sometimes crashes or prints garbage when using the result later in the program:
```cpp
int* getLarger(int a, int b)
{
    int result{ (a > b) ? a : b };
    return &result;
}
```
Explain exactly what is wrong and why the failure might not show up immediately or consistently.
A:

## 5. Implementation
Q: Write a function `std::optional<int> findFirstNegative(const std::vector<int>& nums)` (or equivalent using a raw array/std::array if you haven't covered std::vector) that returns the first negative value found via pass by const reference and std::optional, and a `main()` that calls it with at least one vector containing a negative and one containing none, printing the result (or "none found") in each case using `has_value()`/`value()`.
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
