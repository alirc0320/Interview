---
chapter: Chapter 5 — Constants and Strings
date_written: <YYYY-MM-DD>
---

# Chapter: Constants and Strings

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What is the difference between `const` and `constexpr`, and why can't every `const` variable be used in a context that requires a compile-time constant expression?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>
#include <string_view>

std::string_view getGreeting()
{
    std::string_view greeting{ "hello" };
    return greeting;
}

int main()
{
    std::cout << getGreeting() << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>
#include <string>

int main()
{
    std::string name{ "Ada" };
    std::string greeting{ "Hello, " + name + "!" };
    std::cout << greeting << '\n';
    std::cout << greeting.length() << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A function returns a `std::string_view` that was initialized from a temporary `std::string` created inside that same function (e.g. `std::string_view sv{ std::string("temp") + "data" };` returned by value). The caller prints the returned view and gets garbage. What's the underlying lifetime problem?
A:

## 5. Implementation
Q: Write a program that declares a `constexpr` int for a fixed array-like size (e.g. max score), a regular `const` `std::string` for a player's name, and a `std::string_view` that views a substring/portion of a literal, then prints all three along with the numeral-system value of the constexpr int in hex using `std::cout`'s hex formatting.
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
