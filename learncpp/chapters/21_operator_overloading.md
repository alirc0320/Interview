---
chapter: Chapter 21 — Operator Overloading
date_written: <YYYY-MM-DD>
---

# Chapter: Operator Overloading

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: For a binary operator like `operator+`, explain the three ways it can be implemented (member function, non-member/normal function, friend function), and give a concrete rule for when you're forced to choose one over another (e.g. why `operator<<` for a class can't be a member function, and why a symmetric operator that needs to convert its left-hand operand benefits from being non-member).
A:

## 2. Does this compile?
Q:
```cpp
#include <iostream>

class Point
{
private:
    int m_x{};
    int m_y{};

public:
    Point(int x, int y) : m_x{ x }, m_y{ y } {}

    Point operator+(const Point& other) const
    {
        return Point{ m_x + other.m_x, m_y + other.m_y };
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& p);
};

std::ostream& operator<<(std::ostream& out, const Point& p)
{
    out << '(' << p.m_x << ", " << p.m_y << ')';
    return out;
}

int main()
{
    Point a{ 1, 2 };
    Point b{ 3, 4 };
    std::cout << (a + b) << '\n';
    std::cout << (a << b) << '\n';
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
    Counter(int value = 0) : m_value{ value } {}

    Counter& operator++()
    {
        ++m_value;
        return *this;
    }

    Counter operator++(int)
    {
        Counter temp{ *this };
        ++(*this);
        return temp;
    }

    int value() const { return m_value; }
};

int main()
{
    Counter c{ 5 };
    std::cout << (++c).value() << '\n';
    std::cout << (c++).value() << '\n';
    std::cout << c.value() << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This overloaded `operator=` for a class that owns a dynamically allocated array works in most tests but crashes intermittently:
```cpp
class Buffer
{
private:
    int* m_data;
    int m_size;

public:
    Buffer(int size) : m_data{ new int[size]{} }, m_size{ size } {}
    ~Buffer() { delete[] m_data; }

    Buffer& operator=(const Buffer& other)
    {
        delete[] m_data;
        m_size = other.m_size;
        m_data = new int[m_size];
        for (int i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
        return *this;
    }
};
```
Explain the bug (hint: think about what happens with `b = b;`) and how to fix it.
A:

## 5. Implementation
Q: Write a `class Vector2D` with `double x, y` members. Implement: a constructor, `operator+`, `operator-`, and unary `operator-` (negation), `operator==` and `operator!=`, `operator<<` for printing as `(x, y)`, and `operator[]` that treats the vector as a 2-element array (index 0 = x, index 1 = y), asserting on out-of-range access. Demonstrate each operator being used in `main()`.
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
