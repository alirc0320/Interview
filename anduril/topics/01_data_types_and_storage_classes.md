---
topic: Data types, storage classes, const, and inline functions in Embedded C
date_written: <YYYY-MM-DD>
---

# Topic: Data Types, Storage Classes, const, and inline

Maps to Embedded C interview guide questions 1, 2, 4, 8, 9 (C vs Embedded
C, basic data types, static vs auto, const, inline functions). Close
whatever material you used and write these from memory. All code is C
(C11), not C++.

## 1. Concept explanation
Q: What actually distinguishes "Embedded C" from standard C — is it a different language, a compiler extension, a subset, a superset, or a set of conventions? Name at least three concrete practical differences you'd expect to see in an embedded codebase versus a typical hosted/desktop C codebase (think: available standard library, memory model, how you'd represent a hardware register, how errors are typically signaled).
A:

## 2. Concept explanation — fixed-width types
Q: Why does embedded C code almost always use `uint8_t`, `int16_t`, `uint32_t`, etc. from `<stdint.h>` instead of `char`, `short`, `int`? What specifically is NOT guaranteed about the width of `int` (or `long`) across different targets/compilers, and what could concretely go wrong if firmware written and tested on a 32-bit-`int` host is later ported to a target where `int` is 16 bits?
A:

## 3. Concept explanation — storage classes
Q: Explain, precisely, what each of these does to an object's storage duration and linkage, and give a one-line embedded-relevant reason you'd reach for each:
- `static` on a variable declared inside a function
- `static` on a variable or function declared at file scope
- `extern` on a variable declared in a header, defined in one `.c` file
- a plain local variable with automatic storage duration (the default)
A:

## 4. Does this compile / predict the output?
Q:
```c
#include <stdio.h>

int counter(void)
{
    static int calls = 0;
    calls++;
    return calls;
}

int main(void)
{
    printf("%d %d %d\n", counter(), counter(), counter());
    return 0;
}
```
A: (predict the output, and explain why a `static` local behaves this way across calls — where does it actually live, and how is that different from where a normal local variable lives)

## 5. Debugging — const and where it lives
Q: A teammate on a memory-constrained MCU target complains their `.data`/`.bss` usage is way higher than expected, and traces it to this:
```c
const char *messages[] = {
    "boot ok",
    "sensor fault",
    "low battery",
};

void logMessage(int code)
{
    printf("%s\n", messages[code]);
}
```
Explain what `const char *messages[]` actually declares (is the array const, are the pointers const, are the pointees const — be precise), why the string literals themselves should end up in flash/`.rodata` rather than RAM on a typical embedded toolchain, and what change (if any) to this declaration would make that placement more explicit/guaranteed rather than compiler-dependent.
A:

## 6. Implementation — inline functions vs macros
Q: Write an `inline` function `uint16_t clampU16(uint16_t value, uint16_t lo, uint16_t hi)` that clamps `value` into `[lo, hi]`, and put it in a header the way you would for real embedded code (so it can be included by multiple `.c` files without violating the one-definition rule). Then write the equivalent as a `#define CLAMP_U16(value, lo, hi)` function-like macro. Demonstrate in a `main()` (or comment, if you're not compiling this) one concrete bug the macro version has that the `inline` function version doesn't — specifically around an argument with a side effect, e.g. `CLAMP_U16(x++, 0, 10)`.
A:

---

## Corrections
Reopen the material. Fix anything wrong above (don't erase the original
wrong answer — strike it through or note what was wrong, so the mistake
stays visible on review).

-

## Missed on first pass?
- [ ] Yes — copy the missed question(s) into `review/missed_questions.md`
- [ ] No
