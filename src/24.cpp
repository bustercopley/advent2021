#include "precompiled.h"
#include "symbols.h"

auto regex = re::regex(R"(^(\w+) ([w-z])(?: (?:([w-z])|(-?\d+)))?$)");

using registers = std::array<int, 5>; // w, x, y, z, pc
std::map<registers, std::pair<bool, ss>> cache;

symbols opnames;
enum class opcode { inp = 0, add = 1, mul = 2, div = 3, mod = 4, eql = 5 };

struct insn {
  opcode op;
  int a;
  int b;
  bool immediate;
};
vec<insn> program;

std::pair<bool, ss> execute(registers xs, int part) {
  auto [iter, emplaced] = cache.try_emplace(xs);
  auto &result = iter->second;
  if (emplaced) {
bypass_cache:
    int pc = xs[4]++;
    if (pc == (int)size(program)) {
      result = {xs[3] == 0, ""};
    } else {
      auto &a = xs[program[pc].a];
      if (program[pc].op == opcode::inp) {
        for (int i = 0; i != 9; ++i) {
          a = part == 1 ? 9 - i : i + 1;
          char first = '0' + a;
          if (auto [ok, rest] = execute(xs, part); ok) {
            result = {true, first + rest};
            break;
          }
        }
      } else {
        int b = program[pc].b;
        if (!program[pc].immediate) { b = xs[b]; }
        switch (program[pc].op) {
        case opcode::add: a += b; break;
        case opcode::mul: a *= b; break;
        case opcode::div: a /= b; break;
        case opcode::mod: a %= b; break;
        case opcode::eql: a = (a == b); break;
        default: std::abort();
        }
        goto bypass_cache;
      }
    }
  }
  return result;
}

void parts(int part) {
  registers xs{};
  if (auto [ok, result] = execute(xs, part); ok) {
    std::cout << "Part " << part << ": " << result << std::endl;
  }
  // Clearing the cache is hard work, so just throw it on the sidewalk.
  (new std::map<registers, std::pair<bool, ss>>)->swap(cache);
}

int main() {
  opnames["inp"];
  opnames["add"];
  opnames["mul"];
  opnames["div"];
  opnames["mod"];
  opnames["eql"];
  if (std::ifstream stream("input/24"); stream) {
    for (ss line; std::getline(stream, line);) {
      if (auto m = match(regex, line)) {
        auto op = static_cast<opcode>(opnames[match_string(m, 1)]);
        int a = match_string(m, 2)[0] - 'w';
        bool immediate = matched(m, 4);
        int b;
        if (op == opcode::inp) {
          b = 4;
        } else if (immediate) {
          b = string_to<int>(match_string(m, 4));
        } else {
          b = match_string(m, 3)[0] - 'w';
        }
        program.emplace_back(op, a, b, immediate);
      }
    }
  }
  if (empty(program)) {
    std::cout << "Empty program\n";
  } else {
    parts(1);
    parts(2);
  }
  return 0;
}
