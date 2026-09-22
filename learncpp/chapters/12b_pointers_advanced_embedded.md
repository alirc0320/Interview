---
chapter: 12b — Pointers Deep Dive (custom addition, embedded/interview focus)
date_written: <YYYY-MM-DD>
---

# Chapter: Pointers Deep Dive (not a real LearnCpp chapter)

This file is not part of the official LearnCpp sequence — it's a
supplement inserted right after chapter 12 because raw-pointer questions in
an embedded/accelerator interview routinely go past what LearnCpp's
References and Pointers chapter covers on its own. The topics below are
each covered by LearnCpp elsewhere (arrays/decay in chapter 17, `new`/
`delete` in chapter 19, function pointers in chapter 20) or not at all
(`void*`, `volatile`, memory-mapped I/O, strict aliasing, alignment) — this
file pulls them together specifically as pointer questions, because that's
how they tend to get asked in interviews: "here's a pointer, what happens."

Close whatever material you used (LearnCpp pages, a reference, or your own
notes) and write these from memory. Do not look anything up.

## 1. Concept explanation — pointer arithmetic mechanics
Q: Given `int arr[5]{}; int* p{ arr };`, explain exactly what `p + 1` computes (in terms of bytes, not just "the next element"), why `p[i]` is defined as `*(p + i)`, and why pointer arithmetic on a `char*`/`std::byte*` behaves differently in practice from arithmetic on an `int*` even though the language rule is the same. Then explain why `arr + 5` is legal to compute but `*(arr + 5)` is undefined behavior, and why that one-past-the-end pointer matters for loop idioms like `for (int* p{arr}; p != arr+5; ++p)`.
A:

## 2. Does this compile?
Q:
```cpp
#include <cstdint>
#include <iostream>

int main()
{
    int arr[4]{ 10, 20, 30, 40 };
    int* p{ arr };

    std::cout << *(p + 2) << '\n';
    std::cout << p[2] << '\n';
    std::cout << 2[p] << '\n';

    std::ptrdiff_t diff{ (arr + 3) - (arr + 1) };
    std::cout << diff << '\n';

    int* q{ arr + 1 };
    int* r{ arr + 3 };
    std::cout << (r - q) << '\n';
    std::cout << (r + q) << '\n';

    return 0;
}
```
A: (yes/no for each statement, and why — one of these lines does not compile)

## 3. Predict the output — void* and reinterpret_cast (type punning)
Q:
```cpp
#include <cstdint>
#include <cstring>
#include <iostream>

int main()
{
    float f{ 1.0f };
    void* vp{ &f };

    // "safe" type punning via memcpy
    std::uint32_t bitsViaMemcpy{};
    std::memcpy(&bitsViaMemcpy, &f, sizeof(f));

    // reinterpret_cast-based punning (undefined behavior under strict aliasing,
    // but describe what a typical x86/ARM compiler will print anyway)
    std::uint32_t* bitsViaCast{ reinterpret_cast<std::uint32_t*>(&f) };

    std::cout << std::hex << bitsViaMemcpy << '\n';
    std::cout << std::hex << *bitsViaCast << '\n';

    return 0;
}
```
A: Predict the printed hex value (you don't need the exact bit pattern from memory — explain what it represents), and explain why the `memcpy` version is the one you should actually write in real code even though the `reinterpret_cast` version "works" on most compilers today.

## 4. Debugging / undefined behavior — volatile and memory-mapped I/O
Q: This is meant to poll a hardware status register until a "ready" bit is set, then read a data register once:
```cpp
#include <cstdint>

constexpr std::uint32_t* STATUS_REG{ reinterpret_cast<std::uint32_t*>(0x4000'0000) };
constexpr std::uint32_t* DATA_REG{   reinterpret_cast<std::uint32_t*>(0x4000'0004) };
constexpr std::uint32_t READY_BIT{ 1u << 0 };

std::uint32_t readSensor()
{
    while ((*STATUS_REG & READY_BIT) == 0)
    {
        // spin
    }
    return *DATA_REG;
}
```
In a release build with optimizations on, this hangs forever even when the hardware does become ready, or it reads stale data. Explain, in terms of what the optimizer is allowed to assume about ordinary (non-`volatile`) reads through a pointer, exactly what's wrong here, fix the declarations of `STATUS_REG`/`DATA_REG`, and explain why `volatile` solves this but does **not** make the code thread-safe or provide any memory-ordering/synchronization guarantee (i.e. why you'd still reach for `std::atomic` or a memory barrier for cross-thread communication, and reserve `volatile` for memory-mapped hardware registers and signal handlers).
A:

