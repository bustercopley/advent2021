#include "precompiled.h"
#include <cmath>

auto regex =
  re::regex(R"(^(-?\d+),(-?\d+)(?:,(-?\d+))?$|^--- scanner (\d+) ---$)");

struct point {
  int x;
  int y;
  int z;
  auto operator<=>(const point &) const = default;
};

int G[24][3][3]{};
void make_G() {
  int H[6][3][3] = {
    {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
    {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}},
    {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
    {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
    {{0, -1, 0}, {0, 0, -1}, {1, 0, 0}},
    {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
  };
  int K[4][3][3] = {
    {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
    {{1, 0, 0}, {0, 0, 1}, {0, -1, 0}},
    {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
    {{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},
  };
  for (int m = 0; m != 6; ++m)
    for (int n = 0; n != 4; ++n)
      for (int i = 0; i != 3; ++i)
        for (int j = 0; j != 3; ++j)
          for (int k = 0; k != 3; ++k)
            G[4 * m + n][i][j] += K[n][i][k] * H[m][k][j];
}

void parts(std::istream &stream) {
  bool test = false;
  int expected[2];
  int result[2]{};
  vec<vec<point>> given;
  std::map<point, vec<sz>> fixed;

  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 4)) {
        given.emplace_back();
      } else if (matched(m, 3)) {
        given.back().emplace_back(            //
          string_to<ll>(match_string(m, 1)),  //
          string_to<ll>(match_string(m, 2)),  //
          string_to<ll>(match_string(m, 3))); //
      } else {
        test = true;
        expected[0] = string_to<ll>(match_string(m, 1));
        expected[1] = string_to<ll>(match_string(m, 2));
      }
    }
  }
  if (empty(given)) return;
  vec<point> scanners(size(given));

  for (const auto &p: given.front()) {
    fixed.try_emplace(p, vec<sz>{sz(0)});
  }

  std::set<sz> remaining;
  for (sz n = 1; n != size(given); ++n) {
    remaining.insert(n);
  }

  while (!empty(remaining)) {
    for (auto n: remaining) {
      for (const auto &g: G) {
        for (const auto &p: given[n]) {
          for (const auto &[q, ignored]: fixed) {
            point dp{
              g[0][0] * p.x + g[0][1] * p.y + g[0][2] * p.z - q.x,
              g[1][0] * p.x + g[1][1] * p.y + g[1][2] * p.z - q.y,
              g[2][0] * p.x + g[2][1] * p.y + g[2][2] * p.z - q.z,
            };

            // matches[scanner] = { beacon, ... };
            vec<int> matches(size(given));

            vec<point> beacons(size(given[n]));
            for (sz i = 0; i != size(given[n]); ++i) {
              const auto &r = given[n][i];
              beacons[i] = point{
                g[0][0] * r.x + g[0][1] * r.y + g[0][2] * r.z - dp.x,
                g[1][0] * r.x + g[1][1] * r.y + g[1][2] * r.z - dp.y,
                g[2][0] * r.x + g[2][1] * r.y + g[2][2] * r.z - dp.z,
              };
              if (auto iter = fixed.find(beacons[i]); iter != end(fixed)) {
                for (auto scanner: iter->second) {
                  ++matches[scanner];
                }
              }
            }

            for (sz scanner = 0; scanner != size(given); ++scanner) {
              if (matches[scanner] >= 12) {
                for (const auto &beacon: beacons) {
                  fixed[beacon].push_back(n);
                }
                remaining.erase(n);
                scanners[n] = dp;
                goto next;
              }
            }
          }
        }
      }
    }
    break;
next:;
  }

  result[0] = size(fixed);
  for (sz i = 0; i != size(scanners); ++i) {
    auto &a = scanners[i];
    for (sz j = i + 1; j != size(scanners); ++j) {
      auto &b = scanners[j];
      auto dist =
        std::abs(b.x - a.x) + std::abs(b.y - a.y) + std::abs(b.z - a.z);
      result[1] = std::max(result[1], dist);
    }
  }

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
  std::cout << "Day 19" << std::endl;
  make_G();
  for (auto filename: {"test/19", "input/19"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
