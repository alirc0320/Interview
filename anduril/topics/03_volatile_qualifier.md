---
topic: The volatile qualifier
date_written: <YYYY-MM-DD>
---

# Topic: volatile

Maps to GFG Q5/Q48/Q77 (the guide lists "volatile" three separate times
across the full 110 — that repetition alone tells you how often it gets
asked). Close whatever material you used and write these from memory.

## 1. Concept explanation
Q: Explain precisely what `volatile` tells the compiler to stop assuming about a variable, in terms of the "as-if rule" / optimizer behavior: what is the compiler normally allowed to do with reads/writes to an ordinary variable (caching in a register, reordering, eliminating redundant reads/writes) that it is NOT allowed to do to a `volatile` one? Give the three canonical categories of variable that need `volatile` in embedded C: (1) a memory-mapped hardware register, (2) a global modified inside an ISR and read in `main()`, (3) a variable shared with another execution context outside the compiler's view (e.g. modified by DMA hardware, not by any C code at all).
A:

## 2. Does this compile / predict the output?
Q:
```c
#include <stdint.h>

int checkFlag(int *flag)
{
    while (*flag == 0)
    {
        /* spin, waiting for an ISR to set *flag */
    }
    return 1;
}
```
With optimizations off (`-O0`) this behaves as intended. Explain concretely what a compiler at `-O2` is entitled to do to this loop (in terms of what it's allowed to assume about `*flag` never changing from within this function), what the observable symptom would be at runtime, and the one-word fix.
A:

## 3. Concept explanation — volatile is not synchronization
Q: `volatile` prevents the compiler from over-optimizing accesses to a variable, but it does NOT make an operation atomic and does NOT provide any memory-ordering/synchronization guarantee between cores or against instruction reordering done by the CPU itself (as opposed to the compiler). Give a concrete example of a `volatile int counter` being incremented (`counter++`) from two different contexts (e.g. main loop and an ISR, or two RTOS tasks) where `volatile` alone still allows a lost update, and name the actual tool you'd reach for instead (disabling interrupts around the critical section, a hardware atomic instruction / `_Atomic` type, a mutex/semaphore) depending on the context.
A:

## 4. Debugging / undefined behavior — const volatile together
Q: A status register is read-only from software (hardware updates it, software must never write it) but it does change out from under the program at any time:
```c
uint32_t *STATUS_REG = (uint32_t *)0x40001000;
```
Rewrite this declaration correctly using both `const` and `volatile` together, and explain in your own words what each keyword is independently guaranteeing here — i.e. why you need both, and what would go wrong (in terms of either a compiler bug class or a real hardware/software bug) if you only used one of the two.
A:

## 5. Implementation
Q: Write a small header-only pattern for defining a set of memory-mapped registers at fixed addresses (pick 2-3 plausible ones, e.g. a control register and a status register a few bytes apart), using `#define`s that cast a literal address to the correctly-qualified pointer type, so that code elsewhere can write `CTRL_REG |= SOME_BIT;` or `while (!(STATUS_REG & READY_BIT)) {}` directly. Show both the `#define`-based approach and, as a comment, how you'd do the equivalent with a `volatile struct` overlaid on the peripheral's base address instead — and note briefly why the struct approach requires you to be careful about padding/alignment matching the hardware's actual register layout.
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
