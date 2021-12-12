#include "precompiled.h"
#include "symbols.h"
#include "y-combinator.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^(\w+)-(\w+)$)");

struct cave {
  bool big;
  vec<sz> connections;
};

void parts(std::istream &stream, int part) {
  bool test = false;
  ll result = 0;
  ll expected = -1;
  symbols names;
  vec<cave> caves{{false, {}}, {false, {}}};
  names["start"];
  names["end"];

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        auto a = names[match_string(m, 3)];
        if (a == size(caves)) {
          char c = names[a][0];
          caves.push_back({'A' <= c && c <= 'Z', {}});
        }
        auto b = names[match_string(m, 4)];
        if (b == size(caves)) {
          char c = names[b][0];
          caves.push_back({'A' <= c && c <= 'Z', {}});
        }
        if (b) caves[a].connections.push_back(b);
        if (a) caves[b].connections.push_back(a);
      }
    }
  }

  y_combinator explore{
    [&caves](auto explore, sz position, vec<int> done, int extra_time) -> ll {
      ll result = 0;
      if (position == 1) {
        ++result;
      } else {
        if (!caves[position].big) {
          if (++done[position] > 1) { --extra_time; }
        }
        for (auto next: caves[position].connections) {
          if (caves[next].big || !done[next] || extra_time) {
            result += explore(next, done, extra_time);
          }
        }
      }
      return result;
    }};

  result = explore(0, vec<int>(size(caves)), part == 2);

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 12, Part One" << std::endl;
  for (auto filename: {"test/12", "input/12"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 12, Part Two" << std::endl;
  for (auto filename: {"test/12", "input/12"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
