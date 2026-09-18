#include "minitest.h"
#include "solution.h"

// ---------------------------------------------------------------------
// Problem 23 is a code-review exercise (find and fix bugs in a snippet),
// so there is no fixed API to unit-test. Instead:
//   1. Paste the buggy snippet into solution.cpp and fix it there.
//   2. Expose whatever functions you want to exercise via solution.h.
//   3. Add TEST(...) cases below that call them.
// The Makefile already builds with -fsanitize=address,undefined, which is
// what will flag dangling references, use-after-free, double delete,
// signed overflow, uninitialized reads, etc. at runtime.
// ---------------------------------------------------------------------

TEST_MAIN()
