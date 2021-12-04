#include "binary.h"
#include "precompiled.h"
#include "symbols.h"

auto regex =
  re::regex(R"(^(\d+),(\d+)$|(,)|^(?: *(\d+) +(\d+) +(\d+) +(\d+) +(\d+))$)");
auto regex2 = re::regex(R"((\d+),?(.*)$)");

struct brd {
  ary<ary<int, 5>, 5> grid;
  ary<int, 5> row_marks = {};
  ary<int, 5> col_marks = {};
  int score = 0;
  bool win = false;
};

constexpr int undef = std::numeric_limits<int>::max();

void parts(std::istream &stream, int part) {
  bool test = false;
  int result = -1;
  int expected = -1;

  vec<int> draws;
  vec<brd> boards;
  int row_counter = 0;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<int>(match_string(m, part));
      } else if (matched(m, 3)) {
        for (auto [r]: split(line, regex2)) {
          draws.push_back(string_to<int>(r));
        }
      } else if (matched(m, 4)) {
        if (!row_counter) { boards.emplace_back(); }
        auto &board = boards.back();
        for (int j = 0; j != 5; ++j) {
          int n = string_to<int>(match_string(m, 4 + j));
          board.grid[row_counter][j] = n;
          board.score += n;
        }
        row_counter = (row_counter + 1) % 5;
      }
    }
  }

  for (auto draw: draws) {
    for (auto &board: boards) {
      for (int i = 0; i != 5; ++i) {
        for (int j = 0; j != 5; ++j) {
          if (!board.win && board.grid[i][j] == draw) {
            board.grid[i][j] = undef;
            board.score -= draw;
            if ((++board.row_marks[i] == 5) | (++board.col_marks[j] == 5)) {
              board.win = true;
              result = draw * board.score;
              if (part == 1) { goto out; }
            }
          }
        }
      }
    }
  }
out:
  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 4, Part One" << std::endl;
  for (auto filename: {"test/04", "input/04"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 4, Part Two" << std::endl;
  for (auto filename: {"test/04", "input/04"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
