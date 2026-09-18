---
chapter: Chapter 13 — Compound Types: Enums and Structs
date_written: <YYYY-MM-DD>
---

# Chapter: Compound Types: Enums and Structs

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Why does LearnCpp recommend scoped enumerations (`enum class`) over unscoped enumerations for new code, and what specifically changes about name lookup and implicit conversion to int when you switch from `enum` to `enum class`?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

struct Point3D
{
    double x{};
    double y{};
    double z{};
};

enum class Color
{
    red,
    green,
    blue,
};

int main()
{
    Point3D p{ 1.0, 2.0 };
    Color c{ Color::red };
    if (c == 0)
        std::cout << "red\n";
    std::cout << p.x << ' ' << p.y << ' ' << p.z << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

struct Employee
{
    int id{};
    double salary{ 50000.0 };
};

Employee raise(Employee e, double amount)
{
    e.salary += amount;
    return e;
}

int main()
{
    Employee alice{ 1 };
    Employee bob{ raise(alice, 5000.0) };

    std::cout << alice.salary << ' ' << bob.salary << '\n';

    Employee* pAlice{ &alice };
    pAlice->salary += 1000.0;
    std::cout << alice.salary << ' ' << (*pAlice).salary << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This struct and function compile and often "seem to work" in a quick test, but printing an `Inventory` right after default-constructing one (before any values are assigned) sometimes prints a wildly wrong `count`, and the behavior differs between debug and release builds:
```cpp
struct Inventory
{
    std::string name;
    int count;
};

void printInventory(const Inventory& inv)
{
    std::cout << inv.name << ": " << inv.count << '\n';
}
```
Explain the bug and how the chapter's tools for structs would prevent it.
A:

## 5. Implementation
Q: Define an `enum class Direction { north, south, east, west }`, write a function that converts a `Direction` to its string name (e.g. via switch) and one that overloads `operator<<` to print a `Direction` directly with `std::cout`. Also define a `struct Position { int x; int y; }` with default member initializers, write a function `move(Position pos, Direction dir)` that returns a new `Position` shifted one unit in that direction, and demonstrate all of it in `main()` for each of the four directions.
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
