---
topic: Memory management and memory constraints in embedded C
date_written: <YYYY-MM-DD>
---

# Topic: Memory Management and Memory Constraints

Maps to GFG Q16-20 (static vs dynamic allocation, stack overflow, memory
fragmentation, memory leaks). Mostly conceptual/discussion — code only
where it fits naturally (see `problems/circular_buffer` and
`problems/safe_fixed_buffer_copy`, which are the coding-shaped versions
of "prefer static allocation" and "don't overflow a fixed buffer").

## 1. Concept explanation — static vs. dynamic allocation
Q: Explain the tradeoff between static allocation (a fixed-size global/`static` array or struct, sized at compile time) and dynamic allocation (`malloc`/`free`) in an embedded context. Why do many embedded/safety-critical codebases (including flight software) forbid or heavily restrict `malloc` after initialization? Name at least three concrete reasons (think: determinism/worst-case timing, fragmentation over a long uptime, failure handling when memory is exhausted, and how much harder it is to statically verify memory usage/bounds when allocation is dynamic).
A:

## 2. Concept explanation — stack vs. heap, and overflow
Q: Explain where a function's local variables live (the stack) versus where a `malloc`'d block lives (the heap), and what physically happens when a stack overflows on a typical embedded target with no MMU/hardware guard page (as opposed to a desktop OS process, which usually segfaults cleanly). Give two concrete embedded-relevant causes of stack overflow: (1) something algorithmic (a specific coding pattern that grows the stack unboundedly), and (2) something about sizing (a specific configuration mistake). Name one practical mitigation for each.
A:

## 3. Concept explanation — fragmentation
Q: Explain what memory fragmentation is (distinguish external fragmentation from internal fragmentation), why it's a bigger practical problem for embedded systems that run for a very long time (weeks/months without reboot) than for a short-lived desktop process, and describe one allocator-design strategy that reduces it in constrained environments (e.g. fixed-size memory pools/block allocators for a small number of known object sizes, instead of a general-purpose heap).
A:

## 4. Debugging — memory leak
Q:
```c
typedef struct
{
    int id;
    char *label;
} Widget;

Widget *makeWidget(int id, const char *label)
{
    Widget *w = malloc(sizeof(Widget));
    w->id = id;
    w->label = strdup(label);
    return w;
}

void destroyWidget(Widget *w)
{
    free(w);
}
```
Identify the leak precisely (what's leaked, and on which code path), explain why `destroyWidget` alone isn't enough to prevent it even though it does call `free`, and rewrite `destroyWidget` correctly. Then note the second, separate bug in `makeWidget` itself (a missing check that matters a lot more in embedded than in typical desktop code, where memory is far more likely to actually be exhausted).
A:

## 5. Concept explanation — detecting these problems
Q: Name at least three concrete tools/techniques (beyond "just read the code carefully") for finding memory bugs (leaks, overflows, use-after-free, uninitialized reads) during development, and say which are realistic to run on-target (on the actual embedded hardware/RTOS) versus which really need a desktop/simulator build. Include at least one you've actually used yourself (even if only in this repo's own build line: `-fsanitize=address,undefined`).
A:

## 6. Implementation — a fixed-size memory pool
Q: Write a tiny fixed-block memory pool: `#define POOL_BLOCK_SIZE 32`, `#define POOL_BLOCK_COUNT 16`, a `MemPool` struct holding the backing storage (a `uint8_t[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE]` or equivalent) plus a free-list, `void poolInit(MemPool *pool)`, `void *poolAlloc(MemPool *pool)` (returns `NULL` if exhausted), and `void poolFree(MemPool *pool, void *block)`. No `malloc`/`free` anywhere inside the pool's own implementation. Explain in a comment why allocation/free here are both O(1) and why that predictability (not just avoiding fragmentation) is itself a reason to prefer this over general-purpose `malloc` in a hard-real-time context.
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
