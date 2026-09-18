---
chapter: Chapter 28 — Input and Output (I/O)
date_written: <YYYY-MM-DD>
---

# Chapter: Input and Output (I/O)

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: Explain the C++ stream state flags (`goodbit`, `eofbit`, `failbit`, `badbit`): what condition sets each one, how `std::cin >> someInt` behaves (including what's left in the buffer) when the user types non-numeric input, and what you must do to the stream before it can be used again after a failed extraction.
A:

## 2. Does this compile?
Q:
```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ofstream out{ "data.txt" };
    out << "hello" << ' ' << 42 << '\n';
    out.close();

    std::ifstream in{ "data.txt" };
    std::string word;
    int number;
    in >> word >> number;

    std::cout << word << " " << number << '\n';
    in >> in;
    return 0;
}
```
A: (yes/no, and why)

## 3. Predict the output
Q:
```cpp
#include <iostream>
#include <sstream>

int main()
{
    std::istringstream iss{ "12 abc 34" };
    int a{};
    int b{};

    iss >> a;
    std::cout << "a=" << a << " good=" << iss.good() << '\n';

    iss >> b;
    std::cout << "b=" << b << " fail=" << iss.fail() << '\n';

    iss.clear();
    std::string rest;
    iss >> rest;
    std::cout << "rest=" << rest << '\n';
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: A colleague writes this loop to sum integers typed by the user until end-of-input (Ctrl+D), but if the user accidentally types a letter, the program spins forever printing the same failure message. Explain the bug and what's missing:
```cpp
#include <iostream>

int main()
{
    int total{ 0 };
    int value{};

    while (!std::cin.eof())
    {
        std::cin >> value;
        if (std::cin.fail())
        {
            std::cout << "invalid input, try again\n";
        }
        else
        {
            total += value;
        }
    }

    std::cout << "total: " << total << '\n';
    return 0;
}
```
A:

## 5. Implementation
Q: Write a program that: (1) opens a file for writing and writes a small fixed-size array of `struct Record { int id; double score; }` to it using binary file I/O; (2) closes it and reopens the same file for reading; (3) uses random file access (`seekg`) to read back only the 3rd record directly (without reading the ones before it) and print its fields; (4) validates that the read succeeded by checking the stream state afterward and prints an error message if it didn't.
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
