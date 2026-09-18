---
chapter: Chapter 17 — Fixed-size arrays: std::array and C-style arrays
date_written: <YYYY-MM-DD>
---

# Chapter: Fixed-size arrays: std::array and C-style arrays

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What does it mean for a C-style array to "decay" to a pointer? What specific piece of information is lost when this happens, and why does `std::array` not have this problem?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int main()
{
    char* str{ "hello" };
    str[0] = 'H';
    std::cout << str << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

void printSize(int arr[])
{
    std::cout << sizeof(arr) << '\n';
}

int main()
{
    int arr[10]{};
    std::cout << sizeof(arr) << '\n';
    printSize(arr);
    return 0;
}
```
(Assume a 64-bit system where `int` is 4 bytes and pointers are 8 bytes.)
A:

## 4. Debugging / undefined behavior
Q: A teammate can't figure out why this function returns garbage every time it's called:
```cpp
int* makeArray()
{
    int arr[5]{ 1, 2, 3, 4, 5 };
    return arr;
}
```
Explain exactly what is wrong.
A:

## 5. Implementation
Q: Write a function `int sumArray(const std::array<int, 5>& arr)` that returns the sum of its elements, and a second function `int sumCArray(const int* arr, int length)` that does the same thing for a C-style array. Write a `main()` that builds a `std::array<int, 5>` and an `int[5]` with the same five values, calls each function, and prints both sums. Be ready to explain why `sumCArray` needs an explicit `length` parameter while `sumArray` does not.
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
