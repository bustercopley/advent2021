#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^(\d+)$)");

struct data {
  int energy;
  bool flashed;
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
          row.push_back(data{c - '0', false});
        }
      }
    }
  }

  int cy = size(grid);
  if (!cy) return;
  int cx = size(grid[0]);
  for (int step = 0; part == 2 || step != 100; ++step) {
    auto copy = grid;
    for (auto &row: grid) {
      for (auto &elt: row) {
        ++elt.energy;
      }
    }
    bool keep_going;
    do {
      keep_going = false;
      for (int y = 0; y != cy; ++y) {
        for (int x = 0; x != cx; ++x) {
          if (grid[y][x].energy > 9 && !grid[y][x].flashed) {
            keep_going = true;
            grid[y][x].flashed = true;
            for (int y1 = -1; y1 != 2; ++y1) {
              if (y + y1 != -1 && y + y1 != cy) {
                for (int x1 = -1; x1 != 2; ++x1) {
                  if (x + x1 != -1 && x + x1 != cx && (x1 || y1)) {
                    ++grid[y + y1][x + x1].energy;
                  }
                }
              }
            }
          }
        }
      }
    } while (keep_going);
    bool all_flashed = true;
    for (auto &row: grid) {
      for (auto &[energy, flashed]: row) {
        if (flashed) {
          energy = 0;
          flashed = 0;
          ++result;
        } else {
          all_flashed = false;
        }
      }
    }
    if (part == 2 && all_flashed) {
      result = step + 1;
      break;
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
  std::cout << "Day 11, Part One" << std::endl;
  for (auto filename: {"test/11", "input/11"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 11, Part Two" << std::endl;
  for (auto filename: {"test/11", "input/11"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
