#include "precompiled.h"

auto regex = re::regex(
  R"(^(\d+),(\d+)$|)"
  R"(^o[nf](f)? x=(-?\d+)\.+(-?\d+),y=(-?\d+)\.+(-?\d+),z=(-?\d+)\.+(-?\d+)$)");

struct cube {
  int x0, x1, y0, y1, z0, z1, state;
};

void parts(std::istream &stream) {
  bool test = false;
  ll expected[2];
  ll results[2]{};
  std::vector<cube> cubes;
  std::vector<int> pss[3];
  auto &[xs, ys, zs] = pss;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected[0] = string_to<ll>(match_string(m, 1));
        expected[1] = string_to<ll>(match_string(m, 2));
      } else {
        cubes.emplace_back(                      //
          string_to<ll>(match_string(m, 4)),     //
          string_to<ll>(match_string(m, 5)) + 1, //
          string_to<ll>(match_string(m, 6)),     //
          string_to<ll>(match_string(m, 7)) + 1, //
          string_to<ll>(match_string(m, 8)),     //
          string_to<ll>(match_string(m, 9)) + 1, //
          !matched(m, 3));
      }
    }
  }
  if (empty(cubes)) {
    std::cout << "No cubes" << std::endl;
    return;
  }

  for (int part: {1, 2}) {
    if (test && !expected[part - 1]) { continue; }
    for (const auto &cube: cubes) {
      if ((part == 2) || (-50 <= cube.x0 && cube.x0 <= 50)) {
        xs.push_back(cube.x0);
        xs.push_back(cube.x1);
        ys.push_back(cube.y0);
        ys.push_back(cube.y1);
        zs.push_back(cube.z0);
        zs.push_back(cube.z1);
      }
    }

    for (auto &ps: pss) {
      std::ranges::sort(ps);
      auto [begin, end] = std::ranges::unique(ps);
      ps.erase(begin, end);
    }

    vec<int> states((size(zs) - 1) * (size(ys) - 1) * (size(xs) - 1));
    auto index = [&states, &xs, &ys, &zs](int a, int b, int c) {
      auto i = a + (size(xs) - 1) * b + (size(xs) - 1) * (size(ys) - 1) * c;
      assert(i < size(states));
      return i;
    };

    for (const auto &cube: cubes) {
      if ((part == 2) || (-50 <= cube.x0 && cube.x0 <= 50)) {
        auto a0 = std::ranges::lower_bound(xs, cube.x0) - begin(xs);
        auto a1 = std::ranges::lower_bound(xs, cube.x1) - begin(xs);
        auto b0 = std::ranges::lower_bound(ys, cube.y0) - begin(ys);
        auto b1 = std::ranges::lower_bound(ys, cube.y1) - begin(ys);
        auto c0 = std::ranges::lower_bound(zs, cube.z0) - begin(zs);
        auto c1 = std::ranges::lower_bound(zs, cube.z1) - begin(zs);
        for (auto a = a0; a != a1; ++a) {
          for (auto b = b0; b != b1; ++b) {
            for (auto c = c0; c != c1; ++c) {
              states[index(a, b, c)] = cube.state;
            }
          }
        }
      }
    }

    for (sz a = 0; a != size(xs) - 1; ++a) {
      for (sz b = 0; b != size(ys) - 1; ++b) {
        for (sz c = 0; c != size(zs) - 1; ++c) {
          if (states[index(a, b, c)]) {
            ll x = xs[a + 1] - xs[a];
            ll y = ys[b + 1] - ys[b];
            ll z = zs[c + 1] - zs[c];
            results[part - 1] += x * y * z;
          }
        }
      }
    }
    if (!test) {
      std::cout << "Part " << part << " result " << results[part - 1]
                << std::endl;
    } else if (results[part - 1] != expected[part - 1]) {
      std::cout << "Part " << part << " fail, result " << results[part - 1]
                << ", expected " << expected[part - 1] << std::endl;
    }
  }
}

int main() {
  std::cout << "Day 22" << std::endl;
  for (auto filename: {"test/22", "test/22b", "input/22"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
