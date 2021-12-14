#include "precompiled.h"
#include "symbols.h"
#include "y-combinator.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^(.. -> .)$|^\w\w+$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  std::map<ss, sz> polymer;
  std::map<ss, char> rules;
  char first{};
  char last{};

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else if (matched(m, 3)) {
        rules[line.substr(0, 2)] = line[6];
      } else {
        for (sz index = 1; index != size(line); ++index) {
          ++polymer[line.substr(index - 1, 2)];
        }
        first = line.front();
        last = line.back();
      }
    }
  }

  for (int i = 0; i != (part == 1 ? 10 : 40); ++i) {
    std::map<ss, sz> new_polymer;
    for (auto &[pair, count]: polymer) {
      if (auto iter = rules.find(pair); iter != end(rules)) {
        new_polymer[ss{pair[0], iter->second}] += count;
        new_polymer[ss{iter->second, pair[1]}] += count;
      } else {
        new_polymer[pair] += count;
      }
    }
    swap(polymer, new_polymer);
  }

  std::map<char, sz> histogram;
  for (auto &[pair, count]: polymer) {
    histogram[pair[0]] += count;
    histogram[pair[1]] += count;
  }
  ++histogram[first];
  ++histogram[last];
  vec<sz> counts;
  for (auto [ignored, count]: histogram) {
    counts.push_back(count);
  }
  std::ranges::sort(counts);
  result = (counts.back() - counts.front()) / 2;

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
