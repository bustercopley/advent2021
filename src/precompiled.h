// -*- C++ -*-
#ifndef precompiled_h
#define precompiled_h

#if _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#define WIN32_LEAN_AND_MEAN 1

#include <sdkddkver.h>
#include <tchar.h>
#include <windows.h>
#endif

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <array>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <numbers>
#include <numeric>
#include <ostream>
#include <ranges>
#include <regex>
#include <set>
#include <vector>

#include "re.h"
#include "split.h"
#include "string_to.h"

using sz = std::size_t;
using ul = std::uint32_t;
using ll = std::int64_t;
using ull = std::uint64_t;
using ss = std::string;
template <typename T> using vec = std::vector<T>;
template <typename T, sz N> using ary = std::array<T, N>;

#endif
