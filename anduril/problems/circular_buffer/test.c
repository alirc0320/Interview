#include "minitest.h"
#include "solution.h"

TEST(starts_empty)
{
    RingBuffer rb;
    ring_init(&rb);
    CHECK(ring_is_empty(&rb));
    CHECK(!ring_is_full(&rb));
}

TEST(pop_on_empty_fails_and_leaves_out_untouched)
{
    RingBuffer rb;
    ring_init(&rb);
    int out = 777;
    CHECK(!ring_pop(&rb, &out));
    CHECK_EQ(out, 777);
}

TEST(push_then_pop_is_fifo)
{
    RingBuffer rb;
    ring_init(&rb);
    CHECK(ring_push(&rb, 10));
    CHECK(ring_push(&rb, 20));
    CHECK(ring_push(&rb, 30));

    int out = 0;
    CHECK(ring_pop(&rb, &out));
    CHECK_EQ(out, 10);
    CHECK(ring_pop(&rb, &out));
    CHECK_EQ(out, 20);
    CHECK(ring_pop(&rb, &out));
    CHECK_EQ(out, 30);
    CHECK(ring_is_empty(&rb));
}

TEST(fills_to_capacity_and_rejects_overflow)
{
    RingBuffer rb;
    ring_init(&rb);
    for (int i = 0; i < RING_CAPACITY; ++i)
        CHECK(ring_push(&rb, i));

    CHECK(ring_is_full(&rb));
    CHECK(!ring_push(&rb, 999)); /* buffer is full, must be rejected */
}

TEST(wraps_around_the_backing_array)
{
    RingBuffer rb;
    ring_init(&rb);

    /* Fill it, drain half, refill — forces head/tail past the end of the
     * backing array and back to index 0, which is the actual point of a
     * *circular* buffer over a plain array. */
    for (int i = 0; i < RING_CAPACITY; ++i)
        CHECK(ring_push(&rb, i));

    int out = 0;
    for (int i = 0; i < RING_CAPACITY / 2; ++i)
    {
        CHECK(ring_pop(&rb, &out));
        CHECK_EQ(out, i);
    }

    for (int i = 0; i < RING_CAPACITY / 2; ++i)
        CHECK(ring_push(&rb, 100 + i));

    CHECK(ring_is_full(&rb));

    for (int i = RING_CAPACITY / 2; i < RING_CAPACITY; ++i)
    {
        CHECK(ring_pop(&rb, &out));
        CHECK_EQ(out, i);
    }
    for (int i = 0; i < RING_CAPACITY / 2; ++i)
    {
        CHECK(ring_pop(&rb, &out));
        CHECK_EQ(out, 100 + i);
    }
    CHECK(ring_is_empty(&rb));
}

int main(void)
{
    RUN_TEST(starts_empty);
    RUN_TEST(pop_on_empty_fails_and_leaves_out_untouched);
    RUN_TEST(push_then_pop_is_fifo);
    RUN_TEST(fills_to_capacity_and_rejects_overflow);
    RUN_TEST(wraps_around_the_backing_array);
    TEST_SUMMARY();
    return TEST_EXIT_CODE();
}
