#include "binary.h"
#include "precompiled.h"
#include "symbols.h"
#include <ranges>

auto regex = re::regex(R"(^(\d+),(\d+)|(\d+)$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result;
  ll expected = -1;
  vec<ss> report;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        report.push_back(match_string(m, 3));
      }
    }
  }

  if (part == 1) {
    vec<sz> freq;
    for (auto bits: report) {
      freq.resize(size(bits));
      for (int i = 0; char c: bits) {
        freq[i] += (c == '1');
        ++i;
      }
    }
    ll gamma = 0, epsilon = 0;
    for (auto count: freq) {
      gamma = (gamma << 1) | (2 * count > size(report));
      epsilon = (epsilon << 1) | (2 * count < size(report));
    }
    result = gamma * epsilon;
  } else {
    std::ranges::sort(report);
    result = 1;
    for (auto flag: {false, true}) {
      std::ranges::subrange range{report};
      sz index = 0;
      while (size(range) > 1) {
        auto middle = std::ranges::find_if(range,
          [index](auto item) { return item[index] == '1'; });
        auto bit = flag ^ (middle - begin(range) <= end(range) - middle);
        range = (bit ? std::ranges::subrange{begin(range), middle}
                     : std::ranges::subrange{middle, end(range)});
        ++index;
      }
      result *= from_binary(*begin(range));
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
  std::cout << "Day 3, Part One" << std::endl;
  for (auto filename: {"test/03", "input/03"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 3, Part Two" << std::endl;
  for (auto filename: {"test/03", "input/03"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
