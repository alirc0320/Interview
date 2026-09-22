# Anduril interview prep

A separate prep track from the root Etched C++ prep. Anduril's Flight
Engineering technical screen is **live coding in plain C** (HackerRank
CodePair, ~40-45 min), with a heavy focus on pointers, pointer arithmetic,
bitwise operators, and endianness — the kind of thing you'd actually write
on embedded/flight hardware. See `RECRUITER_NOTES.local.md` (git-ignored,
local only) for the full recruiter prep email and evaluation criteria.

Everything under this folder is pure C11 — no C++, no STL, no references,
matching the actual interview language.

## Coding problems (`problems/`)

Same shape as the root repo's `problems/`: each is
`problems/<name>/{solution.h, solution.c, test.c}`. `solution.h` declares
the interface, `solution.c` ships as a TODO stub, `test.c` is a complete
assert-style test suite (run with `RUN_TEST(...)` calls listed explicitly
in `main()` — no hidden registration magic, so it reads the way you'd want
code to read on a shared CodePair screen).

```
make new  PROB=<name>   # scaffold a new problem from template/
make test PROB=<name>   # build & run problems/<name>/*.c
make list                # list existing problems
make clean               # remove build artifacts
```

Compiler flags (`Makefile`): `-std=c11 -Wall -Wextra -Wpedantic
-Wconversion -Wshadow -g -O0 -fsanitize=address,undefined`.

| Problem | Covers |
|---|---|
| `bitfield_extract_insert` | bit-field extract/insert on a 32-bit word |
| `endian_swap` | byte-swap + runtime endianness detection, no `<arpa/inet.h>` |
| `circular_buffer` | fixed-capacity static ring buffer, no `malloc` |
| `bit_register_ops` | set/clear/toggle/test-bit + masked read-modify-write |
| `uart_frame_parser` | length-prefixed byte-buffer framing + checksum, via pointer arithmetic |
| `safe_fixed_buffer_copy` | bounded string copy/append into fixed storage |

## Study routine (`topics/`)

Same routine as the root repo's LearnCpp track: close the material, write
questions from memory, reopen and correct.

```
make new-topic NAME=<slug>   # scaffold topics/<slug>.md from template_topic.md
```

| # | Topic file | GFG question # | Notes |
|---|---|---|---|
| 1 | `01_data_types_and_storage_classes` | 1, 2, 4, 8, 9 | |
| 2 | `02_pointers_and_arrays` | 3 (+ function pointers) | **10 questions — the big one** |
| 3 | `03_volatile_qualifier` | 5, 48, 77 | |
| 4 | `04_bitwise_operators_and_bitfields` | 6, 7 | pair with `bitfield_extract_insert`, `bit_register_ops` |
| 5 | `05_memory_management_constraints` | 16-20 | conceptual; pair with `circular_buffer`, `safe_fixed_buffer_copy` |
| 6 | `06_rtos_concurrency` | 26-30, 106 | conceptual — explain aloud, draw the diagrams |
| 7 | `07_communication_protocols` | 31-35 | conceptual; pair with `uart_frame_parser` |
| 8 | `08_endianness` | 58, 102 | pair with `endian_swap` |

GFG source: "Top Embedded C Interview Questions" (link in
`RECRUITER_NOTES.local.md`) — the recruiter flagged these exact ranges as
the ones to prioritize; each topic file paraphrases and builds original
questions/code around them rather than reproducing the article's text.

## Priority if short on time

Do topics **2 (pointers), 4 (bitwise), 8 (endianness)** and their paired
coding problems first — those three are where the live CodePair round is
most likely to actually put you in front of a blank editor. Topics 3, 5,
6, 7 are more likely to come up as spoken/design discussion than as code.

## Review

Shares the root repo's `../review/` (log + missed-questions rotation) —
no separate review folder here.
