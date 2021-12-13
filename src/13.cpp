#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^fold along (?:x|(y))=(\d+)$)");

struct point {
  int x;
  int y;
  auto operator<=>(const point &) const = default;
};

void parts(std::istream &stream) {
  std::set<point> grid;
  bool part_one_done = false;
  int cx = 0;
  int cy = 0;
  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        int x = string_to<int>(match_string(m, 1));
        int y = string_to<int>(match_string(m, 2));
        grid.emplace(x, y);
      } else {
        bool horizontal = matched(m, 3);
        int position = string_to<int>(match_string(m, 4));
        std::set<point> new_grid;
        for (auto [x, y]: grid) {
          if (horizontal) {
            if (y > position) { y = 2 * position - y; }
          } else {
            if (x > position) { x = 2 * position - x; }
          }
          new_grid.emplace(x, y);
        }
        swap(grid, new_grid);
        (horizontal ? cy : cx) = position;
        if (!part_one_done) {
          std::cout << "Part one: " << size(grid) << std::endl;
          part_one_done = true;
        }
      }
    }
  }

  std::cout << "Part two:\n";
  point p;
  for (p.y = 0; p.y != cy; ++p.y) {
    for (p.x = 0; p.x != cx; ++p.x) {
      std::cout << (grid.contains(p) ? '#' : '.');
    }
    std::cout << "\n";
  }
}

int main() {
  std::cout << "Day 13" << std::endl;
  for (auto filename: {"test/13", "input/13"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
