#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^([][(){}<>]*)$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  vec<ll> scores;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        ss stack;
        char error = '\0';
        for (auto c: line) {
          switch (c) {
          case '(': stack += ')'; break;
          case '[': stack += ']'; break;
          case '{': stack += '}'; break;
          case '<': stack += '>'; break;
          default:
            if (c == stack.back()) {
              stack.pop_back();
            } else {
              error = c;
            }
            break;
          }
          if (error) { break; }
        }
        if (error) {
          if (part == 1) {
            switch (error) {
            case ')': result += 3; break;
            case ']': result += 57; break;
            case '}': result += 1197; break;
            case '>': result += 25137; break;
            }
          }
        } else if (part == 2) {
          ll score = 0;
          for (char c: std::ranges::views::reverse(stack)) {
            score *= 5;
            switch (c) {
            case ')': score += 1; break;
            case ']': score += 2; break;
            case '}': score += 3; break;
            case '>': score += 4; break;
            }
          }
          scores.push_back(score);
        }
      }
    }
  }

  if (part == 2) {
    std::ranges::sort(scores);
    result = scores[size(scores) / 2];
  }

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 10, Part One" << std::endl;
  for (auto filename: {"test/10", "input/10"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 10, Part Two" << std::endl;
  for (auto filename: {"test/10", "input/10"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
