#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^(\d+)$)");

constexpr auto undef = std::numeric_limits<sz>::max();

struct data {
  int height;
  sz basin;
};

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  vec<data> grid{};
  sz cx = 0;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        const auto &row = match_string(m, 3);
        cx = size(row);
        for (auto c: row) {
          grid.emplace_back(c - '0', undef);
        }
      }
    }
  }

  auto next = [&grid, cx](sz index) {
    int height = grid[index].height;
    sz x = index % cx;
    if (x && grid[index - 1].height < height) {
      index = index - 1;
    } else if (x + 1 != cx && grid[index + 1].height < height) {
      index = index + 1;
    } else if (index >= cx && grid[index - cx].height < height) {
      index = index - cx;
    } else if (index + cx < size(grid) && grid[index + cx].height < height) {
      index = index + cx;
    } else {
      index = undef;
    }
    return index;
  };

  vec<sz> basins;
  vec<sz> path;
  for (sz start = 0; start != size(grid); ++start) {
    if (grid[start].height != 9 && grid[start].basin == undef) {
      path.clear();
      sz i = start;
      while (grid[i].basin == undef) {
        path.push_back(i);
        if (sz j = next(i); j != undef) {
          i = j;
        } else {
          grid[i].basin = size(basins);
          basins.push_back(0);
          result += grid[i].height + 1;
        }
      }
      basins[grid[i].basin] += size(path);
      for (sz j: path) {
        grid[j].basin = grid[i].basin;
      }
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
