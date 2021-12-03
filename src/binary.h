// -*- C++ -*-
#ifndef binary_h
#define binary_h

std::string to_binary(ll x, sz bits) {
  std::string result;
  ll j = 1 << (bits - 1);
  for (sz i = 0; i != bits; ++i) {
    result += ('0' + ((x & j) != 0));
    j >>= 1;
  }
  return result;
}

ll from_binary(ss bits) {
  ll result = 0;
  ll j = 1 << (size(bits) - 1);
  for (auto c: bits) {
    result += j * (c == '1');
    j >>= 1;
  }
  return result;
}

#endif
