#pragma once
#include <cstddef>
#include <optional>
#include <string>

// TODO: implement a simple LRU block/file cache: fixed capacity,
// get() returns cached data if present (and marks it recently used),
// put() inserts/updates and evicts the least-recently-used entry
// when full.
class FileCache
{
  public:
    explicit FileCache(std::size_t capacity);

    std::optional<std::string> get(const std::string& key);
    void put(const std::string& key, std::string value);

  private:
    // TODO
};
