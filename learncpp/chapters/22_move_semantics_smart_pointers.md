---
chapter: Chapter 22 — Move Semantics and Smart Pointers
date_written: <YYYY-MM-DD>
---

# Chapter: Move Semantics and Smart Pointers

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: State the Rule of Five (or Rule of Zero) and explain precisely when the compiler will and will not implicitly generate a move constructor/move assignment operator for a class (e.g. what happens the moment you declare a user-provided destructor, copy constructor, or copy assignment operator). Then compare `std::unique_ptr` and `std::shared_ptr`: what ownership model does each express, what is the runtime/memory cost of each, and when would you choose one over the other in an interview-style design question?
A:

## 2. Does this compile?
Q:
```cpp
#include <memory>
#include <iostream>

class Widget
{
public:
    Widget() = default;
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
};

std::unique_ptr<Widget> makeWidget()
{
    std::unique_ptr<Widget> w{ std::make_unique<Widget>() };
    return w;
}

int main()
{
    std::unique_ptr<Widget> a{ makeWidget() };
    std::unique_ptr<Widget> b{ a };
    std::cout << "done\n";
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>
#include <utility>

class Resource
{
private:
    std::string m_name;

public:
    Resource(std::string name) : m_name{ std::move(name) }
    {
        std::cout << "construct " << m_name << '\n';
    }

    Resource(const Resource& other) : m_name{ other.m_name }
    {
        std::cout << "copy " << m_name << '\n';
    }

    Resource(Resource&& other) noexcept : m_name{ std::move(other.m_name) }
    {
        std::cout << "move " << m_name << '\n';
    }
};

Resource makeResource()
{
    return Resource{ "temp" };
}

int main()
{
    Resource a{ "a" };
    Resource b{ std::move(a) };
    Resource c{ makeResource() };
    Resource d{ a };
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: Two classes each hold a `std::shared_ptr` to the other so they can navigate back and forth:
```cpp
#include <memory>

class B;

class A
{
public:
    std::shared_ptr<B> m_b;
    ~A() { /* never printed */ }
};

class B
{
public:
    std::shared_ptr<A> m_a;
    ~B() { /* never printed */ }
};

void link()
{
    auto a{ std::make_shared<A>() };
    auto b{ std::make_shared<B>() };
    a->m_b = b;
    b->m_a = a;
}
```
After `link()` returns, neither destructor ever runs and the memory is never freed. Explain exactly why (in terms of reference counts) and how `std::weak_ptr` fixes it, and which member should become the `weak_ptr`.
A:

## 5. Implementation
Q: Write a `class UniqueBuffer` that owns a heap-allocated `int` array via `std::unique_ptr<int[]>` and stores its size. Give it: a constructor taking a size, a `set(int index, int value)` / `get(int index) const` pair (asserting bounds), and explicitly implement (or `= default`/`= delete` as appropriate) its move constructor, move assignment, copy constructor, and copy assignment — deciding deliberately whether the class should be copyable at all given it owns a `unique_ptr`. Demonstrate in `main()` that moving a `UniqueBuffer` leaves the source in a valid-but-empty state and transfers ownership to the destination.
A:

## 6. Concept explanation — custom deleters and array/aliasing forms
Q: `std::unique_ptr<T>` and `std::shared_ptr<T>` both support custom deleters (e.g. for a resource that must be freed with `fclose` or a hardware-specific release function instead of `delete`), but the mechanism differs: for `unique_ptr` the deleter is part of the type (`std::unique_ptr<T, Deleter>`), while for `shared_ptr` it's erased into the control block and isn't part of the type. Explain why that difference exists (what it implies about the size of a `unique_ptr` with a stateless vs. stateful deleter vs. the size of a `shared_ptr`), and describe what the "aliasing constructor" of `shared_ptr` (`shared_ptr<U>(shared_ptr<T> owner, U* ptr)`) is for — give a concrete example, like handing out a `shared_ptr` to a single member of a struct while keeping the whole struct alive.
A:

## 7. Predict the output — shared_ptr refcounts and weak_ptr locking
Q:
```cpp
#include <iostream>
#include <memory>

int main()
{
    auto a{ std::make_shared<int>(42) };
    std::cout << a.use_count() << '\n';

    std::weak_ptr<int> w{ a };
    {
        auto b{ a };
        std::cout << a.use_count() << '\n';
    }
    std::cout << a.use_count() << '\n';

    if (auto locked{ w.lock() })
        std::cout << "alive: " << *locked << '\n';

    a.reset();
    std::cout << w.expired() << '\n';
    if (auto locked{ w.lock() })
        std::cout << "alive: " << *locked << '\n';
    else
        std::cout << "expired\n";

    return 0;
}
```
A:

## 8. Debugging / undefined behavior — mixing raw and smart pointers to the same object
Q: A function takes ownership via `unique_ptr` but a caller keeps a raw pointer around "just to check something later":
```cpp
#include <memory>

class Sensor
{
public:
    int read() const { return 42; }
};

Sensor* g_lastSensor{ nullptr };

void registerSensor(std::unique_ptr<Sensor> s)
{
    g_lastSensor = s.get();
    // s goes out of scope here
}

int useSensorLater()
{
    return g_lastSensor->read();
}

int main()
{
    auto sensor{ std::make_unique<Sensor>() };
    registerSensor(std::move(sensor));
    return useSensorLater();
}
```
Explain exactly what's wrong with `g_lastSensor` by the time `useSensorLater()` runs, why `.get()` is a code smell whenever the raw pointer can outlive the smart pointer that produced it, and name at least one design fix appropriate here (changing who owns the `Sensor`, using `shared_ptr`/`weak_ptr`, or restructuring lifetime/ownership so `registerSensor` doesn't take ownership at all).
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
