#include "precompiled.h"

auto regex = re::regex(R"(^(\d+),(\d+)$|^[][0-9,]+$)");

struct node {
  using ptr = std::unique_ptr<node>;
  ll value;
  ptr children[2];
  node(ll value): value(value) {}
  node(ptr &&left, ptr &&right): value(-1) {
    children[0] = move(left);
    children[1] = move(right);
  }
  bool is_leaf() { return value != -1; }
};

node::ptr make_node(int value) { return std::make_unique<node>(value); }

node::ptr make_node(node::ptr &&left, node::ptr &&right) {
  return std::make_unique<node>(move(left), move(right));
}

node::ptr read(const char *&p) {
  if ('0' <= *p && *p <= '9') { return make_node(*p++ - '0'); }
  node::ptr left = read(++p);
  node::ptr right = read(++p);
  ++p;
  return make_node(move(left), move(right));
}

node *find_if(const node::ptr &p, auto f, int level = 0) {
  if (f(p.get(), level)) { return p.get(); }
  if (!p->is_leaf()) {
    for (auto &child: p->children) {
      if (auto p = find_if(child, f, level + 1); p) return p;
    }
  }
  return nullptr;
}

node::ptr copy(const node::ptr &p) {
  if (!p->is_leaf()) {
    return make_node(copy(p->children[0]), copy(p->children[1]));
  } else {
    return make_node(p->value);
  }
}

void reduce(const node::ptr &p) {
  while (true) {
    node *prev = nullptr;
    node *exploder = nullptr;
    if (node *next = find_if(p,
          [&prev, &exploder](node *p, int level) {
            if (!exploder && level == 4 && !p->is_leaf()) {
              exploder = p;
              return false;
            }
            if (exploder) {
              return p != exploder->children[0].get() &&
                p != exploder->children[1].get() && p->is_leaf();
            }
            if (p->is_leaf()) { prev = p; }
            return false;
          });
        exploder) {
      if (prev) prev->value += exploder->children[0]->value;
      if (next) next->value += exploder->children[1]->value;
      exploder->value = 0;
      exploder->children[0] = nullptr;
      exploder->children[1] = nullptr;
    } else if (node *splitter =
                 find_if(p, [](node *p, int) { return p->value >= 10; })) {
      splitter->children[0] = make_node(splitter->value / 2);
      splitter->children[1] = make_node((splitter->value + 1) / 2);
      splitter->value = -1;
    } else {
      break;
    }
  }
}

node::ptr add(const node::ptr &a, const node::ptr &b) {
  if (!a.get()) {
    return copy(b);
  } else {
    node::ptr result = make_node(copy(a), copy(b));
    reduce(result);
    return result;
  }
}

ll magnitude(const node::ptr &p) {
  if (p->is_leaf()) {
    return p->value;
  } else {
    return 3 * magnitude(p->children[0]) + 2 * magnitude(p->children[1]);
  }
}

std::ostream &operator<<(std::ostream &stream, const node::ptr &p) {
  if (p->is_leaf()) {
    return stream << p->value;
  } else {
    return stream << "[" << p->children[0] << "," << p->children[1] << "]";
  }
}

int main() {
  std::cout << "Day 18" << std::endl;

  for (auto filename: {"test/18", "input/18"}) {
    if (std::ifstream stream(filename); stream) {
      bool test = false;
      ll expected[2];
      ll results[2]{0, 0};
      vec<node::ptr> numbers;

      for (ss line; std::getline(stream, line);) {
        if (auto m = match(regex, line)) {
          if (matched(m, 1)) {
            test = true;
            expected[0] = string_to<ll>(match_string(m, 1));
            expected[1] = string_to<ll>(match_string(m, 2));
          } else {
            const char *p = data(line);
            numbers.push_back(read(p));
          }
        }
      }

      node::ptr accumulator = nullptr;
      for (auto &n: numbers) {
        accumulator = add(accumulator, n);
      }
      results[0] = magnitude(accumulator);

      for (auto &a: numbers) {
        for (auto &b: numbers) {
          if (a != b) {
            results[1] = std::max(results[1], magnitude(add(a, b)));
          }
        }
      }

      if (test) {
        for (int part: {1, 2}) {
          if (results[part - 1] != expected[part - 1]) {
            std::cout << "Part " << part << " fail, result "
                      << results[part - 1] << ", expected "
                      << expected[part - 1] << std::endl;
          }
        }
      } else {
        std::cout << "Part 1 result " << results[0] << "\nPart 2 result "
                  << results[1] << std::endl;
      }
    }
  }
  return 0;
}
