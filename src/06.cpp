#include "binary.h"
#include "precompiled.h"
#include "symbols.h"
#include <ranges>

auto regex = re::regex(R"(^(\d+),(\d+)$|^(\d+(?:,\d+)*)$)");
auto regex2 = re::regex(R"((\d+)(.*))");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  vec<ll> hist(9);

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        for (auto [s]: split(line, regex2)) {
          ++hist[string_to<ll>(s)];
        }
      }
    }
  }

  ll days = part == 1 ? 80 : 256;

  for (ll i = 0; i != days; ++i) {
    std::rotate(begin(hist), next(begin(hist)), end(hist));
    hist[6] += hist[8];
  }

  result = std::accumulate(begin(hist), end(hist), 0ll);

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 6, Part One" << std::endl;
  for (auto filename: {"test/06", "input/06"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 6, Part Two" << std::endl;
  for (auto filename: {"test/06", "input/06"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
