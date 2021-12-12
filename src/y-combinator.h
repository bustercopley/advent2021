// -*- C++ -*-
#ifndef y_combinator_cpp
#define y_combinator_cpp

// C++17 Y combinator by Barry[1]
// [1] https://stackoverflow.com/a/40873657/1396177

#include <functional>

template <typename F> struct y_combinator {
  F f;
  template <typename... Args> decltype(auto) operator()(Args &&...args) const {
    return f(std::cref(*this), std::forward<Args>(args)...);
  }
};

template <typename F> y_combinator(F &&f) -> y_combinator<F>;

// Usage:
//
// y_combinator fib{[](auto fib, unsigned n) -> unsigned {
//   return n < 2u ? 1u : fib(n - 2u) + fib(n - 1u);
// }};
//
// std::cout << fib(16) << std::endl; // 1597

#endif
