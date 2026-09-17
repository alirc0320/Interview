#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace minitest
{

inline std::vector<std::pair<std::string, std::function<void()>>>& tests()
{
    static std::vector<std::pair<std::string, std::function<void()>>> t;
    return t;
}

struct Registrar
{
    Registrar(const std::string& name, std::function<void()> fn)
    {
        tests().push_back({name, std::move(fn)});
    }
};

inline int checks = 0;
inline int failures = 0;

constexpr const char* kGreen = "\033[32m";
constexpr const char* kRed = "\033[31m";
constexpr const char* kReset = "\033[0m";

inline int run_all()
{
    for (auto& [name, fn] : tests())
    {
        int failures_before = failures;
        fn();
        bool passed = failures == failures_before;
        std::cerr << "-- " << name << ": " << (passed ? kGreen : kRed) << (passed ? "PASS" : "FAIL")
                  << kReset << "\n";
    }
    std::cerr << tests().size() << " test case(s), " << checks << " check(s), "
              << (failures == 0 ? kGreen : kRed) << failures << " failure(s)" << kReset << "\n";
    return failures == 0 ? 0 : 1;
}

} // namespace minitest

#define TEST(name)                                                                                 \
    static void name();                                                                            \
    static minitest::Registrar registrar_##name(#name, name);                                      \
    static void name()

#define CHECK(expr)                                                                                \
    do                                                                                             \
    {                                                                                              \
        ++minitest::checks;                                                                        \
        if (!(expr))                                                                               \
        {                                                                                          \
            ++minitest::failures;                                                                  \
            std::cerr << "  CHECK failed: " << #expr << " at " << __FILE__ << ":" << __LINE__      \
                      << "\n";                                                                     \
        }                                                                                          \
    } while (0)

#define CHECK_EQ(a, b)                                                                             \
    do                                                                                             \
    {                                                                                              \
        ++minitest::checks;                                                                        \
        auto&& va_ = (a);                                                                          \
        auto&& vb_ = (b);                                                                          \
        if (!(va_ == vb_))                                                                         \
        {                                                                                          \
            ++minitest::failures;                                                                  \
            std::cerr << "  CHECK_EQ failed: " << #a << " != " << #b << "  (" << va_ << " vs "     \
                      << vb_ << ") at " << __FILE__ << ":" << __LINE__ << "\n";                    \
        }                                                                                          \
    } while (0)

#define TEST_MAIN()                                                                                \
    int main()                                                                                     \
    {                                                                                              \
        return minitest::run_all();                                                                \
    }
