// -*- C++ -*-
#ifndef binary_h
#define binary_h

std::string to_binary(auto value, sz width) {
  std::string result;
  for (sz i = 0; i != width; ++i) {
    result = static_cast<char>('0' + (value & 1)) + result;
    value >>= 1;
  }
  return result;
}

ll from_binary(ss bits) {
  ll result = 0;
  for (auto c: bits) {
    result = (result << 1) | (c == '1');
  }
  return result;
}

#endif
