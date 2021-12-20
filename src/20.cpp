#include "binary.h"
#include "precompiled.h"
#include <cmath>
#include <cstdio>

auto regex = re::regex(R"(^(\d+),(\d+)$|^[#.]+$)");

struct point {
  int x;
  int y;
  auto operator<=>(const point &) const = default;
};

void parts(std::istream &stream) {
  bool test = false;
  int expected[2];
  int result[2]{};

  ss algo;
  vec<ss> image;
  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected[0] = string_to<ll>(match_string(m, 1));
        expected[1] = string_to<ll>(match_string(m, 2));
      } else {
        if (empty(algo)) {
          algo = line;
        } else {
          image.push_back(move(line));
        }
      }
    }
  }

  if (size(algo) != 512) {
    std::cout << "Bailing out, size " << size(algo) << std::endl;
    return;
  }

  bool invert_input = false;
  bool invert_output = (algo[0] == '#');

  for (int part: {0, 1}) {
    for (int i = 0; i != (part ? 48 : 2); ++i) {
      int width = size(image[0]) + 4;
      int height = size(image) + 4;

      for (auto &line: image) {
        line = ".." + line + "..";
      }
      image.emplace(begin(image), width, '.');
      image.emplace(begin(image), width, '.');
      image.push_back(image.front());
      image.push_back(image.front());

      for (int y = 0; y != height - 2; ++y) {
        for (int x = 0; x != width - 2; ++x) {
          int n = 0;
          n |= (image[y + 0][x + 0] == '#') << 8;
          n |= (image[y + 0][x + 1] == '#') << 7;
          n |= (image[y + 0][x + 2] == '#') << 6;
          n |= (image[y + 1][x + 0] == '#') << 5;
          n |= (image[y + 1][x + 1] == '#') << 4;
          n |= (image[y + 1][x + 2] == '#') << 3;
          n |= (image[y + 2][x + 0] == '#') << 2;
          n |= (image[y + 2][x + 1] == '#') << 1;
          n |= (image[y + 2][x + 2] == '#') << 0;
          if (invert_input) n ^= 511;
          image[y][x] = ((algo[n] == '#') ^ invert_output) ? '#' : '.';
        }
      }

      std::swap(invert_input, invert_output);
    }

    result[part] = 0;
    for (const auto &line: image) {
      result[part] += std::ranges::count(line, '#');
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
}

int main() {
  std::cout << "Day 20" << std::endl;
  for (auto filename: {"test/20", "input/20"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
