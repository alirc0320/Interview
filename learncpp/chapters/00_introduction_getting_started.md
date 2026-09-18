---
chapter: Chapter 0 — Introduction / Getting Started
date_written: <YYYY-MM-DD>
---

# Chapter: Introduction / Getting Started

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What are the distinct jobs performed by the compiler and the linker, and which one would catch a call to a function that has been declared but never defined anywhere in the project?
A:

## 2. Does this compile?
Q: A project has two source files. `main.cpp` calls a function `int square(int x);` (declared but not defined in `main.cpp`), and `math.cpp` defines `int square(int x) { return x * x; }`, but `math.cpp` is never added to the build/compile command — only `main.cpp` is compiled and linked. Does this build succeed?
A: (yes/no, and why)

## 3. Predict the output
Q: You compile a single-file program with warnings enabled but NOT treated as errors (`-Wall -Wextra`, no `-Werror`), and the program has an unused variable in `main()`. What happens when you compile and then run it — does compilation fail, and what does running it print?
A:

## 4. Debugging / undefined behavior
Q: A beginner sets their compiler's language standard to C++17 instead of C++20, then tries to use a C++20-only feature and gets a wall of confusing template/syntax errors that don't mention "C++20" anywhere. What's actually going wrong, and which build setting should they check first?
A:

## 5. Implementation
Q: Describe the exact sequence of steps you'd take, using your own IDE/compiler, to create a brand-new "Hello, world" C++ project, set the language standard to C++20, enable a solid warning level, and confirm (by deliberately breaking something) that warnings are visible in the build output.
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
