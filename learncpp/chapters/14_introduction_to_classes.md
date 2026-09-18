---
chapter: Chapter 14 — Introduction to Classes
date_written: <YYYY-MM-DD>
---

# Chapter: Introduction to Classes

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What problem does a constructor member initializer list solve that assignment inside the constructor body cannot (or cannot do as well), and why does it matter for members that are references, const, or class types without a default constructor?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

class Fraction
{
private:
    int m_numerator{};
    int m_denominator{ 1 };

public:
    Fraction(int numerator, int denominator)
        : m_numerator{ numerator }, m_denominator{ denominator }
    {
    }

    void print() const
    {
        std::cout << m_numerator << '/' << m_denominator << '\n';
    }
};

int main()
{
    Fraction f{ 3, 4 };
    f.print();

    const Fraction g{ 1, 2 };
    g.print();

    Fraction h;
    h.print();
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

class Counter
{
private:
    int m_value{};

public:
    Counter(int value) : m_value{ value }
    {
        std::cout << "construct " << m_value << '\n';
    }

    Counter(const Counter& other) : m_value{ other.m_value }
    {
        std::cout << "copy " << m_value << '\n';
    }
};

Counter makeCounter()
{
    Counter c{ 42 };
    return c;
}

int main()
{
    Counter a{ makeCounter() };
    Counter b{ a };
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This class is meant to wrap a temperature reading, but users report that after constructing a `Reading` with only a location name, calling `getCelsius()` on it gives an unpredictable value instead of a sensible default:
```cpp
class Reading
{
private:
    std::string m_location;
    double m_celsius;

public:
    Reading(std::string location) : m_location{ location }
    {
    }

    double getCelsius() const { return m_celsius; }
};
```
Explain the bug and how the chapter's constructor/initialization features fix it.
A:

## 5. Implementation
Q: Write a `class BankAccount` with private members for owner name (`std::string`) and balance (`double`), a constructor with a default argument for an opening balance of 0.0, a delegating constructor that takes only an owner name, public member functions `deposit(double)` and `withdraw(double)` (withdraw should refuse to overdraw), and a const `getBalance()` accessor. Demonstrate creating accounts both ways and performing a few deposits/withdrawals in `main()`.
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
