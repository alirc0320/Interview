---
chapter: Chapter 9 — Error Detection and Handling
date_written: <YYYY-MM-DD>
---

# Chapter: Error Detection and Handling

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What's the difference between `assert` and `static_assert` — when does each one actually run, what happens on failure, and why would you pick one over the other for validating a precondition?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

constexpr int getBufferSize()
{
    return -5;
}

int main()
{
    static_assert(getBufferSize() > 0, "Buffer size must be positive");
    std::cout << "ok\n";
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>
#include <limits>

int main()
{
    std::cout << "Enter a number: ";
    int x{};
    if (!(std::cin >> x))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "invalid\n";
    }
    else
    {
        std::cout << "you entered " << x << '\n';
    }
    return 0;
}
```
(Assume the user types `abc` and presses enter.) What gets printed?
A:

## 4. Debugging / undefined behavior
Q: A function is supposed to divide two integers safely:
```cpp
int safeDivide(int numerator, int denominator)
{
    assert(denominator != 0 && "Division by zero");
    return numerator / denominator;
}
```
In the release build (compiled with `NDEBUG` defined), a user manages to trigger a division-by-zero crash even though this assert exists and presumably would have caught it in debug builds. Why does this happen, and how would you make the function robust in both debug and release builds?
A:

## 5. Implementation
Q: Write a program that repeatedly prompts the user to enter an integer between 1 and 10 (inclusive), using `std::cin` validation to reject non-numeric input and an explicit range check to reject out-of-range numbers, looping until valid input is received, then prints the accepted value. Include at least one `assert` verifying an internal invariant (e.g. that the accepted value really is in range before printing).
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
