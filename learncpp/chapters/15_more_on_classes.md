---
chapter: Chapter 15 — More on Classes
date_written: <YYYY-MM-DD>
---

# Chapter: More on Classes

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What is a static member variable, how does its lifetime and initialization differ from a regular (non-static) data member, and why can a static member function not access non-static members directly?
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

class IdGenerator
{
private:
    static inline int s_nextId{ 1 };
    int m_id{};

public:
    IdGenerator() : m_id{ s_nextId++ }
    {
    }

    int getId() const { return m_id; }

    static int getNextId() { return s_nextId; }
};

int main()
{
    IdGenerator a;
    IdGenerator b;
    std::cout << a.getId() << ' ' << b.getId() << ' ' << IdGenerator::getNextId() << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

class Logger
{
public:
    Logger() { std::cout << "construct\n"; }
    ~Logger() { std::cout << "destruct\n"; }

    Logger& tag(const std::string& s)
    {
        std::cout << "tag " << s << '\n';
        return *this;
    }
};

int main()
{
    {
        Logger log;
        log.tag("a").tag("b").tag("c");
        std::cout << "before end of block\n";
    }
    std::cout << "after end of block\n";
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This `Buffer` class allocates a resource in its constructor, and a colleague adds a destructor to clean it up, but running the program under a sanitizer reports a double free / heap-use-after-free when objects are copied around:
```cpp
class Buffer
{
private:
    int* m_data;
    int m_size;

public:
    Buffer(int size) : m_data{ new int[size]{} }, m_size{ size }
    {
    }

    ~Buffer()
    {
        delete[] m_data;
    }
};

void useBuffer(Buffer b)
{
    // ...
}
```
Explain what's happening and why the destructor alone isn't enough here (you don't need to fix it with copy constructors/operators if not covered yet — just explain the failure mechanism).
A:

## 5. Implementation
Q: Write a class template `Box<T>` with a private member of type `T`, a constructor taking a `T` value, a member function `getValue() const` returning the stored value, and a nested type `Box<T>::ValueType` alias (member type) equal to `T`. Add a static member variable that counts how many `Box` objects of that instantiation have been constructed (incremented in the constructor, decremented in the destructor), and a static member function to read the current count. Demonstrate with at least two different `T` types (e.g. `Box<int>` and `Box<std::string>`) in `main()`, printing the live count as objects go in and out of scope.
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
