#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)|(?:forward|(up)|(down)) (\d+)$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = -1, expected = -1;
  ll depth = 0, position = 0, aim = 0;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else if (part == 1) {
        ll value = string_to<ll>(match_string(m, 5));
        if (matched(m, 3)) {
          depth -= value;
        } else if (matched(m, 4)) {
          depth += value;
        } else {
          position += value;
        }
      } else {
        ll value = string_to<ll>(match_string(m, 5));
        if (matched(m, 3)) {
          aim -= value;
        } else if (matched(m, 4)) {
          aim += value;
        } else {
          position += value;
          depth += value * aim;
        }
      }
    }
  }

  result = depth * position;

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 1, Part One" << std::endl;
  for (auto filename: {"test/02", "input/02"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 1, Part Two" << std::endl;
  for (auto filename: {"test/02", "input/02"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
