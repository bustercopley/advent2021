#include "precompiled.h"
#include <cmath>

auto regex =
  re::regex(R"(.*x=(\d+)\.\.(\d+), y=(-\d+)\.\.(-\d+)(?:;(\d+),(\d+))?$)");

constexpr double f(double t, double v) { return -0.5 * t * t + (v + 0.5) * t; }
constexpr double g(double v, double x) {
  return v + 0.5 + std::sqrt(v * v + v + 0.25 - 2.0 * x);
}

void parts(std::istream &stream) {
  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      bool test = false;
      int expected[2];
      int targetx[2];
      int targety[2];
      int result[2]{};
      targetx[0] = string_to<ll>(match_string(m, 1));
      targetx[1] = string_to<ll>(match_string(m, 2));
      targety[0] = string_to<ll>(match_string(m, 3));
      targety[1] = string_to<ll>(match_string(m, 4));
      if (matched(m, 5)) {
        test = true;
        expected[0] = string_to<ll>(match_string(m, 5));
        expected[1] = string_to<ll>(match_string(m, 6));
      }
      result[0] = std::numeric_limits<int>::min();
      for (double v = -100000.0; v <= 100000.0; ++v) {
        double t0 = g(v, targety[1]); // fractional time y crosses top of box
        double t1 = g(v, targety[0]); // fractional time y crosses bottom of box
        if (std::ceil(t0) <= std::floor(t1)) {
          std::set<int> us;
          for (double t = std::ceil(t0); t <= std::floor(t1); ++t) {
            for (int u = 0; u <= 100000; ++u) {
              int x = t >= u ? f(u, u) : f(t, u);
              if (targetx[0] <= x && x <= targetx[1]) {
                int apex = v > 0.0 ? static_cast<int>(f(v, v)) : 0;
                result[0] = std::max(result[0], apex);
                us.insert(u);
              }
            }
          }
          result[1] += size(us);
          // 926 too low
        }
      }
      if (!test) {
        std::cout << "Part 1 result " << result[0] << "\nPart 2 result "
                  << result[1] << std::endl;
      } else {
        for (int part: {1, 2}) {
          if (result[part - 1] != expected[part - 1]) {
            std::cout << "Part " << part << " fail, result " << result[part - 1]
                      << ", expected " << expected[part - 1] << std::endl;
          }
        }
      }
    } else {
      std::cout << "no match: " << line << std::endl;
    }
  }
}

int main() {
  std::cout << "Day 17" << std::endl;
  for (auto filename: {"test/17", "input/17"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
