#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^\d+$)");

constexpr auto undef = std::numeric_limits<int>::max();

struct point {
  int x;
  int y;
  auto operator<=>(const point &) const = default;
};

struct data {
  ll risk;
  ll best;
};

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  vec<vec<data>> grid;
  int cx = 0;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        cx = size(line);
        std::ranges::transform(line, begin(grid.emplace_back(cx)), [](char c) {
          return data{c - '0', undef};
        });
      }
    }
  }
  int cy = size(grid);

  if (part == 2) {
    vec<vec<data>> new_grid(5 * cy, vec<data>(5 * cx));
    for (int y = 0; y != cy; ++y) {
      for (int x = 0; x != cx; ++x) {
        new_grid[y][x] = grid[y][x];
      }
    }
    for (int y = 0; y != cy; ++y) {
      for (int x = cx; x != 5 * cx; ++x) {
        new_grid[y][x] = data{new_grid[y][x - cx].risk % 9 + 1, undef};
      }
    }
    for (int y = cy; y != 5 * cy; ++y) {
      for (int x = 0; x != 5 * cx; ++x) {
        new_grid[y][x] = data{new_grid[y - cy][x].risk % 9 + 1, undef};
      }
    }
    swap(grid, new_grid);
    cx = 5 * cx;
    cy = 5 * cy;
  }

  auto neighbor = [cx, cy](int x, int y,
                    int direction) -> std::tuple<bool, int, int> {
    switch (direction) {
    case 0:
      if (x) return {true, x - 1, y};
      break;
    case 1:
      if (x != cx - 1) return {true, x + 1, y};
      break;
    case 2:
      if (y) return {true, x, y - 1};
      break;
    case 3:
      if (y != cy - 1) return {true, x, y + 1};
      break;
    }
    return {};
  };

  grid[0][0].best = 0;
  std::map<ll, std::set<point>> stacks;
  stacks[0].emplace(0, 0);
  while (!empty(stacks) && begin(stacks)->first < grid[cy - 1][cx - 1].best) {
    auto stack = move(begin(stacks)->second);
    stacks.erase(begin(stacks));
    for (const auto &[x, y]: stack) {
      for (int direction = 0; direction != 4; ++direction) {
        if (auto [ok, nx, ny] = neighbor(x, y, direction); ok) {
          ll best = grid[y][x].best + grid[ny][nx].risk;
          if (grid[ny][nx].best > best) {
            grid[ny][nx].best = best;
            stacks[best].emplace(nx, ny);
          }
        }
      }
    }
  }

  result = grid[cy - 1][cx - 1].best;

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 15, Part One" << std::endl;
  for (auto filename: {"test/15", "input/15"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 15, Part Two" << std::endl;
  for (auto filename: {"test/15", "input/15"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
