---
chapter: Chapter 23 — Object Relationships
date_written: <YYYY-MM-DD>
---

# Chapter: Object Relationships

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Distinguish composition, aggregation, and association in terms of (a) ownership/lifetime dependency and (b) how strongly the parts are coupled to the whole. Give a one-sentence example relationship for each (e.g. "a `Car` and its `Engine`") and explain why the choice affects whether a member should be stored by value, by pointer/reference, or by `std::unique_ptr`/`std::shared_ptr`.
A:

## 2. Does this compile?
Q:
```cpp
#include <initializer_list>
#include <iostream>
#include <vector>

class IntList
{
private:
    std::vector<int> m_list;

public:
    IntList(std::initializer_list<int> list) : m_list{ list } {}

    int operator[](int index) const { return m_list[index]; }
    int size() const { return static_cast<int>(m_list.size()); }
};

int main()
{
    IntList list{ 1, 2, 3, 4 };
    for (int i = 0; i < list.size(); ++i)
        std::cout << list[i] << ' ';
    std::cout << '\n';

    IntList empty{};
    std::cout << empty.size() << '\n';

    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>
#include <string>

class Engine
{
private:
    std::string m_name;

public:
    Engine(std::string name) : m_name{ name }
    {
        std::cout << "Engine " << m_name << " built\n";
    }
    ~Engine()
    {
        std::cout << "Engine " << m_name << " destroyed\n";
    }
};

class Car
{
private:
    Engine m_engine;

public:
    Car(std::string engineName) : m_engine{ engineName }
    {
        std::cout << "Car built\n";
    }
    ~Car()
    {
        std::cout << "Car destroyed\n";
    }
};

int main()
{
    std::cout << "start\n";
    {
        Car c{ "V8" };
    }
    std::cout << "end\n";
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A `Department` is modeled as an aggregation of `Employee*` (it doesn't own them — employees can exist and move between departments independently):
```cpp
#include <vector>
#include <iostream>
#include <string>

class Employee
{
public:
    std::string m_name;
    Employee(std::string name) : m_name{ name } {}
};

class Department
{
private:
    std::vector<Employee*> m_employees;

public:
    void add(Employee* e) { m_employees.push_back(e); }
    void printAll() const
    {
        for (auto* e : m_employees)
            std::cout << e->m_name << '\n';
    }
};

Department makeSalesDept()
{
    Department d;
    Employee alice{ "Alice" };
    d.add(&alice);
    return d;
}

int main()
{
    Department d{ makeSalesDept() };
    d.printAll();
    return 0;
}
```
This has undefined behavior. Identify the bug and explain how the composition-vs-aggregation choice in this design is what makes it easy to introduce.
A:

## 5. Implementation
Q: Write a `class Playlist` that models composition over `Song` value objects: `Song` holds a title (`std::string`) and duration in seconds (`int`); `Playlist` owns a `std::vector<Song>` internally (not pointers), supports construction from a `std::initializer_list<Song>`, an `addSong(Song)` method, a `totalDuration() const` method summing all song durations, and an `operator<<` that prints every song title and its duration. Demonstrate building a playlist both from an initializer list and via `addSong` in `main()`.
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
