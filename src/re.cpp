#include "re.h"
#include "precompiled.h"

#define SRCLOC const std::experimental::source_location &location

std::string format_error(int error_number) {
  std::string result(256, '\0');
  result.resize(pcre2_get_error_message(error_number,
    (PCRE2_UCHAR8 *)data(result), size(result)));
  return result;
}

void throw_error(int error_number, SRCLOC) {
  std::string error = format_error(error_number);
  std::cerr << location.file_name() << ":" << location.line() << ": "
            << format_error(error_number) << "\n";
  throw std::runtime_error{"Regex error"};
}

re::code re::regex(const char *pattern, SRCLOC) {
  int error_number;
  PCRE2_SIZE error_offset;
  if (code p{pcre2_compile_8((PCRE2_SPTR8)pattern, PCRE2_ZERO_TERMINATED, 0,
        &error_number, &error_offset, nullptr)};
      p) {
    return p;
  }
  std::cerr << location.file_name() << ":" << location.line() << ": "
            << format_error(error_number) << "\n"
            << pattern << "\n"
            << std::string(error_offset, ' ') << "^" << std::endl;
  throw std::runtime_error{"Regex compile error"};
}

re::match_data re::match(const re::code &re, const std::string &subject,
  SRCLOC) {
  match_data p{pcre2_match_data_create_from_pattern(&*re, nullptr)};
  int rc = pcre2_match(&*re, (PCRE2_SPTR8)data(subject), PCRE2_ZERO_TERMINATED,
    0, 0, &*p, nullptr);
  if (rc < 0) {
    if (rc != PCRE2_ERROR_NOMATCH) { throw_error(rc, location); }
    p.reset();
  }
  return p;
}

re::match_data re::match(const re::code &re, std::string_view subject, SRCLOC) {
  match_data p{pcre2_match_data_create_from_pattern(&*re, nullptr)};
  int rc = pcre2_match(&*re, (PCRE2_SPTR8)data(subject), size(subject), 0, 0,
    &*p, nullptr);
  if (rc < 0) {
    if (rc != PCRE2_ERROR_NOMATCH) { throw_error(rc, location); }
    p.reset();
  }
  return p;
}

bool re::matched(const re::match_data &mdata, int n, SRCLOC) {
  int rc = pcre2_substring_length_bynumber(&*mdata, n, nullptr);
  if (rc < 0 && rc != PCRE2_ERROR_UNSET) { throw_error(rc, location); }
  return rc != PCRE2_ERROR_UNSET;
}

std::size_t re::matched_length(const re::match_data &mdata, int n, SRCLOC) {
  PCRE2_SIZE length;
  int rc = pcre2_substring_length_bynumber(&*mdata, n, &length);
  if (rc < 0) { throw_error(rc, location); }
  return length;
}

std::pair<std::size_t, std::size_t> re::match_bounds(const re::match_data &mdata,
  int n, SRCLOC) {
  if (!re::matched(mdata, n, location)) {
    throw_error(PCRE2_ERROR_UNSET, location);
  }
  auto p = pcre2_get_ovector_pointer(&*mdata);
  return {p[n * 2], p[n * 2 + 1]};
}

std::string_view re::match_view(const re::match_data &mdata, int n,
  std::string_view subject, SRCLOC) {
  auto [begin, end] = match_bounds(mdata, n, location);
  return std::string_view(&subject[begin], end - begin);
}

std::string_view re::match_view(const re::match_data &mdata, int n,
  const std::string &subject, SRCLOC) {
  auto [begin, end] = match_bounds(mdata, n, location);
  return std::string_view(&subject[begin], end - begin);
}

std::string re::match_string(const re::match_data &mdata, int n, SRCLOC) {
  auto length = re::matched_length(mdata, n, location);
  std::string result(length, '\0');
  ++length; // writable buffer size including null terminator
  if (int rc = pcre2_substring_copy_bynumber(&*mdata, n,
        reinterpret_cast<PCRE2_UCHAR8 *>(data(result)), &length);
      rc < 0) {
    throw_error(rc, location);
  }
  result.resize(length);
  return result;
}
