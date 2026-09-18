---
chapter: Chapter 27 — Exceptions
date_written: <YYYY-MM-DD>
---

# Chapter: Exceptions

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Explain stack unwinding: when an exception is thrown and propagates out of a function, what exactly happens to local objects on the stack between the throw point and the matching catch block, and in what order? Then explain why catch blocks should generally catch exceptions by (const) reference rather than by value, specifically in terms of what happens to a derived-type exception object when caught by value as its base type.
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>
#include <stdexcept>

class NetworkError : public std::runtime_error
{
public:
    NetworkError(const std::string& msg) : std::runtime_error{ msg } {}
};

void connect(bool fail)
{
    if (fail)
        throw NetworkError{ "connection refused" };
    std::cout << "connected\n";
}

int main()
{
    try
    {
        connect(true);
    }
    catch (std::runtime_error e)
    {
        std::cout << "caught: " << e.what() << '\n';
    }
    catch (NetworkError e)
    {
        std::cout << "network issue: " << e.what() << '\n';
    }
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

class Guard
{
private:
    std::string m_name;

public:
    Guard(const std::string& name) : m_name{ name } { std::cout << "acquire " << m_name << '\n'; }
    ~Guard() { std::cout << "release " << m_name << '\n'; }
};

void inner()
{
    Guard g{ "inner" };
    throw std::runtime_error{ "boom" };
}

void outer()
{
    Guard g{ "outer" };
    inner();
    std::cout << "unreachable\n";
}

int main()
{
    try
    {
        outer();
    }
    catch (const std::exception& e)
    {
        std::cout << "caught: " << e.what() << '\n';
    }
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This function is marked `noexcept` for performance reasons, but the program terminates unexpectedly in production instead of the caller's `catch` block ever running. Explain what happens and why:
```cpp
#include <vector>

int computeChecksum(const std::vector<int>& data) noexcept
{
    int sum = 0;
    for (int v : data.at(0) == 0 ? data : data)  // simplified stand-in for indexing logic
        sum += v;
    return data.at(sum % static_cast<int>(data.size()));  // may throw std::out_of_range
}
```
A:

## 5. Implementation
Q: Write a small `BankAccount` class whose `withdraw(double amount)` throws a custom exception type `InsufficientFundsError` (derived from `std::runtime_error`, carrying the requested amount and available balance) when `amount > balance`, and throws a different custom exception `InvalidAmountError` when `amount <= 0`. In `main()`, call `withdraw` several times with a `try` block that has separate `catch` clauses for each specific exception type plus a final `catch (const std::exception&)` fallback, printing a distinct message for each case. Demonstrate that exceptions are still caught correctly after being rethrown once from an intermediate helper function using `throw;`.
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
