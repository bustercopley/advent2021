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
  std::set<point> image;
  int width = 0;
  int height = 0;
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
          width = 0;
          for (char c: line) {
            if (c == '#') { image.emplace(width, height); }
            ++width;
          }
          ++height;
        }
      }
    }
  }

  if (size(algo) != 512 || !width || !height) {
    std::cout << "Bailing out, size " << size(algo) << ", width " << width
              << ", height " << height << std::endl;
    return;
  }

  bool invert_input = false;
  bool invert_output = (algo[0] == '#');

  for (int part: {0, 1}) {
    for (int i = 0; i != (part ? 48 : 2); ++i) {
      std::set<point> newimage;

      width += 4;
      height += 4;
      for (int y = 0; y != height; ++y) {
        for (int x = 0; x != width; ++x) {
          int n = 0;
          n |= image.contains(point{x - 4, y - 4}) << 8;
          n |= image.contains(point{x - 3, y - 4}) << 7;
          n |= image.contains(point{x - 2, y - 4}) << 6;
          n |= image.contains(point{x - 4, y - 3}) << 5;
          n |= image.contains(point{x - 3, y - 3}) << 4;
          n |= image.contains(point{x - 2, y - 3}) << 3;
          n |= image.contains(point{x - 4, y - 2}) << 2;
          n |= image.contains(point{x - 3, y - 2}) << 1;
          n |= image.contains(point{x - 2, y - 2}) << 0;
          if (invert_input) n ^= 511;
          if ((algo[n] == '#') ^ invert_output) { newimage.emplace(x, y); }
        }
      }

      std::swap(invert_input, invert_output);
      swap(image, newimage);
    }

    result[part] = size(image);
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
