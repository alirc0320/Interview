---
chapter: Chapter 18 — Iterators and Algorithms
date_written: <YYYY-MM-DD>
---

# Chapter: Iterators and Algorithms

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What is an iterator, conceptually? How does the `begin()`/`end()` pair define a range, and why is `end()` a "one past the last element" position rather than pointing at the last element itself?
A:

## 2. Does this compile?
Q:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    auto it{ std::find(v.begin(), v.end(), 3) };
    if (it == v.end())
        std::cout << "not found\n";
    else
        std::cout << *it << '\n';
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v{ 5, 3, 4, 1, 2 };
    std::sort(v.begin(), v.end());
    for (auto e : v)
        std::cout << e << ' ';
    std::cout << '\n';

    auto it{ std::find(v.begin(), v.end(), 4) };
    std::cout << (it - v.begin()) << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A teammate writes this to remove all even numbers from a vector, but it crashes or silently leaves an even number behind:
```cpp
std::vector<int> v{ 1, 2, 3, 4, 5, 6 };
for (auto it{ v.begin() }; it != v.end(); ++it)
{
    if (*it % 2 == 0)
        v.erase(it);
}
```
Explain exactly what is wrong.
A:

## 5. Implementation
Q: Implement selection sort by hand on a `std::vector<int>` (following the algorithm's own logic — no calling `std::sort`), following the approach described in this chapter. Then write a second version of the same task using `std::sort` with iterators. Run both on a few test vectors and confirm (with `assert` or by inspection) that they produce identical sorted output.
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
