#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^([a-z ]+)[|]([a-z ]+)$)");
auto regex2 = re::regex(R"(([a-z]+)(.*))");

const std::map<ss, int> canonical = {
  {"abcefg", 0},
  {"cf", 1},
  {"acdeg", 2},
  {"acdfg", 3},
  {"bcdf", 4},
  {"abdfg", 5},
  {"abdefg", 6},
  {"acf", 7},
  {"abcdefg", 8},
  {"abcdfg", 9},
};

const std::map<sz, int> easy = {{2, 1}, {4, 4}, {3, 7}, {7, 8}};

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
      for (auto [s]: split(output, regex2)) {
        if (auto iter = easy.find(size(s)); iter != end(easy)) { ++result; }
      }
    }
  } else {
    for (const auto &[input, output]: report) {
      ss digits[10];

      // Decode the four easy digits
      sz easy_count = 0;
      for (auto [word]: split(input, regex2)) {
        if (auto iter = easy.find(size(word)); iter != end(easy)) {
          if (empty(digits[iter->second])) {
            std::string s{word};
            std::ranges::sort(s);
            digits[iter->second] = s;
            if (++easy_count == 4) { break; }
          }
        }
      }

      assert(easy_count == 4);
      assert(!empty(digits[1]));
      assert(!empty(digits[4]));
      assert(!empty(digits[7]));
      assert(!empty(digits[8]));

      // Get a possible ordering of the segments
      std::string segments(7, '\0');
      segments[0] = digits[7][digits[7].find_first_not_of(digits[1])];
      segments[2] = digits[1][0];
      segments[5] = digits[1][1];
      for (auto c: digits[4]) {
        if (c != segments[2] && c != segments[5]) {
          segments[segments[1] ? 3 : 1] = c;
        }
      }
      for (auto c: digits[8]) {
        if (c != segments[0] && c != segments[1] && c != segments[2] &&
          c != segments[3] && c != segments[5]) {
          segments[segments[4] ? 6 : 4] = c;
        }
      }

      // Check permutations
      for (int permutation = 0; permutation != 8; ++permutation) {
        if ((permutation & 0) == 0) std::swap(segments[2], segments[5]);
        if ((permutation & 1) == 1) std::swap(segments[1], segments[3]);
        if ((permutation & 3) == 3) std::swap(segments[4], segments[6]);
        // Build cross-reference map from segment-set to digit
        std::string segments_xref(7, '\0');
        for (int index = 0; auto c: segments) {
          segments_xref[c - 'a'] = static_cast<char>('a' + index);
          ++index;
        }
        // Is this it?
        std::map<ss, int> digits_xref;
        for (auto words: {input, output}) {
          for (auto [word]: split(words, regex2)) {
            // Apply permutation to segment-set
            ss s{word};
            for (auto &c: s) {
              c = segments_xref[c - 'a'];
            }
            std::ranges::sort(s);
            if (auto iter = canonical.find(s); iter != end(canonical)) {
              digits_xref[ss{word}] = iter->second;
            } else {
              goto next_permutation;
            }
          }
        }
        for (int place_value = 1000; auto [word]: split(output, regex2)) {
          result += place_value * digits_xref[ss{word}];
          place_value /= 10;
        }
        goto next_input_line;
next_permutation:;
      }
      assert(false);
next_input_line:;
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
