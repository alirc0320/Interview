---
chapter: Chapter 3 — Debugging C++ Programs
date_written: <YYYY-MM-DD>
---

# Chapter: Debugging C++ Programs

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation
Q: What is the difference between a syntax error and a semantic error, and why can semantic errors be far more dangerous even though the program "runs fine"?
A:

## 2. Does this compile?
Q: You have a program that compiles cleanly and runs without crashing, but it prints the wrong average for a list of numbers because of an off-by-one error in a loop bound. Does this scenario represent a compiler-catchable problem? Explain why static analysis / the compiler generally can't find this class of bug for you.
A: (yes/no, and why)

## 3. Predict the output
Q: A programmer inserts a temporary `std::cerr << "reached here, x = " << x << '\n';` debug statement inside a function to narrow down where a calculation goes wrong, runs the program once, sees the output, and then forgets to remove the statement before their next build. What's the risk of leaving ad hoc debug prints like this in place long-term, and what's a better alternative discussed in this chapter?
A:

## 4. Debugging / undefined behavior
Q: You have a function that's supposed to return the largest of three integers, but it sometimes returns the wrong one depending on the input order. Describe, step by step, how you would use print-statement debugging (not a debugger) to isolate exactly which comparison is wrong.
A:

## 5. Implementation
Q: Take a small program with a deliberately planted logic bug (you write it), and describe the specific breakpoint(s) and variable(s) you would watch in an integrated debugger to catch the bug in the act, and what stepping strategy (step over/into/out) you'd use to get there efficiently.
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
