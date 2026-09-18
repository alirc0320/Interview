---
chapter: Chapter 24 — Inheritance
date_written: <YYYY-MM-DD>
---

# Chapter: Inheritance

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: When a derived class object is constructed, explain the full order of operations: which base subobject constructor runs, when member initializers run, and where in a derived constructor's initializer list you must place a call to a non-default base constructor. Why can't a derived class initialize an inherited (base-class) member directly in its own member-initializer list?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

class Animal
{
protected:
    std::string m_name;

public:
    Animal(const std::string& name) : m_name{ name } {}
};

class Dog : public Animal
{
private:
    std::string m_breed;

public:
    Dog(const std::string& name, const std::string& breed)
        : m_breed{ breed }, Animal{ name }
    {
    }

    void print() const
    {
        std::cout << m_name << " is a " << m_breed << '\n';
    }
};

int main()
{
    Dog d{ "Rex", "Labrador" };
    d.print();
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
    Base() { std::cout << "Base ctor\n"; }
    ~Base() { std::cout << "Base dtor\n"; }
};

class Middle : public Base
{
public:
    Middle() { std::cout << "Middle ctor\n"; }
    ~Middle() { std::cout << "Middle dtor\n"; }
};

class Derived : public Middle
{
public:
    Derived() { std::cout << "Derived ctor\n"; }
    ~Derived() { std::cout << "Derived dtor\n"; }
};

int main()
{
    std::cout << "creating\n";
    {
        Derived d{};
    }
    std::cout << "done\n";
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A colleague writes this class hierarchy and is confused why `getStealth()` "hides" `getHealth()` in a way they didn't expect, and separately why changing `Base::m_health` to `private` broke `Derived::heal()`:
```cpp
class Base
{
private:
    int m_health{ 100 };

public:
    int getHealth() const { return m_health; }
};

class Derived : public Base
{
public:
    void heal(int amount) { m_health += amount; }
    int getHealth(int bonus) const { return Base::getHealth() + bonus; }
};
```
Explain both problems: why `Derived::heal` fails to compile, and what happens to `Base::getHealth()` (name hiding) once `Derived` declares its own `getHealth(int)` overload.
A:

## 5. Implementation
Q: Write a base class `Shape` with a protected `std::string m_name`, a constructor, and a member function `describe() const` that prints the name. Derive `Circle` (adds a `double m_radius`, a constructor that calls the base constructor, and an `area() const` member function) and `Rectangle` (adds `double m_width, m_height`, a constructor, and `area() const`). Also demonstrate multiple inheritance by writing a small `Timestamped` class (stores an `int m_createdTick`) and a `LoggedCircle : public Circle, public Timestamped` that combines both. Demonstrate all classes in `main()`.
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
