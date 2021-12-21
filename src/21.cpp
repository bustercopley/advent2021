#include "binary.h"
#include "precompiled.h"
#include <cmath>
#include <cstdio>

auto regex = re::regex(R"(^(\d+),(\d+)$|: (\d+)$)");

void parts(std::istream &stream) {
  bool test = false;
  ll expected[2];
  ll results[2]{};

  int player = 0;
  int starting_positions[2]{};
  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected[0] = string_to<ll>(match_string(m, 1));
        expected[1] = string_to<ll>(match_string(m, 2));
      } else {
        starting_positions[player++] = string_to<ll>(match_string(m, 3)) - 1;
      }
    }
  }

  // Part One
  ll total{};
  ll scores[2]{};
  ll die{};
  ll positions[2]{starting_positions[0], starting_positions[1]};
  player = 0;
  while (scores[player ^ 1] < 1000) {
    total += 3 * die + 6;
    positions[player] = (positions[player] + 3 * die + 6) % 10;
    scores[player] += positions[player] + 1;
    die += 3;
    player = player ^ 1;
  }
  results[0] = die * scores[player];

  // Part Two
  static constexpr int p[]{1, 3, 6, 7, 6, 3, 1};
  ll matrix[22][10][22][10]{}; // matrix[score][posn][score][posn] = count
  matrix[0][starting_positions[0]][0][starting_positions[1]] = 1;
  bool keep_going;
  do {
    keep_going = false;
    ll oldmatrix[22][10][22][10];
    std::memcpy(&oldmatrix, &matrix, sizeof matrix);
    std::memset(&matrix, '\0', sizeof matrix);
    for (int score1 = 0; score1 != 22; ++score1) {
      for (int p1 = 0; p1 != 10; ++p1) {
        for (int score2 = 0; score2 != 22; ++score2) {
          for (int p2 = 0; p2 != 10; ++p2) {
            ll count = oldmatrix[score1][p1][score2][p2];
            if (score1 == 21 || score2 == 21) {
              matrix[score1][p1][score2][p2] += count;
            } else if (count) {
              keep_going = true;
              for (int roll = 0; roll != 7; ++roll) {
                int np1 = (p1 + roll + 3) % 10;
                int nscore1 = std::min(21, score1 + np1 + 1);
                matrix[nscore1][np1][score2][p2] += p[roll] * count;
              }
            }
          }
        }
      }
    }
    std::memcpy(&oldmatrix, &matrix, sizeof matrix);
    std::memset(&matrix, '\0', sizeof matrix);
    for (int score1 = 0; score1 != 22; ++score1) {
      for (int p1 = 0; p1 != 10; ++p1) {
        for (int score2 = 0; score2 != 22; ++score2) {
          for (int p2 = 0; p2 != 10; ++p2) {
            ll count = oldmatrix[score1][p1][score2][p2];
            if (score1 == 21 || score2 == 21) {
              matrix[score1][p1][score2][p2] += count;
            } else if (count) {
              keep_going = true;
              for (int roll = 0; roll != 7; ++roll) {
                int np2 = (p2 + roll + 3) % 10;
                int nscore2 = std::min(21, score2 + np2 + 1);
                matrix[score1][p1][nscore2][np2] += p[roll] * count;
              }
            }
          }
        }
      }
    }
  } while (keep_going);
  ll counts[2]{};
  for (int p1 = 0; p1 != 10; ++p1) {
    for (int score2 = 0; score2 != 21; ++score2) {
      for (int p2 = 0; p2 != 10; ++p2) {
        counts[0] += matrix[21][p1][score2][p2];
      }
    }
  }
  for (int score1 = 0; score1 != 21; ++score1) {
    for (int p1 = 0; p1 != 10; ++p1) {
      for (int p2 = 0; p2 != 10; ++p2) {
        counts[1] += matrix[score1][p1][21][p2];
      }
    }
  }
  results[1] = std::max(counts[0], counts[1]);

  if (!test) {
    std::cout << "Part 1 result " << results[0] << "\nPart 2 result "
              << results[1] << std::endl;
  } else {
    for (int part: {1, 2}) {
      if (results[part - 1] != expected[part - 1]) {
        std::cout << "Part " << part << " fail, result " << results[part - 1]
                  << ", expected " << expected[part - 1] << std::endl;
      }
    }
  }
}

int main() {
  std::cout << "Day 21" << std::endl;
  for (auto filename: {"test/21", "input/21"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
