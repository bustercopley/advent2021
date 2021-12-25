#include "precompiled.h"
#include "symbols.h"

auto regex = re::regex(R"(^(\d+)$|^[v>.]+$)");

void parts(std::istream &stream) {
  bool test = false;
  ll expected;
  ll result{};
  vec<ss> grid;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, 1));
      } else {
        grid.push_back(move(line));
      }
    }
  }

  if (int height = size(grid)) {
    int width = size(grid[0]);
    bool keep_going;
    do {
      ++result;
      keep_going = false;
      auto new_grid = grid;
      for (int i = 0; i != height; ++i) {
        for (int j = 0; j != width; ++j) {
          if (grid[i][j] == '>') {
            if (grid[i][(j + 1) % width] == '.') {
              new_grid[i][j] = '.';
              new_grid[i][(j + 1) % width] = '>';
              keep_going = true;
            }
          }
        }
      }
      grid = new_grid;
      for (int i = 0; i != height; ++i) {
        for (int j = 0; j != width; ++j) {
          if (new_grid[i][j] == 'v') {
            if (new_grid[(i + 1) % height][j] == '.') {
              grid[i][j] = '.';
              grid[(i + 1) % height][j] = 'v';
              keep_going = true;
            }
          }
        }
      }
    } while (keep_going);
  }
  if (!test) {
    std::cout << "Result: " << result << std::endl;
  } else {
    if (result != expected) {
      std::cout << "Fail, result " << result << ", expected " << expected
                << std::endl;
    }
  }
}

int main() {
  std::cout << "Day 25" << std::endl;
  for (auto filename: {"test/25", "input/25"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
