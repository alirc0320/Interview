---
chapter: Chapter 26 — Templates and Classes
date_written: <YYYY-MM-DD>
---

# Chapter: Templates and Classes

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What is a template non-type parameter, and how does using one (e.g. `template <typename T, int N> class Array`) differ from storing the same value as a regular runtime data member? Also explain what a full (explicit) class template specialization is and when you'd reach for one instead of just writing a different class.
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

template <typename T, int Size>
class FixedArray
{
private:
    T m_data[Size]{};

public:
    T& operator[](int index) { return m_data[index]; }
    const T& operator[](int index) const { return m_data[index]; }
    int size() const { return Size; }
};

int main()
{
    FixedArray<int, 5> arr;
    for (int i = 0; i < arr.size(); ++i)
        arr[i] = i * 2;

    FixedArray<int> arr2;
    std::cout << arr[2] << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>

template <typename T>
class Wrapper
{
public:
    void identify() const { std::cout << "generic wrapper\n"; }
};

template <>
class Wrapper<double>
{
public:
    void identify() const { std::cout << "double wrapper\n"; }
};

template <typename T>
void report(const Wrapper<T>& w)
{
    w.identify();
}

int main()
{
    Wrapper<int> wi{};
    Wrapper<double> wd{};
    Wrapper<char> wc{};

    report(wi);
    report(wd);
    report(wc);
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A colleague writes a generic `Pair<T>` class template and then adds this "optimization" for pointer types using partial specialization, but it silently does the wrong thing for some callers. Find the bug:
```cpp
template <typename T>
class Pair
{
private:
    T m_first;
    T m_second;

public:
    Pair(T first, T second) : m_first{ first }, m_second{ second } {}
    T getFirst() const { return m_first; }
};

template <typename T>
class Pair<T*>
{
private:
    T* m_first;
    T* m_second;

public:
    Pair(T* first, T* second) : m_first{ first }, m_second{ second } {}
    T* getFirst() const { return m_first; }
    ~Pair() { delete m_first; delete m_second; }
};

int main()
{
    int x{ 5 }, y{ 10 };
    Pair<int*> p{ &x, &y };
    std::cout << *p.getFirst() << '\n';
    return 0;
}
```
A:

## 5. Implementation
Q: Write a class template `Stack<T, int Capacity>` (Capacity as a non-type template parameter) backed by a fixed-size C-style array. Implement `push(const T&)`, `pop()` (removes and returns the top element), `top() const`, `isEmpty() const`, `isFull() const`, and `size() const`. Add a full specialization `Stack<bool, Capacity>` (or a class template specialization for `T = char`) that behaves the same but prints a distinguishing debug message from one of its member functions, to prove specialization selection works. Demonstrate both the generic and specialized versions in `main()`.
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
