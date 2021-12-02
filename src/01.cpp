#include "precompiled.h"

auto regex = re::regex(R"(^(\d+)(?:,(\d+))?$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0, expected = -1;
  sz size = (part == 1 ? 2 : 4);
  sz index = 0;
  vec<int> entries(size, std::numeric_limits<int>::max());

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 2)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        sz next = (index + 1) % size;
        entries[index] = string_to<ll>(match_string(m, 1));
        result += entries[index] > entries[next];
        index = next;
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
