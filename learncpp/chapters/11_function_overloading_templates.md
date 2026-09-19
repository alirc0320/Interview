---
chapter: Chapter 11 — Function Overloading and Function Templates
---

# Chapter: Function Overloading and Function Templates

Close the chapter. Write these five from memory. Do not look anything up.

## 1. Concept explanation

Q: How does the compiler pick which overload to call when multiple overloads could match a given call (the ranking of exact match, promotion, conversion), and under what circumstances does this process fail with an "ambiguous call" error instead of just picking the best one?

A:

When several overloads could match a call, the compiler looks only at the function name and the parameter list; the return type is ignored. It ranks how well each argument matches each candidate's parameter, from best to worst:

1. Exact match
2. Numeric promotion
3. Numeric conversion
4. User defined conversion

For pointer and reference parameters, const and volatile qualifiers also distinguish overloads, and const distinguishes member functions. They do not distinguish by value parameters, because top-level const is ignored.

The best overload is the one that is at least as good as every other candidate on every argument and strictly better on at least one.

- If no candidate can be reached through any of these conversions, the call fails with a "no matching function" error.
- If two or more candidates tie for best, the call fails with an "ambiguous call" error instead, unless a tie-breaker applies: a non-template function is preferred over a function template that matches equally well, and a more specialized template is preferred over a more general one.
- With multiple parameters, ambiguity also happens when each overload wins on a different argument.

For example, if a char argument is passed to overloads taking float and double, both are numeric conversions, so they tie and the call is ambiguous.

## 2. Does this compile?

Q:

```cpp
#include <iostream>

void print(int x)
{
    std::cout << "int: " << x << '\n';
}

void print(double x)
{
    std::cout << "double: " << x << '\n';
}

int main()
{
    print('a');
    print(5.0f);
    return 0;
}
```

A:

Yes, this should compile because the first print statement in main will cause the compiler to use numeric promotion and call the print function that uses an int, however the numeric ASCII value will be printed out. The second print statement will also cause the float argument to be interpreted by the compiler as a numeric promotion to a double, so the print function that uses a double is called.

Output:

```
int: 97
double: 5
```

## 3. Predict the output

Q:

```cpp
#include <iostream>

template <typename T>
T maxOf(T a, T b)
{
    return (a > b) ? a : b;
}

int maxOf(int a, int b)
{
    std::cout << "non-template used\n";
    return (a > b) ? a : b;
}

int main()
{
    std::cout << maxOf(3, 7) << '\n';
    std::cout << maxOf(3.5, 7.5) << '\n';
    return 0;
}
```

A:

```
non-template used
7
7.5
```

## 4. Debugging / undefined behavior

Q: A project declares a function template in a header and calls it from two different `.cpp` files:

```cpp
// mathutils.h
template <typename T>
T square(T x);

// mathutils.cpp
template <typename T>
T square(T x) { return x * x; }
```

`main.cpp` includes `mathutils.h` and calls `square(5)`. The build fails at the link stage with an "undefined reference to square<int>" error even though everything compiles individually. What's happening, and what's the fix?

A:

Two problems combine to cause this:

1. **The template is not defined where it is used.** `main.cpp` only sees the declaration of `square` in the header, so when it calls `square(5)` the compiler cannot instantiate `square<int>`. It instead emits a reference to that symbol, expecting it to exist in another object file.
2. **The template is not instantiated where it is defined.** `mathutils.cpp` contains the definition, but nothing in that file uses `square<int>`, so the compiler never instantiates it and no `square<int>` symbol is generated.

The linker then finds nothing to resolve the reference against and reports "undefined reference to square<int>". A template is only a blueprint, and code is generated only where it is instantiated. Fixing either problem is enough, because the linker only needs one object file to contain `square<int>`.

There are three fixes:

1. Move the template definition into the header, so every translation unit that calls `square` can instantiate its own copy (this fixes problem 1). This is the standard fix. It is safe because templates and their instantiations are exempt from the one-definition rule, as long as the definitions are identical.
2. Copy the definition into each `.cpp` file that uses it. This also works for the same ODR reason, but it duplicates code and is error-prone, so it is not recommended.
3. Keep the definition in `mathutils.cpp` and explicitly instantiate it there with `template int square<int>(int);`. This forces the compiler to generate the symbol (this fixes problem 2), but it limits the template to the types listed.

## 5. Implementation

Q: Write a function template `T clampValue(T value, T lo, T hi)` that returns `value` clamped into the range `[lo, hi]`. Use a non-type template parameter or a default argument to make the clamping behavior optionally invertible (your choice), and demonstrate it in `main()` with at least two different types (e.g. `int` and `double`).

A:

```cpp
#include <iostream>

template <typename T>
T clampValue(T value, T lo, T hi)
{
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

int main()
{
    std::cout << clampValue(15, 0, 10) << '\n';       // 10 (T = int)
    std::cout << clampValue(-2.5, -1.0, 1.0) << '\n'; // -1 (T = double)
    std::cout << clampValue('m', 'a', 'f') << '\n';   // f (T = char)
    return 0;
}
```

---

## Corrections

Reopen the chapter. Fix anything wrong above (don't erase the original wrong
answer — strike it through or note what was wrong, so the mistake stays
visible on review).

-

## Missed on first pass?

- [ ] Yes — copy the missed question(s) into `review/missed_questions.md`
- [ ] No
