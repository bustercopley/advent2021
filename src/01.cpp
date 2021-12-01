#include "precompiled.h"

auto regex = re::regex(R"(^(\d+)(?:,(\d+))?$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = -1, expected = -1;
  vec<int> entries;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 2)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        entries.push_back(string_to<ll>(match_string(m, 1)));
      }
    }
  }

  if (part == 1) {
    if (!empty(entries)) {
      result = 0;
      for (sz i = 1; i != size(entries); ++i) {
        result += entries[i - 1] < entries[i];
      }
    }
  } else {
    if (size(entries) >= 2) {
      ll previous = std::numeric_limits<ll>::min();
      for (sz i = 2; i != size(entries); ++i) {
        ll current = entries[i - 2] + entries[i - 1] + entries[i];
        result += previous < current;
        previous = current;
      }
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
  std::cout << "Day 1, Part One" << std::endl;
  for (auto filename: {"test/01", "input/01"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 1, Part Two" << std::endl;
  for (auto filename: {"test/01", "input/01"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
