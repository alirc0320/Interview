---
chapter: Chapter 4 — Fundamental Data Types
date_written: <YYYY-MM-DD>
---

# Chapter: Fundamental Data Types

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Why does this chapter recommend avoiding unsigned integers for holding quantities in most cases, even though they seem like a natural fit for values that "can't be negative"?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int main()
{
    unsigned int a{ 3 };
    int b{ 5 };
    int result{ a - b };
    std::cout << result << '\n';
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
    char c{ 'A' };
    int i{ c + 1 };
    std::cout << i << '\n';
    std::cout << static_cast<char>(i) << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A function computes `unsigned int size = a.size() - b.size();` where both are `std::size_t` (unsigned), intending it to represent "how many more elements `a` has than `b`." When `b` has more elements than `a`, the program doesn't crash but later logic that uses `size` behaves bizarrely (e.g. a loop runs billions of times). What's going wrong under the hood?
A:

## 5. Implementation
Q: Write a small program that declares one variable of each of: `int`, `unsigned int`, `double`, `bool`, and `char`, prints the `sizeof` each one, and then uses `static_cast` to convert a `double` holding a fractional value into an `int` and prints the result, so you can see the truncation happen.
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
