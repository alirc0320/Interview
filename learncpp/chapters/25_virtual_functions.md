---
chapter: Chapter 25 — Virtual Functions
date_written: <YYYY-MM-DD>
---

# Chapter: Virtual Functions

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Explain how the virtual table (vtable) mechanism achieves runtime polymorphism: what gets stored per-class vs per-object, what a "vptr" is, how a call through a base-class pointer/reference is resolved at runtime, and why this makes virtual calls slower than non-virtual calls. Then explain precisely why a base class with any virtual function (or that is meant to be used polymorphically) needs a virtual destructor, and what specifically goes wrong at runtime if it doesn't have one.
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

class Shape
{
public:
    virtual double area() const = 0;
    virtual void print() const { std::cout << "Shape, area=" << area() << '\n'; }
};

class Square : public Shape
{
private:
    double m_side;

public:
    Square(double side) : m_side{ side } {}
    double area() const override { return m_side * m_side; }
};

int main()
{
    Shape s{};
    Square sq{ 4.0 };
    sq.print();
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

class Base
{
public:
    virtual void speak() const { std::cout << "Base speaks\n"; }
    void greet() const
    {
        std::cout << "Greeting: ";
        speak();
    }
};

class Derived : public Base
{
public:
    void speak() const override { std::cout << "Derived speaks\n"; }
};

void announce(const Base& b)
{
    b.speak();
}

int main()
{
    Derived d{};
    Base& br{ d };
    br.greet();
    announce(d);
    announce(br);
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This code compiles and "seems to work" under casual testing, but running it under AddressSanitizer reports a leak (and on some compilers, worse UB). Find and explain the bug:
```cpp
class Resource
{
public:
    Resource() { std::cout << "acquire\n"; }
    ~Resource() { std::cout << "release\n"; }
};

class Base
{
public:
    virtual void use() { std::cout << "using base\n"; }
};

class Derived : public Base
{
private:
    Resource m_res;

public:
    void use() override { std::cout << "using derived\n"; }
};

int main()
{
    Base* b = new Derived{};
    b->use();
    delete b;
    return 0;
}
```
A:

## 5. Implementation
Q: Design an abstract base class `Employee` with a pure virtual `double monthlyPay() const` and a virtual `void printInfo() const` (prints name and pay, calling `monthlyPay()` polymorphically), plus a virtual destructor. Derive `SalariedEmployee` (fixed monthly salary) and `HourlyEmployee` (hourly rate × hours worked, stored as members). Store a collection of `std::unique_ptr<Employee>` (mixing both derived types) and call `printInfo()` on each through the base pointer to demonstrate dynamic dispatch. Also write one function that takes a `const Employee&` and uses `dynamic_cast` to check if it's specifically a `HourlyEmployee`, printing a special message if so.
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
