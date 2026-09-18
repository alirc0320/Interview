#include "minitest.h"
#include "solution.h"

#include <cstddef>
#include <memory>
#include <string>

// ---------------------------------------------------------------------
// Assumed contract (adjust if you design it differently on purpose):
//   - capacity N means the buffer holds up to N elements (full when
//     size() == N), i.e. you may not "waste" a slot.
//   - push() on a full buffer returns false and leaves the contents
//     untouched (no overwrite of the oldest element).
//   - pop() on an empty buffer returns std::nullopt.
//   - FIFO order, including across index wraparound.
//   - front() is only called on non-empty buffers (behaviour on an empty
//     buffer is unspecified, so it isn't tested).
// ---------------------------------------------------------------------

TEST(new_buffer_is_empty)
{
    RingBuffer<int> rb(4);
    CHECK(rb.empty());
    CHECK(!rb.full());
    CHECK_EQ(rb.size(), 0u);
}

TEST(pop_on_empty_returns_nullopt)
{
    RingBuffer<int> rb(4);
    CHECK(!rb.pop().has_value());
    CHECK(rb.empty());
    CHECK_EQ(rb.size(), 0u);
}

TEST(push_then_pop_single_element)
{
    RingBuffer<int> rb(4);
    CHECK(rb.push(42));
    CHECK(!rb.empty());
    CHECK_EQ(rb.size(), 1u);
    auto v = rb.pop();
    CHECK(v.has_value());
    CHECK(v && *v == 42);
    CHECK(rb.empty());
}

TEST(fifo_order)
{
    RingBuffer<int> rb(5);
    for (int i = 1; i <= 5; ++i)
        CHECK(rb.push(i * 10));
    for (int i = 1; i <= 5; ++i)
    {
        auto v = rb.pop();
        CHECK(v.has_value());
        CHECK(v && *v == i * 10);
    }
    CHECK(rb.empty());
}

TEST(becomes_full_at_exactly_capacity)
{
    RingBuffer<int> rb(3);
    CHECK(rb.push(1));
    CHECK(!rb.full());
    CHECK(rb.push(2));
    CHECK(!rb.full());
    CHECK(rb.push(3));
    CHECK(rb.full());
    CHECK_EQ(rb.size(), 3u);
}

TEST(push_on_full_fails_and_does_not_overwrite)
{
    RingBuffer<int> rb(2);
    CHECK(rb.push(1));
    CHECK(rb.push(2));
    CHECK(!rb.push(3));
    CHECK(!rb.push(4));
    CHECK_EQ(rb.size(), 2u);
    CHECK(rb.full());
    CHECK_EQ(rb.front(), 1);
    auto a = rb.pop();
    auto b = rb.pop();
    CHECK(a && *a == 1);
    CHECK(b && *b == 2);
    CHECK(!rb.pop().has_value());
}

TEST(pop_from_full_makes_room)
{
    RingBuffer<int> rb(2);
    CHECK(rb.push(1));
    CHECK(rb.push(2));
    CHECK(rb.pop().has_value());
    CHECK(!rb.full());
    CHECK(rb.push(3));
    CHECK(rb.full());
}

TEST(front_returns_oldest_without_removing)
{
    RingBuffer<int> rb(3);
    rb.push(7);
    rb.push(8);
    CHECK_EQ(rb.front(), 7);
    CHECK_EQ(rb.front(), 7);
    CHECK_EQ(rb.size(), 2u);
    rb.pop();
    CHECK_EQ(rb.front(), 8);
}

TEST(indices_wrap_around_correctly)
{
    RingBuffer<int> rb(3);
    // Push/pop far more elements than capacity so head and tail wrap many times.
    int next_in = 0;
    int next_out = 0;
    for (int round = 0; round < 50; ++round)
    {
        CHECK(rb.push(next_in++));
        CHECK(rb.push(next_in++));
        auto a = rb.pop();
        CHECK(a && *a == next_out++);
        auto b = rb.pop();
        CHECK(b && *b == next_out++);
        CHECK(rb.empty());
    }
}

TEST(wraparound_while_partially_full)
{
    RingBuffer<int> rb(4);
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.pop(); // 2,3
    rb.pop(); // 3
    rb.push(4);
    rb.push(5);
    rb.push(6); // 3,4,5,6 -- tail has wrapped past the end of storage
    CHECK(rb.full());
    CHECK(!rb.push(7));
    for (int expected : {3, 4, 5, 6})
    {
        CHECK_EQ(rb.front(), expected);
        auto v = rb.pop();
        CHECK(v && *v == expected);
    }
    CHECK(rb.empty());
}

TEST(size_tracks_across_wraparound)
{
    RingBuffer<int> rb(3);
    for (int i = 0; i < 20; ++i)
    {
        CHECK(rb.push(i));
        CHECK_EQ(rb.size(), 1u);
        CHECK(rb.pop().has_value());
        CHECK_EQ(rb.size(), 0u);
    }
}

TEST(capacity_one)
{
    RingBuffer<int> rb(1);
    CHECK(rb.empty());
    CHECK(rb.push(1));
    CHECK(rb.full());
    CHECK(!rb.push(2));
    auto v = rb.pop();
    CHECK(v && *v == 1);
    CHECK(rb.empty());
    CHECK(rb.push(3));
    auto w = rb.pop();
    CHECK(w && *w == 3);
}

TEST(works_with_non_trivial_types)
{
    RingBuffer<std::string> rb(3);
    CHECK(rb.push(std::string("hello, this string is long enough to defeat SSO")));
    CHECK(rb.push(std::string("world")));
    auto a = rb.pop();
    CHECK(a && *a == "hello, this string is long enough to defeat SSO");
    CHECK(rb.push(std::string("again")));
    CHECK(rb.push(std::string("and again")));
    CHECK(rb.full());
    auto b = rb.pop();
    auto c = rb.pop();
    auto d = rb.pop();
    CHECK(b && *b == "world");
    CHECK(c && *c == "again");
    CHECK(d && *d == "and again");
    CHECK(rb.empty());
}

TEST(elements_are_destroyed_exactly_once)
{
    // shared_ptr use_count as a leak probe: every handle the buffer holds
    // must be released when the buffer itself is destroyed. (ASan in the
    // Makefile catches raw leaks and double-frees on top of this.)
    auto token = std::make_shared<int>(1);
    {
        RingBuffer<std::shared_ptr<int>> rb(4);
        rb.push(token);
        rb.push(token);
        CHECK(token.use_count() >= 3);
    }
    CHECK_EQ(token.use_count(), 1);
}

TEST_MAIN()
