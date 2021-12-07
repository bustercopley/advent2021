#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^(\d+(?:,\d+)*)$)");
auto regex2 = re::regex(R"((\d+)(.*))");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  std::map<int, int> histogram;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        for (auto [s]: split(line, regex2)) {
          ++histogram[string_to<ll>(s)];
        }
      }
    }
  }

  auto eval = [&histogram, part](int position) {
    ll value = 0;
    for (auto [p, c]: histogram) {
      ll d = std::abs(p - position);
      value += c * (part == 1 ? d : ((d * (d + 1) / 2)));
    }
    return value;
  };

  int a = begin(histogram)->first;
  int b = rbegin(histogram)->first;
  auto av = eval(a);
  auto bv = eval(b);

  // 'Golden section' search, ruined by putting phi = 3/4
  while (b - a > 2) {
    int c = a + (b - a + 3) / 4; // ceil(mix(a, b, 1/4))
    int d = a + 3 * (b - a) / 4; // floor(mix(a, b, 3/4))
    auto cv = eval(c);
    auto dv = eval(d);
    if (cv < dv) {
      b = d;
      bv = dv;
    } else {
      a = c;
      av = cv;
    }
  }
  int c = a + 1;
  auto cv = eval(c);
  result = std::min(std::min(av, bv), cv);

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 7, Part One" << std::endl;
  for (auto filename: {"test/07", "input/07"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 7, Part Two" << std::endl;
  for (auto filename: {"test/07", "input/07"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
