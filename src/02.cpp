#include "precompiled.h"
#include "symbols.h"

auto regex = re::regex(R"(^(\d+),(\d+)|(\w+) (\d+)$)");

void parts(std::istream &stream, int part) {
  bool test = false;
  ll expected = -1;
  ll depth = 0, position = 0, aim = 0;
  enum class cmd : sz { up, down, forward };
  symbols commands;
  commands["up"];
  commands["down"];
  commands["forward"];

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected = string_to<ll>(match_string(m, part));
      } else {
        auto command = static_cast<cmd>(commands[match_string(m, 3)]);
        ll arg = string_to<ll>(match_string(m, 4));
        if (part == 1) {
          switch (command) {
          case cmd::up: depth -= arg; break;
          case cmd::down: depth += arg; break;
          case cmd::forward: position += arg; break;
          }
        } else {
          switch (command) {
          case cmd::up: aim -= arg; break;
          case cmd::down: aim += arg; break;
          case cmd::forward:
            position += arg;
            depth += arg * aim;
            break;
          }
        }
      }
    }
  }

  ll result = depth * position;

  if (!test) {
    std::cout << result << std::endl;
  } else if (result != expected) {
    std::cout << "Fail, result " << result << ", expected " << expected
              << std::endl;
  }
}

int main() {
  std::cout << "Day 2, Part One" << std::endl;
  for (auto filename: {"test/02", "input/02"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 1); }
  }

  std::cout << "Day 2, Part Two" << std::endl;
  for (auto filename: {"test/02", "input/02"}) {
    if (std::ifstream stream(filename); stream) { parts(stream, 2); }
  }

  return 0;
}
