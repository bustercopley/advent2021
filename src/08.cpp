#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^([a-z ]+)[|]([a-z ]+)$)");
auto regex2 = re::regex(R"(([a-z]+)(.*))");

bool contains(auto range, auto element) {
  return std::ranges::find(range, element) != std::end(range);
}

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;

  vec<std::pair<ss, ss>> report;
  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        ss input = match_string(m, 3);
        ss output = match_string(m, 4);
        report.emplace_back(input, output);
      }
    }
  }

  if (part == 1) {
    for (const auto &[input_ignored, output]: report) {
      for (auto [digit]: split(output, regex2)) {
        switch (size(digit)) {
        case 2:
        case 3:
        case 4:
        case 7: ++result;
        }
      }
    }
  } else {
    for (const auto &[input, output]: report) {
      std::map<ss, int> decode;
      std::vector<ss> digits;
      int counts[7]{};
      char b{}, c{}, e{}, f{};
      ss s1, s2;

      for (auto [digit]: split(input, regex2)) {
        for (auto segment: digit) {
          ++counts[segment - 'a'];
        }
        ss canonical{digit};
        std::ranges::sort(canonical);
        digits.push_back(canonical);
        switch (size(digit)) {
        case 2: decode[s1 = canonical] = 1; break;
        case 3: decode[canonical] = 7; break;
        case 4: decode[canonical] = 4; break;
        case 7: decode[canonical] = 8; break;
        }
      }

      // e is the only segment contained in 4 digits
      // f is the only segment contained in 9 digits
      for (auto segment: ss{"abcdefg"}) {
        switch (counts[segment - 'a']) {
        case 4: e = segment; break;
        case 9: f = segment; break;
        }
      }

      // 2 is the only digit not containing segment f
      for (auto digit: digits) {
        if (!contains(digit, f)) { decode[s2 = digit] = 2; }
      }

      // b is the only segment not contained in 2 except for f
      // c is the only segment contained in 1 except for f
      for (auto segment: ss{"abcdefg"}) {
        if (segment != f) {
          if (!contains(s2, segment)) { b = segment; }
          if (contains(s1, segment)) { c = segment; }
        }
      }

      // 5 is the only 5-segment digit containing b
      // 3 is the only 5-segment digit not containing b except for 2
      // 6 is the only 6-segment digit not containing c
      // 9 is the only 6-segment digit not containing e
      // 0 is the last remaining digit
      for (auto digit: digits) {
        if (size(digit) == 5) {
          if (contains(digit, b)) {
            decode[digit] = 5;
          } else if (digit != s2) {
            decode[digit] = 3;
          }
        } else if (size(digit) == 6) {
          if (!contains(digit, c)) {
            decode[digit] = 6;
          } else if (!contains(digit, e)) {
            decode[digit] = 9;
          } else {
            decode[digit] = 0;
          }
        }
      }

      int value = 0;
      for (auto [digit]: split(output, regex2)) {
        ss canonical{digit};
        std::ranges::sort(canonical);
        value = 10 * value + decode[canonical];
      }
      result += value;
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
  std::cout << "Day 8, Part One" << std::endl;
  for (auto filename: {"test/08", "input/08"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 8, Part Two" << std::endl;
  for (auto filename: {"test/08", "input/08"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
