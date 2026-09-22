---
chapter: Chapter 12 — Compound Types: References and Pointers
date_written: <YYYY-MM-DD>
---

# Chapter: Compound Types: References and Pointers

Close the chapter. Write these from memory. Do not look anything up.

This chapter gets more questions than most (10 instead of 5) and leans
harder on coding: pointers are the single most likely deep-dive topic in an
embedded/accelerator interview, and interviewers will push past "can you
define a pointer" into const-correctness, lifetime bugs, and multi-level
indirection. See `12b_pointers_advanced_embedded.md` for pointer
arithmetic, function pointers as data, void*/reinterpret_cast, and
volatile/MMIO-flavored questions that don't fit this LearnCpp chapter's
scope but come up constantly in this kind of interview.

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

## 6. Concept explanation — const and pointers
Q: There are three distinct ways to combine `const` with a pointer declaration: pointer to non-const, pointer to const, and const pointer (and const pointer to const). Write out the four declarations of a pointer to an `int x` using each combination, state in each case whether you can reassign the pointer itself and whether you can modify the pointee through it, and explain the "read right-to-left" trick for parsing declarations like `const int* const p`.
A:

## 7. Does this compile?
Q:
```cpp
#include <iostream>

void modify(const int* ptr)
{
    *ptr = 10;
}

int process(int* const ptr)
{
    *ptr = 20;
    return *ptr;
}

int main()
{
    int x{ 5 };
    const int* p1{ &x };
    p1 = nullptr;

    int* const p2{ &x };
    p2 = nullptr;

    std::cout << process(&x) << '\n';
    return 0;
}
```
A: (yes/no, and why — identify every line that is or isn't a compile error, not just the first one)

## 8. Predict the output — pointer to pointer
Q:
```cpp
#include <iostream>

void reseat(int** ppInt, int* newTarget)
{
    *ppInt = newTarget;
}

int main()
{
    int a{ 1 };
    int b{ 2 };
    int* p{ &a };
    int** pp{ &p };

    std::cout << **pp << '\n';
    reseat(pp, &b);
    std::cout << **pp << ' ' << *p << '\n';

    **pp = 99;
    std::cout << a << ' ' << b << '\n';
    return 0;
}
```
A:

## 9. Debugging / undefined behavior — dangling and double free
Q: A colleague's cleanup helper is meant to free a buffer and reset the caller's pointer so it can't be used again by mistake, but a crash still shows up later, sometimes in a completely unrelated function:
```cpp
void freeBuffer(int* buf)
{
    delete[] buf;
    buf = nullptr;
}

int main()
{
    int* data{ new int[100] };
    // ... use data ...
    freeBuffer(data);
    // ... much later ...
    data[0] = 5;   // still crashes/corrupts, even though freeBuffer "reset" it
    return 0;
}
```
Explain precisely why `buf = nullptr` inside `freeBuffer` does nothing useful for the caller, what category of bug `data[0] = 5` is after the `delete[]`, and rewrite `freeBuffer` (changing its signature if needed) so that `data` in `main` is actually null afterward.
A:

## 10. Implementation — swap and array bounds via pointers
Q: Write `void swapViaPointers(int* a, int* b)` that swaps the two pointees (not the pointers) with no temporary beyond one `int`, and a second function `int* findMax(int* first, int* last)` that scans the half-open range `[first, last)` (pointer, not index, based — as if walking a raw array with two pointers the way `std::max_element` does) and returns a pointer to the maximum element, or `nullptr` if the range is empty (`first == last`). In `main()`, build a small `int` array, get `std::begin`/`std::end`-style pointers into it (`arr` and `arr + size`), call `findMax`, dereference the result to print it, and demonstrate `swapViaPointers` swapping two elements of that same array by passing `&arr[i]`/`&arr[j]`.
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
