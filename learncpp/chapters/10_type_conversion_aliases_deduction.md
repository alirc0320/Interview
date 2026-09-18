---
chapter: Chapter 10 — Type Conversion, Type Aliases, and Type Deduction
date_written: <YYYY-MM-DD>
---

# Chapter: Type Conversion, Type Aliases, and Type Deduction

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What's a narrowing conversion, why does list initialization (brace init) treat narrowing conversions differently from copy/direct initialization, and what's a situation where a narrowing conversion is still allowed inside braces?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

int main()
{
    int x{ 10 };
    double d{ x };

    int y{ 4.5 };

    std::cout << d << ' ' << y << '\n';
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
    auto a{ 5 };
    auto b{ 5.0 };
    auto c{ a + b };

    char ch{ 'A' };
    auto d{ ch + 1 };

    std::cout << c << ' ' << d << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A teammate writes this and is confused why it doesn't compile with `-Wconversion` warnings turned into errors, and separately why the logic is wrong even ignoring the warning:
```cpp
unsigned int getPlayerCount()
{
    int count{ -1 };
    return count;
}

void printHealth(unsigned int health)
{
    int remaining{ health - 100 };
    std::cout << remaining << '\n';
}
```
What's going wrong here, in terms of the conversions taking place?
A:

## 5. Implementation
Q: Write a program with a type alias `using Meters = double;` and a function template-free function `double convertToFeet(Meters m)` that converts meters to feet (1 meter = 3.28084 feet). In `main()`, use `auto` to declare a few `Meters` values via the alias, call the function, and print results, relying on type deduction and static_cast where an explicit narrowing conversion is needed.
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
