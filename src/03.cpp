#include "precompiled.h"
#include "symbols.h"
#include "binary.h"

auto regex = re::regex(R"(^(\d+),(\d+)|(\d+)$)");

void parts(std::istream &stream, int part) {
  bool test = false;
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

  ll result;

  if (part == 1) {
    vec<ll> freq;
    ll count = 0;
    for (auto bits: report) {
      freq.resize(size(bits));
      ++count;
      for (int i = 0; char c: bits) {
        freq[i] += (c == '1');
        ++i;
      }
    }
    ll gamma = 0, epsilon = 0;
    for (int i = 1 << (size(freq) - 1); auto n: freq) {
      gamma |= i * (2 * n > count);
      epsilon |= i * (2 * n < count);
      i >>= 1;
    }

    result = gamma * epsilon;
  } else {
    std::sort(begin(report), end(report));
    result = 1;
    for (int i = 0; i != 2; ++i) {
      auto b = begin(report);
      auto e = end(report);
      ss prefix;
      while (e - b > 1) {
        auto mid = std::find_if(b, e,
          [index = size(prefix)](auto item) { return item[index] == '1'; });
        int bit = i ^ (mid - b <= e - mid);
        prefix += ('0' + bit);
        (bit ? b : e) = mid;
      }
      result *= from_binary(*b);
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
