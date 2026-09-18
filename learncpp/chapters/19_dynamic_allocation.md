---
chapter: Chapter 19 — Dynamic Allocation
date_written: <YYYY-MM-DD>
---

# Chapter: Dynamic Allocation

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Explain the difference between `new`/`delete` and `new[]`/`delete[]`. Why must an array allocated with `new[]` be freed with `delete[]` rather than plain `delete`, and what goes wrong (conceptually) if you mismatch them?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int main()
{
    int size{};
    std::cin >> size;
    int arr[size]{};
    std::cout << arr[0] << '\n';
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
    int n{ 4 };
    int* arr{ new int[n] };
    for (int i{ 0 }; i < n; ++i)
        arr[i] = i * i;

    int sum{ 0 };
    for (int i{ 0 }; i < n; ++i)
        sum += arr[i];

    std::cout << sum << '\n';
    std::cout << arr[0] << ' ' << *(arr + 1) << ' ' << arr[2] << '\n';

    delete[] arr;
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A teammate's program crashes intermittently with a heap corruption error. Here's the relevant code:
```cpp
int* p{ new int{ 5 } };
int* q{ p };
delete p;
delete q;
```
Explain exactly what is wrong, and describe one technique that would have made this bug harder to write in the first place.
A:

## 5. Implementation
Q: Implement a small class `IntBuffer` that manages a dynamically allocated `int` array: a constructor taking a size that allocates the array with `new[]`, a destructor that frees it with `delete[]`, and methods to set and get an element by index. Write a `main()` that creates an `IntBuffer`, fills it, reads values back, and lets it go out of scope, demonstrating that there is no leak (e.g. by reasoning about it under AddressSanitizer).
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
