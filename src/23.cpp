#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|#[A-D]#[A-D]#[A-D]#[A-D]#)");

constexpr int infinity = std::numeric_limits<int>::max();
constexpr int multiplier[4]{1, 10, 100, 1000};

template <bool PartTwo> struct state {
  static constexpr int room_size = PartTwo ? 4 : 2;
  std::int8_t hallway[7];
  std::int8_t rooms[4][room_size];
  auto operator<=>(const state<PartTwo> &) const = default;
};

template <bool PartTwo> std::map<state<PartTwo>, int> cache;
template <bool PartTwo> void initialize_cache() {
  state<PartTwo> winning_state{};
  for (int room = 0; room != 4; ++room) {
    for (int seat = 0; seat != winning_state.room_size; ++seat) {
      winning_state.rooms[room][seat] = room + 1;
    }
  }
  cache<PartTwo>.emplace(winning_state, 0);
}

template <bool PartTwo> int play(const state<PartTwo> &old) {
  constexpr int room_size = old.room_size;
  auto [iter, emplaced] = cache<PartTwo>.try_emplace(old, infinity);
  auto &score = iter->second;
  if (emplaced) {
    // Moves from rooms to hallway
    for (int room = 0; room != 4; ++room) {
      int i = 0;
      while (i != room_size && !old.rooms[room][i]) {
        ++i;
      }
      if (i != room_size) {
        // The amphipod in seat i is nearest the door
        int j = i;
        while (j != room_size && old.rooms[room][j] - 1 == room) {
          ++j;
        }
        if (j != room_size) {
          // At least one amphipod in seat i or greater is in the wrong room
          int type = old.rooms[room][i] - 1;
          for (int k = 0; k != 7; ++k) {
            int distance = infinity;
            if (k < room + 2) {
              // Move amphipod left into hallway
              int l = k;
              while (l != room + 2 && !old.hallway[l]) {
                ++l;
              }
              if (l == room + 2) {
                // Path is clear
                distance = i + 2 * (room + 2 - k) - (k == 0);
              }
            } else {
              // Move amphipod right into hallway
              int l = room + 2;
              while (l != k + 1 && !old.hallway[l]) {
                ++l;
              }
              if (l == k + 1) {
                // Path is clear
                distance = i + 2 * (k - 1 - room) - (k == 6);
              }
            }
            if (distance != infinity) {
              auto next = old;
              next.hallway[k] = type + 1;
              next.rooms[room][i] = 0;
              auto cost = distance * multiplier[type];
              auto subscore = play(next);
              if (subscore != infinity) {
                score = std::min(score, cost + subscore);
              }
            }
          }
        }
      }
    }
    // Moves from hallway into rooms
    for (int i = 0; i != 7; ++i) {
      if (old.hallway[i]) {
        int type = old.hallway[i] - 1;
        int j = 0;
        while (j != room_size && old.rooms[type][j] == 0) {
          ++j;
        };
        if (j) {
          // Seat j - 1 is the empty seat furthest from the door
          int k = j;
          while (k != room_size && old.rooms[type][k] == type + 1) {
            ++k;
          }
          if (k == room_size) {
            // All the amphipods in this room are in the right room
            int distance = infinity;
            if (i < type + 2) {
              // Move right
              int l = i + 1;
              while (l != type + 2 && !old.hallway[l]) {
                ++l;
              }
              if (l == type + 2) {
                // Path is clear
                distance = j - 1 + 2 * (type + 2 - i) - (i == 0);
              }
            } else {
              // Move left
              int l = type + 2;
              while (l != i + 1 && !old.hallway[l]) {
                ++l;
              }
              if (l == i) {
                // Path is clear
                distance = j - 1 + 2 * (i - 1 - type) - (i == 6);
              }
            }
            if (distance != infinity) {
              auto next = old;
              next.rooms[type][j - 1] = type + 1;
              next.hallway[i] = 0;
              auto cost = distance * multiplier[type];
              auto subscore = play(next);
              if (subscore != infinity) {
                score = std::min(score, cost + subscore);
              }
            }
          }
        }
      }
    }
  }
  return score;
}

void parts(std::istream &stream) {
  bool test = false;
  ll expected[2];
  ll results[2]{};

  state<false> part_one_start{};
  state<true> part_two_start{{},
    {{0, 4, 4, 0}, {0, 3, 2, 0}, {0, 2, 1, 0}, {0, 1, 3, 0}}};

  int y = 0;
  for (ss line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        test = true;
        expected[0] = string_to<ll>(match_string(m, 1));
        expected[1] = string_to<ll>(match_string(m, 2));
      } else {
        part_one_start.rooms[0][y] = line[3] - 'A' + 1;
        part_one_start.rooms[1][y] = line[5] - 'A' + 1;
        part_one_start.rooms[2][y] = line[7] - 'A' + 1;
        part_one_start.rooms[3][y] = line[9] - 'A' + 1;
        ++y;
      }
    }
  }

  for (int room = 0; room != 4; ++room) {
    part_two_start.rooms[room][0] = part_one_start.rooms[room][0];
    part_two_start.rooms[room][3] = part_one_start.rooms[room][1];
  }

  for (int part: {1, 2}) {
    if (part == 1) {
      results[0] = play(part_one_start);
    } else {
      results[1] = play(part_two_start);
    }

    if (!test) {
      std::cout << "Part " << part << " result " << results[part - 1]
                << std::endl;
    } else {
      if (results[part - 1] != expected[part - 1]) {
        std::cout << "Part " << part << " fail, result " << results[part - 1]
                  << ", expected " << expected[part - 1] << std::endl;
      }
    }
  }
}

int main() {
  initialize_cache<false>();
  initialize_cache<true>();
  std::cout << "Day 23" << std::endl;
  for (auto filename: {"test/23", "input/23"}) {
    if (std::ifstream stream(filename); stream) { parts(stream); }
  }

  return 0;
}
