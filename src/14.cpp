#include "precompiled.h"
#include "symbols.h"
#include "y-combinator.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^(.. -> .)$|^\w\w+$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  ss polymer;
  std::map<ss, char> rules;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else if (matched(m, 3)) {
        rules[line.substr(0, 2)] = line[6];
      } else {
        polymer = line;
      }
    }
  }

  if (part == 1) {
    for (int i = 0; i != 10; ++i) {
      ss new_polymer;
      new_polymer += polymer[0];
      for (sz index = 1; index != size(polymer); ++index) {
        if (auto iter = rules.find(polymer.substr(index - 1, 2)); iter != end(rules)) {
          new_polymer += iter->second;
        }
        new_polymer += polymer[index];
      }
      polymer = new_polymer;
    }
    std::map<char, int> histogram;
    for (auto c: polymer) {
      ++histogram[c];
    }
    vec<int> counts;
    for (auto [ignored, count]: histogram) {
      counts.push_back(count);
    }
    std::ranges::sort(counts);
    result = counts.back() - counts.front();
  }


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
