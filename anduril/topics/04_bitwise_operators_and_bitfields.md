---
topic: Bitwise operators and bit-fields
date_written: <YYYY-MM-DD>
---

# Topic: Bitwise Operators and Bit-Fields

Maps to GFG Q6/Q7. This is the topic most likely to show up verbatim as a
live-coding problem (see `problems/bitfield_extract_insert` and
`problems/bit_register_ops`) — do those two problems as part of closing
this topic out, not just the Q&A below.

## 1. Concept explanation
Q: List the C bitwise operators (`&`, `|`, `^`, `~`, `<<`, `>>`) and, for each, give the one-line idiom it's used for in register/flag manipulation: setting a bit, clearing a bit, toggling a bit, testing a bit, and building/clearing a multi-bit mask. Then explain the difference between `>>` on a signed vs. unsigned operand (arithmetic vs. logical right shift) and why using `>>` on a signed negative value is implementation-defined behavior you should avoid in portable embedded code.
A:

## 2. Does this compile / predict the output?
Q:
```c
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint8_t flags = 0;
    flags |= (1 << 0);
    flags |= (1 << 2);
    printf("%d\n", flags);

    int8_t x = -8;
    printf("%d\n", x >> 1);

    uint32_t y = 1u << 31;
    printf("%u\n", y);

    int z = 1 << 31;
    printf("%d\n", z);

    return 0;
}
```
A: Predict the output line by line, and separately flag which line, if any, invokes undefined behavior (not just "surprising" behavior) under the C standard, and why.

## 3. Concept explanation — bit-fields in structs
Q: Explain what a bit-field member declaration like `unsigned status : 3;` inside a struct does, and then explain the two big reasons embedded engineers are told to be careful with (or often avoid) struct bit-fields for hardware register layouts specifically: (1) the C standard does NOT guarantee bit-field packing order (MSB-first vs LSB-first) or exact layout across compilers/platforms, and (2) how that compares to just using explicit shift/mask macros/functions on a plain `uint32_t`, which you fully control. When would you still reach for bit-fields anyway (hint: think about what you're NOT relying on cross-compiler portability for)?
A:

## 4. Debugging / undefined behavior
Q: This is meant to build a 16-bit config word from four 4-bit fields, but the caller gets back garbage for fields near the top:
```c
uint16_t buildConfig(uint8_t mode, uint8_t rate, uint8_t chan, uint8_t flags)
{
    uint16_t cfg = mode;
    cfg |= rate << 4;
    cfg |= chan << 8;
    cfg |= flags << 12;
    return cfg;
}
```
Called as `buildConfig(0x1, 0x2, 0x3, 0xF)`. Trace through what actually happens on a typical platform where `int` is 32 bits (hint: what type does each `uint8_t` operand get promoted to before the shift, and does that promotion by itself explain a bug here, or is the real issue something the caller could trigger with badly-ranged inputs like `mode = 0x1F` instead of a proper 4-bit value)? Identify the actual defect and fix it.
A:

## 5. Implementation — population count
Q: Write `int countSetBits(uint32_t value)` that returns how many bits are set, using ONLY bitwise operators and a loop (no `__builtin_popcount`, no lookup table for this version). Then write a second version, `int countSetBitsLookup(uint32_t value)`, using a 256-entry lookup table of bit counts for a single byte, applied four times — and explain in a comment the tradeoff this represents (a classic embedded optimization: trading ROM/flash space for cycles) and when you'd prefer the table version on a real MCU vs. when you wouldn't (e.g. a target with very limited flash).
A:

## 6. Implementation — bit reversal
Q: Write `uint8_t reverseBits8(uint8_t value)` that reverses the order of the 8 bits (bit 0 becomes bit 7, bit 1 becomes bit 6, etc.) using only shifts, masks, and a loop. State the time complexity in terms of number of bits, and as a comment, describe (you don't need to implement it) how a lookup-table or divide-and-conquer swap approach could do it faster/without a loop.
A:

## 7. Implementation — parity check
Q: Write `int hasEvenParity(uint32_t value)` that returns 1 if the number of set bits in `value` is even, 0 otherwise, without calling `countSetBits` from question 5 — instead use the XOR-folding trick (repeatedly XOR the value with a right-shifted copy of itself, halving the range each time) so it runs in `O(log n)` operations instead of `O(n)`. Explain in a comment why XOR is the right operator for this (what property of XOR on individual bit pairs makes the parity survive the folding).
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
