#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^(\d+)$)");

struct point {
  int x;
  int y;
  bool operator==(const point &) const = default;
};

struct data {
  int h;
  int index;
};

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  vec<vec<data>> grid;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        auto &row = grid.emplace_back();
        for (auto c: match_string(m, 3)) {
          row.push_back(data{c - '0', -1});
        }
      }
    }
  }

  int cx = static_cast<int>(std::size(grid[0]));
  int cy = static_cast<int>(std::size(grid));

  std::vector<int> basins;
  std::vector<point> stack;

  for (int y = 0; y != cy; ++y) {
    for (int x = 0; x != cx; ++x) {
      auto &p = grid[y][x];
      if (x != 0 && grid[y][x - 1].h <= p.h) continue;
      if (x != cx - 1 && grid[y][x + 1].h <= p.h) continue;
      if (y != 0 && grid[y - 1][x].h <= p.h) continue;
      if (y != cy - 1 && grid[y + 1][x].h <= p.h) continue;
      result += p.h + 1;
      p.index = size(basins);
      basins.push_back(0);
      stack.push_back(point{x, y});
    }
  }

  if (part == 2) {
    while (!stack.empty()) {
      auto [x, y] = stack.back();
      stack.pop_back();
      auto &p = grid[y][x];
      ++basins[p.index];
      auto explore = [&](int x, int y) -> void {
        if (grid[y][x].index == -1 && grid[y][x].h != 9) {
          grid[y][x].index = p.index;
          stack.push_back(point{x, y});
        }
      };
      if (x != 0) explore(x - 1, y);
      if (x != cx - 1) explore(x + 1, y);
      if (y != 0) explore(x, y - 1);
      if (y != cy - 1) explore(x, y + 1);
    }
  }

  if (part == 2) {
    std::ranges::sort(basins, std::greater<sz>{});
    result = basins[0] * basins[1] * basins[2];
  }

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 9, Part One" << std::endl;
  for (auto filename: {"test/09", "input/09"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 9, Part Two" << std::endl;
  for (auto filename: {"test/09", "input/09"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