## 5. Predict the output — alignment and misaligned access
Q:
```cpp
#include <cstdint>
#include <iostream>

struct Packed
{
    std::uint8_t  tag;
    std::uint32_t value;
};

int main()
{
    std::cout << sizeof(Packed) << '\n';
    std::cout << alignof(Packed) << '\n';

    char buffer[16]{};
    // deliberately construct a misaligned std::uint32_t* into raw storage
    std::uint32_t* misaligned{ reinterpret_cast<std::uint32_t*>(buffer + 1) };
    *misaligned = 0xDEADBEEF;

    return 0;
}
```
A: Predict `sizeof(Packed)` and `alignof(Packed)` (explain the padding the compiler inserts and why), then explain why the `misaligned` write is undefined behavior even though it "usually works" on x86 — and why the exact same code can hard-fault (bus error) on some 32-bit ARM/embedded targets. What's the safe way to read/write a multi-byte value at an arbitrary, possibly-unaligned byte offset in a buffer (e.g. when parsing a wire protocol)?

## 6. Concept explanation — pointers vs. references vs. smart pointers, as a design decision
Q: You're writing a function signature and need to decide between `T*`, `T&`, `const T&`, `std::unique_ptr<T>&`, `std::unique_ptr<T>`, and `std::shared_ptr<T>` as the parameter type. Give the decision rule you'd use for each of: (a) "I need to observe/modify an object I don't own and it's optional whether one exists," (b) "I need to observe an object I don't own and it must exist," (c) "I'm taking ownership of a heap object," (d) "I just need to use an object someone else owns, without affecting its lifetime, and it must exist." For each, say which of the six types you'd pick and why the others are wrong for that case — this is close to a direct interview question in embedded/systems roles because it tests whether you reach for `shared_ptr` out of habit instead of thinking about ownership.
A:

## 7. Debugging / undefined behavior — array-to-pointer decay and sizeof
Q:
```cpp
#include <cstddef>
#include <iostream>

std::size_t countElements(int arr[])
{
    return sizeof(arr) / sizeof(arr[0]);
}

int main()
{
    int data[10]{};
    std::cout << (sizeof(data) / sizeof(data[0])) << '\n';
    std::cout << countElements(data) << '\n';
    return 0;
}
```
The two printed values differ, and a teammate is confused why the "same expression" gives a different answer inside vs. outside `countElements`. Explain exactly what `arr` is inside `countElements` (what decays, and when), what `sizeof(arr)` actually evaluates to in that function, and what general rule this implies about ever trying to compute an array's length from a bare pointer parameter (and what you should pass instead — a template, a `std::span`, or an explicit size parameter).
A:

## 8. Implementation — pointer arithmetic on a byte buffer (protocol parsing)
Q: Write a function
```cpp
std::size_t parseHeader(const std::uint8_t* buf, std::size_t len, std::uint16_t& outId, std::uint32_t& outPayloadLen);
```
that parses a little-endian wire format from the front of `buf`: a 2-byte ID, then a 4-byte payload length (6 bytes total), writing them into `outId`/`outPayloadLen` via pointer arithmetic over `buf` (not by `reinterpret_cast`ing `buf` to a struct pointer — explain in a comment why that would be unsafe here, tying back to Q5's alignment/strict-aliasing issues). Return the number of bytes consumed (6) on success, or 0 if `len < 6`. In `main()`, build a small `std::uint8_t` byte array by hand representing one header, call `parseHeader`, and assert the decoded `outId`/`outPayloadLen` match what you encoded.
A:

## 9. Implementation — a tiny function-pointer dispatch table
Q: Embedded firmware often dispatches incoming command bytes to handlers via a table instead of a big `switch`. Write a fixed-size dispatch table `void (*handlers[4])(std::uint8_t arg)` (or an equivalent `std::array` of function pointers), populate it with four small free functions (or captureless lambdas), and write `bool dispatch(std::uint8_t opcode, std::uint8_t arg)` that looks up `handlers[opcode]` and calls it if `opcode` is in range, returning `false` (without calling anything) if it's out of range. Explain in a comment why a captureless lambda can go in this table but a lambda that captures state cannot, and what you'd use instead if a handler needed to carry state (e.g. `std::function`, and the cost of that vs. a raw function pointer table).
A:

---

## Corrections
Reopen whatever material you used. Fix anything wrong above (don't erase
the original wrong answer — strike it through or note what was wrong, so
the mistake stays visible on review).

-

## Missed on first pass?
- [ ] Yes — copy the missed question(s) into `review/missed_questions.md`
- [ ] No
