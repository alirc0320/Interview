# Interview prep environment

C++ practice environment for the Etched Accelerator Software interview
(2026-10-15). See `QUESTIONS.md` for what every folder/file maps to in the
guide.

Separate track: `anduril/` is a self-contained, **pure C** prep folder
for the Anduril Flight Engineering interview (live coding in C via
HackerRank CodePair, pointers/bitwise/endianness-heavy) — see
`anduril/README.md`. It has its own Makefile; nothing below applies to it.

## Coding problems

Each problem is `problems/<name>/{solution.h, solution.cpp, test.cpp}`.

```
make new    PROB=<name>   # scaffold a new problem from template/
make test   PROB=<name>   # build & run problems/<name>/*.cpp
make list                 # list existing problems
make format               # format all .cpp/.h/.hpp files with clang-format
make clean                # remove build artifacts
```

`solution.h` declares the interface; write your implementation in
`solution.cpp` and your cases in `test.cpp` using `common/minitest.h`:

```cpp
TEST(name) {
    CHECK(expr);           // fails loudly with file:line, keeps running
    CHECK_EQ(a, b);        // like CHECK but prints both values on mismatch
}
TEST_MAIN()                 // expands to main(); runs everything registered above
```

Compiler flags (`Makefile`) match the guide's recommended line:
`-std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g -O0
-fsanitize=address,undefined`.

## LearnCpp study routine

```
make new-chapter  NAME=<slug>     # scaffold learncpp/chapters/<slug>.md
make new-practice NAME=<slug>     # scaffold learncpp/practice/<slug>.cpp
make practice     NAME=<slug>     # build & run learncpp/practice/<slug>.cpp
```

Workflow: close the chapter, write 5 questions from memory into the chapter
log, reopen and correct. Every 2 chapters, write one combined 20-40 line
`practice.cpp` with 5+ `assert()`s.

## OS study routine

```
make new-section NAME=<slug>      # scaffold os/sections/<slug>.md
```

Workflow: close the material, explain the mechanism aloud, draw the
diagram, answer 5 questions, reopen and correct. Code only when it fits
naturally — see `os_page_replacement`, `os_cpu_scheduler`,
`os_file_cache` in `problems/`, or reuse `21_bounded_producer_consumer` /
`09_page_allocator` / `20_arena_allocator`.

## Review cadence

Tracked in `review/`:
- `review/log.md` — dated next-day / weekend / following-week sessions.
- `review/missed_questions.md` — rotation of anything missed, cleared only
  once you can explain it unprompted.
