---
topic: Pointers, arrays, and pointer arithmetic in C
date_written: <YYYY-MM-DD>
---

# Topic: Pointers, Arrays, and Pointer Arithmetic (the big one)

This topic gets 10 questions instead of 5 — pointers are the single most
likely deep-dive area in an embedded C live-coding interview (per the
recruiter's own prep notes: "Be familiar with... pointers, pointer
arithmetic... that you'd see in embedded work"). Maps to GFG Q3, and pulls
in function pointers (Q64/109) since callback tables and ISR-style
dispatch come up constantly in embedded C even though this specific guide
didn't number them in the assigned range. Close whatever material you
used and write these from memory. All code is C (C11) — this is the
"port the C++ pointers material to C, and go further" pass; if you did
the LearnCpp pointers chapters, notice which C++ safety nets (references,
`std::optional`, RAII/smart pointers) simply aren't there in C and what
that means for how carefully you have to write this code by hand.

## 1. Concept explanation
Q: Explain what a pointer variable actually stores, why `sizeof(int*)` and `sizeof(double*)` are the same on a given machine even though `sizeof(int)` and `sizeof(double)` differ, and walk through the "read right-to-left from the variable name" trick for parsing a declaration like `int *arr[10]` versus `int (*arr)[10]` — say in words what each one actually is.
A:

## 2. Does this compile?
Q:
```c
#include <stdlib.h>

int main(void)
{
    int *p = malloc(10 * sizeof(int));   /* line A */
    void *vp = p;                         /* line B */
    int *p2 = vp;                         /* line C */
    const int *cp = p;                    /* line D */
    int *p3 = cp;                         /* line E */
    free(p);
    return 0;
}
```
A: (yes/no for each line, and why — none of these is a hard compile error in C, but one produces a warning; identify which one and why, and separately note which lines would become hard *errors* if this exact file were compiled as C++ instead of C)

## 3. Predict the output — pointer arithmetic and array decay
Q:
```c
#include <stdio.h>

void printSizes(int arr[10])
{
    printf("%zu\n", sizeof(arr));
}

int main(void)
{
    int arr[10] = {0};
    printf("%zu\n", sizeof(arr));
    printSizes(arr);

    int *p = arr;
    printf("%ld\n", (long)((p + 4) - p));
    printf("%ld\n", (long)((arr + 10) - arr));
    return 0;
}
```
A:

## 4. Debugging / undefined behavior
Q: This helper is meant to return a pointer to a small on-stack buffer holding a formatted status string, for the caller to print:
```c
#include <stdio.h>

char *statusString(int code)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "status=%d", code);
    return buf;
}

int main(void)
{
    char *s = statusString(200);
    printf("%s\n", s);
    return 0;
}
```
Explain exactly what's wrong, why it might "seem to work" in a debug build or when nothing else runs between the call and the print, and why that unreliability is worse in practice than a guaranteed crash. Give two different fixes with different tradeoffs (one that changes the function's contract to use a caller-supplied buffer, one that uses static/heap storage) and say which you'd actually pick for embedded firmware and why.
A:

## 5. Concept explanation — const and pointers in C
Q: Write the four declarations of a pointer to an `int x` covering every combination of `const` placement (pointer to non-const, pointer to const, const pointer to non-const, const pointer to const). For each, state whether you can reassign the pointer and whether you can modify `*x` through it. Then explain why, in C specifically (unlike C++), you can implicitly assign a `void*` to any object pointer type but NOT implicitly assign a `const int*` to an `int*` — what's the compiler protecting against in each case?
A:

## 6. Predict the output — pointer to array vs array of pointers
Q:
```c
#include <stdio.h>

int main(void)
{
    int matrix[3][4];
    int (*rowPtr)[4] = matrix;      /* pointer to array of 4 ints */

    int a = 0;
    int b = 10;
    int c = 20;
    int *ptrArr[3] = { &a, &b, &c }; /* array of 3 pointers */

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            rowPtr[i][j] = i * 10 + j;

    printf("%d %d\n", matrix[2][1], rowPtr[2][1]);
    printf("%d %d %d\n", *ptrArr[0], *ptrArr[1], *ptrArr[2]);

    rowPtr++;
    printf("%d\n", (*rowPtr)[0]);

    return 0;
}
```
A: Predict the output, and explain in your own words the difference between `int (*rowPtr)[4]` and `int *ptrArr[3]` — what does `rowPtr++` actually move past (how many bytes), versus what would `ptrArr++` move past?

## 7. Debugging / undefined behavior — void* arithmetic
Q:
```c
void *advance(void *base, int byteOffset)
{
    return base + byteOffset;
}
```
Strict ISO C does not define arithmetic on `void*` (unlike GCC/Clang, which allow it as an extension treating it like `char*`). Rewrite `advance` to be portable standard C, explain why doing arithmetic through a `char*`/`unsigned char*` intermediate is the idiomatic fix, and explain why this matters specifically for embedded work (portability across compilers/targets, e.g. moving from GCC to a vendor's more strictly-conforming compiler).
A:

## 8. Implementation — function pointers as a dispatch table
Q: Write a fixed-size array of function pointers `void (*table[4])(int)` (or `typedef` a named function-pointer type first — your choice, but be able to explain the typedef'd version's syntax), populate it with four small functions with signature `void handler(int arg)`, and write `int dispatch(int opcode, int arg)` that calls `table[opcode](arg)` and returns 0, or returns -1 without calling anything if `opcode` is out of `[0, 4)`. This is close to a direct interview question for the flight-engineering team: explain in a comment why this pattern (vs. a big `switch`) is common in embedded/RTOS command handling, and what you'd need to change if a handler needed to also report success/failure back to the dispatcher.
A:

## 9. Implementation — pointer-to-pointer (resizing via an out-parameter)
Q: Write `int growBuffer(int **bufPtr, size_t *capacity, size_t newCapacity)` that reallocates `*bufPtr` to hold `newCapacity` ints (via `realloc`), updates `*capacity` on success, and returns 0 on success or -1 on failure (leaving `*bufPtr`/`*capacity` unchanged on failure — do not leak the original buffer if `realloc` fails). Explain in a comment exactly why the parameter has to be `int **` rather than `int *` for the caller's pointer to actually be updated, tying it back to how C passes everything by value.
A:

## 10. Implementation — in-place string reversal with two pointers
Q: Write `void reverseInPlace(char *s)` that reverses a NUL-terminated string in place using two pointers walking toward each other from opposite ends (no index variables, no second buffer). Handle the empty string and single-character string correctly without any special-cased branch (the loop condition alone should make those cases do nothing). This is a very common embedded/systems live-coding warm-up — be able to write it clean on a shared screen without hesitating over the loop condition or the swap.
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
