#include "precompiled.h"
#include "y-combinator.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^[][0-9,]+$)");
using couple = std::array<int, 2>;

constexpr sz undef = std::numeric_limits<sz>::max();

struct node {
  bool terminal;
  int value;
  sz left;
  sz right;
};

void parts(std::istream &stream) {
  bool test = false;
  ll expected[2]{};

  vec<node> nodes;

  y_combinator read{[&nodes](auto read, auto &range) -> sz {
    if (range[0] == '[') {
      range.advance(1);
      sz left = read(range);
      assert(range[0] == ',');
      range.advance(1);
      sz right = read(range);
      assert(range[0] == ']');
      range.advance(1);
      sz n = size(nodes);
      nodes.emplace_back(false, 0, left, right);
      return n;
    }
    if ('0' <= range[0] && range[0] <= '9') {
      int value = range[0] - '0';
      range.advance(1);
      sz n = size(nodes);
      nodes.emplace_back(true, value, undef, undef);
      return n;
    }
    assert(false);
  }};

  auto reduce = [&nodes](sz root) {
    y_combinator find_if{
      [&nodes](auto find_if, sz iter, auto f, int level = 0) -> sz {
        if (f(iter, level)) return iter;
        if (nodes[iter].terminal) return undef;
        if (auto p = find_if(nodes[iter].left, f, level + 1); p != undef) {
          return p;
        }
        return find_if(nodes[iter].right, f, level + 1);
      }};

    while (true) {
      sz prev = undef;
      sz exploder = undef;

      if (auto next = find_if(root,
            [&nodes, &exploder, &prev](sz iter, int level) {
              if (exploder == undef && level == 4 && !nodes[iter].terminal) {
                assert(nodes[nodes[iter].left].terminal);
                assert(nodes[nodes[iter].right].terminal);
                exploder = iter;
                return false;
              }
              if (exploder != undef) {
                return iter != nodes[exploder].left &&
                  iter != nodes[exploder].right && nodes[iter].terminal;
              }
              if (nodes[iter].terminal) { prev = iter; }
              return false;
            });
          exploder != undef) {
        if (prev != undef) {
          nodes[prev].value += nodes[nodes[exploder].left].value;
        }
        if (next != undef) {
          nodes[next].value += nodes[nodes[exploder].right].value;
        }
        nodes[exploder].terminal = true;
        nodes[exploder].value = 0;
      } else if (auto splitter = find_if(root,
                   [&nodes](sz iter, int) {
                     return nodes[iter].terminal && nodes[iter].value >= 10;
                   });
                 splitter != undef) {
        auto value = nodes[splitter].value;
        nodes[splitter].terminal = false;
        nodes[splitter].value = 0;
        nodes[splitter].left = size(nodes);
        nodes.emplace_back(true, value / 2, undef, undef);
        nodes[splitter].right = size(nodes);
        nodes.emplace_back(true, (value + 1) / 2, undef, undef);
      } else {
        break;
      }
    }
  };

  y_combinator copy{[&nodes](auto copy, sz index) -> sz {
    sz result = size(nodes);
    nodes.push_back(nodes[index]);
    if (!nodes[result].terminal) {
      nodes[result].left = copy(nodes[result].left);
      nodes[result].right = copy(nodes[result].right);
    }
    return result;
  }};

  auto add = [&nodes, copy, reduce](sz a, sz b) {
    sz result = size(nodes);
    nodes.emplace_back(false, 0, undef, undef);
    nodes[result].left = copy(a);
    nodes[result].right = copy(b);
    reduce(result);
    return result;
  };

  vec<sz> numbers;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected[0] = string_to<ll>(match_string(m, 1));
        expected[1] = string_to<ll>(match_string(m, 2));
      } else {
        auto range = std::ranges::subrange(line);
        numbers.push_back(read(range));
      }
    }
  }

  y_combinator magnitude{[&nodes](auto magnitude, sz iter) -> ll {
    if (nodes[iter].terminal) { return nodes[iter].value; }
    return 3 * magnitude(nodes[iter].left) + 2 * magnitude(nodes[iter].right);
  }};

  assert(!empty(numbers));

  sz accumulator = undef;
  for (auto number: numbers) {
    if (accumulator == undef) {
      accumulator = number;
    } else {
      accumulator = add(accumulator, number);
    }
  }

  ll part2 = std::numeric_limits<ll>::min();
  for (auto a: numbers) {
    for (auto b: numbers) {
      if (a != b) { part2 = std::max(part2, magnitude(add(a, b))); }
    }
  }

  ll result[2] = {magnitude(accumulator), part2};

  if (!test) {
    std::cout << "Part 1 result " << result[0] << "\nPart 2 result "
              << result[1] << std::endl;
  } else {
    for (int part: {1, 2}) {
      if (result[part - 1] != expected[part - 1]) {
        std::cout << "Part " << part << " fail, result " << result[part - 1]
                  << ", expected " << expected[part - 1] << std::endl;
      }
    }
  }
}

int main() {
  std::cout << "Day 18" << std::endl;
  for (auto filename: {"test/18", "input/18"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
