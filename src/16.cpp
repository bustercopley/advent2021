#include "precompiled.h"
#include "y-combinator.h"

auto regex = re::regex(R"(^([0-9A-F]+)(?:,(,)?(\d+))?$)");

ll from_binary(auto bits) {
  ll result = 0;
  for (auto c: bits) {
    result = (result << 1) | (c == '1');
  }
  return result;
}

ll from_binary(auto begin, auto end) {
  return from_binary(std::ranges::subrange(begin, end));
}

void parts(std::istream &stream) {
  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      bool test = false;
      bool part = 0;
      ll expected = -1;
      ll result1 = 0;
      ss bits;
      if (matched(m, 3)) {
        test = true;
        part = matched(m, 2);
        expected = string_to<ll>(match_string(m, 3));
      }

      for (auto c: match_string(m, 1)) {
        int n = ('0' <= c && c <= '9') ? (c - '0') : 10 + (c - 'A');
        for (int i = 0; i != 4; ++i) {
          bits += static_cast<char>('0' + ((n >> 3) & 1));
          n <<= 1;
        }
      }

      y_combinator parse{[&](auto parse, char *begin, char *end = nullptr,
                           int count, int type) -> std::pair<ll, char *> {
        ll value;
        switch (type) {
        case 0: value = 0; break;
        case 1: value = 1; break;
        case 2: value = std::numeric_limits<ll>::max(); break;
        case 3: value = std::numeric_limits<ll>::min(); break;
        case 5: value = -1; break;
        case 6: value = -1; break;
        case 7: value = -1; break;
        }

        auto get = [&begin](int bits) -> int {
          int result = from_binary(begin, begin + bits);
          begin += bits;
          return result;
        };

        int n = 0;
        while (end ? begin != end : n != count) {
          int version = get(3);
          int subtype = get(3);
          result1 += version;
          ll subvalue;
          if (subtype == 4) {
            subvalue = 0;
            while (true) {
              int number = get(5);
              subvalue = (subvalue << 4) | (number & 15);
              if (!(number & 16)) break;
            }
          } else {
            int length_type = get(1);
            if (!length_type) {
              int length = get(15);
              auto [value, end] = parse(begin, begin + length, 0, subtype);
              assert(end = begin + length);
              begin = end;
              subvalue = value;
            } else {
              int count = get(11);
              auto [value, end] = parse(begin, nullptr, count, subtype);
              begin = end;
              subvalue = value;
            }
          }
          ++n;
          switch (type) {
          case 0: value += subvalue; break;
          case 1: value *= subvalue; break;
          case 2: value = std::min(value, subvalue); break;
          case 3: value = std::max(value, subvalue); break;
          case 5: value = (n == 1 ? subvalue : value > subvalue); break;
          case 6: value = (n == 1 ? subvalue : value < subvalue); break;
          case 7: value = (n == 1 ? subvalue : value == subvalue); break;
          }
        }
        return {value, begin};
      }};

      auto [result2, end] = parse(data(bits), nullptr, 1, 0);

      if (!test) {
        std::cout << "Part 1 result: " << result1 << std::endl;
        std::cout << "Part 2 result: " << result2 << std::endl;
      } else if (part == 0) {
        if (result1 != expected) {
          std::cout << line << "\nPart 1 fail, result " << result1
                    << ", expected " << expected << std::endl;
        }
      } else {
        if (result2 != expected) {
          std::cout << line << "\nPart 2 fail, result " << result2
                    << ", expected " << expected << std::endl;
        }
      }
    }
  }
}

int main() {
  std::cout << "Day 16" << std::endl;
  for (auto filename: {"test/16", "input/16"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
