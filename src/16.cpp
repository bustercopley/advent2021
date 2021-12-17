#include "precompiled.h"

auto regex = re::regex(R"(^([0-9A-F]+)(?:,(,)?(\d+))?$)");

enum class op { add = 0, mul, min, max, literal, gt, lt, eq };

using subrange = std::ranges::subrange<ss::iterator>;

std::tuple<ll, int> eval(subrange &bits);
std::tuple<ll, int> apply(op opcode, subrange &bits, int count);

ll consume(subrange &bits, int bitcount) {
  ll result = 0;
  for (int i = 0; bits && i != bitcount; ++i) {
    result = (result << 1) | (bits[i] == '1');
  }
  bits.advance(bitcount);
  return result;
}

// Read and evaluate one expression; return the value and version sum
std::tuple<ll, int> eval(subrange &bits) {
  auto version = consume(bits, 3);
  auto opcode = static_cast<op>(consume(bits, 3));

  if (opcode == op::literal) {
    // Read literal
    ll value = 0;
    while (true) {
      int number = consume(bits, 5);
      value = (value << 4) | (number & 15);
      if (!(number & 16)) break;
    }
    return {value, version};
  } else {
    // Read argument list length
    auto args_end = std::end(bits);
    int args_count = -1;
    if (consume(bits, 1)) {
      args_count = consume(bits, 11);
    } else {
      int size = consume(bits, 15);
      args_end = std::begin(bits) + size;
    }
    // Evaluate arguments and apply operation
    subrange args_bits{std::begin(bits), args_end};
    auto [value, args_version_sum] = apply(opcode, args_bits, args_count);
    bits = subrange{std::begin(args_bits), std::end(bits)};
    return {value, version + args_version_sum};
  }
}

// Read argument list and return operation value and version sum
std::tuple<ll, int> apply(op opcode, subrange &bits, int count) {
  // Initialize operation result value
  ll value{};
  ll version_sum{};
  switch (opcode) {
  case op::add: value = 0; break;
  case op::mul: value = 1; break;
  case op::min: value = std::numeric_limits<ll>::max(); break;
  case op::max: value = std::numeric_limits<ll>::min(); break;
  default:;
  }

  for (int n = 0; bits && n != count; ++n) {
    auto [arg_value, arg_version_sum] = eval(bits);
    // Accumulate operation result value
    switch (opcode) {
    case op::add: value += arg_value; break;
    case op::mul: value *= arg_value; break;
    case op::min: value = std::min(value, arg_value); break;
    case op::max: value = std::max(value, arg_value); break;
    case op::literal: value = arg_value; break;
    case op::gt: value = (n == 0 ? arg_value : value > arg_value); break;
    case op::lt: value = (n == 0 ? arg_value : value < arg_value); break;
    case op::eq: value = (n == 0 ? arg_value : value == arg_value); break;
    }
    version_sum += arg_version_sum;
  }
  return {value, version_sum};
}

int main() {
  std::cout << "Day 16" << std::endl;
  for (auto filename: {"test/16", "input/16"}) {
    if (std::ifstream stream(filename); stream) {
      for (ss line; std::getline(stream, line);) {
        if (auto m = match(regex, line)) {
          bool test = false;
          bool part = 0;
          ll expected = -1;
          ss bits;
          if (matched(m, 3)) {
            test = true;
            part = matched(m, 2);
            expected = string_to<ll>(match_string(m, 3));
          }

          for (auto c: match_string(m, 1)) {
            int n = ('0' <= c && c <= '9') ? (c - '0') : 10 + (c - 'A');
            for (int i = 0; i != 4; ++i) {
              bits += static_cast<char>('0' + ((n >> (3 - i)) & 1));
            }
          }

          subrange input(std::begin(bits), std::end(bits));
          auto [value, version_sum] = eval(input);

          if (!test) {
            std::cout << "Part 1 result: " << version_sum << std::endl;
            std::cout << "Part 2 result: " << value << std::endl;
          } else if (part == 0) {
            if (version_sum != expected) {
              std::cout << line << "\nPart 1 fail, result " << version_sum
                        << ", expected " << expected << std::endl;
            }
          } else {
            if (value != expected) {
              std::cout << line << "\nPart 2 fail, result " << value
                        << ", expected " << expected << std::endl;
            }
          }
        }
      }
    }
  }

  return 0;
}
