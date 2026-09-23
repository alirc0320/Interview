#pragma once
/* Minimal C11 test harness — deliberately no macros hiding registration
 * magic (no constructor attributes, no function-pointer tables the reader
 * has to reverse-engineer). List tests explicitly in main() with RUN_TEST,
 * the same way you'd want to read someone else's test file live on a
 * screen-shared CodePair session. */

#include <stdio.h>

static int mt_checks = 0;
static int mt_failures = 0;

#define CHECK(expr)                                                                                \
    do                                                                                             \
    {                                                                                              \
        mt_checks++;                                                                               \
        if (!(expr))                                                                               \
        {                                                                                          \
            mt_failures++;                                                                         \
            fprintf(stderr, "  CHECK failed: %s at %s:%d\n", #expr, __FILE__, __LINE__);           \
        }                                                                                          \
    } while (0)

/* Compares as long long — fine for the integer/bool/pointer-sized values
 * these problems deal with. Use CHECK(memcmp(...) == 0) for buffers. */
#define CHECK_EQ(a, b)                                                                             \
    do                                                                                             \
    {                                                                                              \
        mt_checks++;                                                                               \
        long long va_ = (long long) (a);                                                           \
        long long vb_ = (long long) (b);                                                           \
        if (va_ != vb_)                                                                            \
        {                                                                                          \
            mt_failures++;                                                                         \
            fprintf(stderr, "  CHECK_EQ failed: %s != %s (%lld vs %lld) at %s:%d\n", #a, #b, va_,  \
                    vb_, __FILE__, __LINE__);                                                      \
        }                                                                                          \
    } while (0)

#define TEST(name) static void name(void)

#define RUN_TEST(name)                                                                             \
    do                                                                                             \
    {                                                                                              \
        int before = mt_failures;                                                                  \
        name();                                                                                    \
        fprintf(stderr, "-- %s: %s\n", #name, (mt_failures == before) ? "PASS" : "FAIL");          \
    } while (0)

#define TEST_SUMMARY() fprintf(stderr, "%d check(s), %d failure(s)\n", mt_checks, mt_failures)

#define TEST_EXIT_CODE() (mt_failures == 0 ? 0 : 1)
