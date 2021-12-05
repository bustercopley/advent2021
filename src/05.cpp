#include "binary.h"
#include "precompiled.h"
#include "symbols.h"
#include <ranges>

auto regex = re::regex(R"(^(\d+),(\d+)(?: -> (\d+),(\d+))?$)");

struct point {
  int x;
  int y;
  auto operator<=>(const point &) const = default;
};

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  std::map<point, int> grid;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (!matched(m, 3)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        point a{
          string_to<int>(match_string(m, 1)),
          string_to<int>(match_string(m, 2)),
        };
        point b{
          string_to<int>(match_string(m, 3)),
          string_to<int>(match_string(m, 4)),
        };

        if (a.x == b.x) {
          for (int y = std::min(a.y, b.y); y != std::max(a.y, b.y) + 1; ++y) {
            result += ++grid[point{a.x, y}] == 2;
          }
        } else if (a.y == b.y) {
          for (int x = std::min(a.x, b.x); x != std::max(a.x, b.x) + 1; ++x) {
            result += ++grid[point{x, a.y}] == 2;
          }
        } else if (part == 2) {
          for (int x = std::min(a.x, b.x); x != std::max(a.x, b.x) + 1; ++x) {
            int y = a.y + (x - a.x) * (b.y - a.y) / (b.x - a.x);
            result += ++grid[point{x, y}] == 2;
          }
        }
      }
    }
  }

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 5, Part One" << std::endl;
  for (auto filename: {"test/05", "input/05"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 5, Part Two" << std::endl;
  for (auto filename: {"test/05", "input/05"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
