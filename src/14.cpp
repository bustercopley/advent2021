#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^([A-Z][A-Z] -> [A-Z])$|^[A-Z]{2,}$)");
using couple = std::array<int, 2>;

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  std::map<couple, sz> polymer;
  std::map<couple, int> rules;
  vec<sz> histogram(26);

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else if (matched(m, 3)) {
        rules[couple{line[0] - 'A', line[1] - 'A'}] = line[6] - 'A';
      } else {
        for (sz index = 1; index != size(line); ++index) {
          ++polymer[couple{line[index - 1] - 'A', line[index] - 'A'}];
        }
        ++histogram[line.front() - 'A'];
        ++histogram[line.back() - 'A'];
      }
    }
  }

  for (int i = 0; i != (part == 1 ? 10 : 40); ++i) {
    std::map<couple, sz> new_polymer;
    for (auto &[pair, count]: polymer) {
      if (auto iter = rules.find(pair); iter != end(rules)) {
        new_polymer[couple{pair[0], iter->second}] += count;
        new_polymer[couple{iter->second, pair[1]}] += count;
      } else {
        new_polymer[pair] += count;
      }
    }
    swap(polymer, new_polymer);
  }

  for (auto &[pair, count]: polymer) {
    histogram[pair[0]] += count;
    histogram[pair[1]] += count;
  }

  sz low = std::numeric_limits<sz>::max();
  sz high = std::numeric_limits<sz>::min();
  for (auto count: histogram) {
    if (count) {
      low = std::min(low, count);
      high = std::max(high, count);
    }
  }

  result = (high - low) / 2;

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 14, Part One" << std::endl;
  for (auto filename: {"test/14", "input/14"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 14, Part Two" << std::endl;
  for (auto filename: {"test/14", "input/14"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
