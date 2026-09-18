#include "minitest.h"
#include "solution.h"

#include <string>

// Contract: LRU cache. get() on a hit returns the value and marks the key
// most-recently-used; a miss returns std::nullopt. put() inserts or
// overwrites (either way the key becomes most-recently-used) and, when
// inserting a new key into a full cache, evicts the least-recently-used
// key. Capacity 0 stores nothing.

namespace
{
bool has(FileCache& c, const std::string& key, const std::string& value)
{
    auto v = c.get(key);
    return v.has_value() && *v == value;
}
bool missing(FileCache& c, const std::string& key)
{
    return !c.get(key).has_value();
}
} // namespace

TEST(miss_on_empty_cache)
{
    FileCache c(2);
    CHECK(missing(c, "a"));
}

TEST(put_then_get)
{
    FileCache c(2);
    c.put("a", "1");
    CHECK(has(c, "a", "1"));
}

TEST(stores_up_to_capacity)
{
    FileCache c(3);
    c.put("a", "1");
    c.put("b", "2");
    c.put("c", "3");
    CHECK(has(c, "a", "1"));
    CHECK(has(c, "b", "2"));
    CHECK(has(c, "c", "3"));
}

TEST(evicts_least_recently_inserted_when_untouched)
{
    FileCache c(2);
    c.put("a", "1");
    c.put("b", "2");
    c.put("c", "3");
    CHECK(missing(c, "a"));
    CHECK(has(c, "b", "2"));
    CHECK(has(c, "c", "3"));
}

TEST(get_refreshes_recency)
{
    FileCache c(2);
    c.put("a", "1");
    c.put("b", "2");
    CHECK(has(c, "a", "1")); // a is now MRU, b is LRU
    c.put("c", "3");
    CHECK(has(c, "a", "1"));
    CHECK(missing(c, "b"));
    CHECK(has(c, "c", "3"));
}

TEST(put_of_existing_key_updates_value_without_evicting)
{
    FileCache c(2);
    c.put("a", "1");
    c.put("b", "2");
    c.put("a", "one");
    CHECK(has(c, "a", "one"));
    CHECK(has(c, "b", "2"));
}

TEST(put_of_existing_key_refreshes_recency)
{
    FileCache c(2);
    c.put("a", "1");
    c.put("b", "2");
    c.put("a", "1b"); // a is MRU, b is LRU
    c.put("c", "3");
    CHECK(has(c, "a", "1b"));
    CHECK(missing(c, "b"));
    CHECK(has(c, "c", "3"));
}

TEST(missed_get_does_not_disturb_order)
{
    FileCache c(2);
    c.put("a", "1");
    c.put("b", "2");
    CHECK(missing(c, "zzz"));
    c.put("c", "3");
    CHECK(missing(c, "a")); // a was still the LRU
    CHECK(has(c, "b", "2"));
}

TEST(capacity_one)
{
    FileCache c(1);
    c.put("a", "1");
    CHECK(has(c, "a", "1"));
    c.put("b", "2");
    CHECK(missing(c, "a"));
    CHECK(has(c, "b", "2"));
    c.put("b", "22");
    CHECK(has(c, "b", "22"));
}

TEST(capacity_zero_stores_nothing)
{
    FileCache c(0);
    c.put("a", "1");
    CHECK(missing(c, "a"));
}

TEST(eviction_order_follows_access_pattern)
{
    FileCache c(3);
    c.put("a", "1");
    c.put("b", "2");
    c.put("c", "3");
    c.get("a");       // order LRU->MRU: b c a
    c.get("b");       // c a b
    c.put("d", "4");  // evicts c
    CHECK(missing(c, "c"));
    c.put("e", "5");  // evicts a  (order was a b d)
    CHECK(missing(c, "a"));
    CHECK(has(c, "b", "2"));
    CHECK(has(c, "d", "4"));
    CHECK(has(c, "e", "5"));
}

TEST(empty_string_key_and_value_are_ordinary)
{
    FileCache c(2);
    c.put("", "");
    CHECK(has(c, "", ""));
    c.put("k", "");
    CHECK(has(c, "k", ""));
}

TEST(large_values_are_preserved)
{
    FileCache c(2);
    std::string big(100000, 'x');
    c.put("big", big);
    CHECK(has(c, "big", big));
}

TEST(many_inserts_keep_only_the_most_recent_capacity_keys)
{
    FileCache c(10);
    for (int i = 0; i < 1000; ++i)
        c.put(std::to_string(i), std::to_string(i * 2));
    for (int i = 0; i < 990; ++i)
        CHECK(missing(c, std::to_string(i)));
    for (int i = 990; i < 1000; ++i)
        CHECK(has(c, std::to_string(i), std::to_string(i * 2)));
}

TEST_MAIN()
