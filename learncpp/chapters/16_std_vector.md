---
chapter: Chapter 16 — Dynamic arrays: std::vector
date_written: <YYYY-MM-DD>
---

# Chapter: Dynamic arrays: std::vector

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Explain the difference between a std::vector's `size()` and its `capacity()`. What happens internally when `push_back()` causes the vector to exceed its current capacity, and why can that operation invalidate existing iterators, pointers, and references into the vector?
A:

## 2. Does this compile?
Q:
```cpp
#include <vector>

void addOne(const std::vector<int>& v)
{
    v.push_back(1);
}

int main()
{
    std::vector<int> nums{ 1, 2, 3 };
    addOne(nums);
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v{ 1, 2, 3 };
    for (auto& e : v)
        e *= 2;
    for (auto e : v)
        std::cout << e << ' ';
    std::cout << '\n';

    v.push_back(100);
    std::cout << v.size() << ' ' << v.front() << ' ' << v.back() << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A teammate writes this and is confused why `first` sometimes prints garbage instead of `1`:
```cpp
std::vector<int> v{ 1, 2, 3 };
int& first{ v[0] };
v.push_back(4);
v.push_back(5);
std::cout << first << '\n';
```
Explain exactly what is wrong and why it might work fine in a quick test but fail later or on a different machine.
A:

## 5. Implementation
Q: Write a function `std::vector<int> buildSquares(int n)` that builds and returns a vector containing the squares of `0` through `n-1` (built up with `push_back`), and a `main()` that calls it for a couple of values of `n` and prints the results. Be ready to explain why returning the vector by value here is efficient rather than wasteful.
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
