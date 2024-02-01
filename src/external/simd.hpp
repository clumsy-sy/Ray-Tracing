/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 */

#ifndef _simd_hpp_
#define _simd_hpp_
#include "../global.hpp"

template <class T, unsigned int N>
class simd {
public:
  // Number of elements contained in this object
  static constexpr unsigned int size = N;

  // Type of the elements contained in this object
  using type = T;

#if defined(__clang__)
  // Underlying vector type aligned
  using aligned = T __attribute__((ext_vector_type(N)));

  // Underlying vector type unaligned
  using unaligned = T __attribute__((ext_vector_type(N), __aligned__(1)));
#else
  // Underlying vector type aligned
  typedef T aligned __attribute__((vector_size(N * sizeof(T))));

  // Underlying vector type unaligned
  typedef T unaligned __attribute__((vector_size(N * sizeof(T)), __aligned__(1)));
#endif

  // Underlying vector containing data
  aligned r;

  constexpr simd() = default;
  constexpr simd(const aligned &r1) : r(r1) {}
  constexpr simd(const simd &s) : r(s.r) {}

  // Costruction from single or multiple scalar values
  template <class V>
  constexpr simd(const V &x) : r(T(x) - aligned{}) {}
  template <class... V>
  constexpr simd(const V &...x) : r{T(x)...} {}

  // Construction from different simd type
  template <class V>
  constexpr simd(const simd<V, N> &x) : r(__builtin_convertvector(x.r, aligned)) {}

  // Store and load operations
  static auto load(const T *p) -> simd {
    return *reinterpret_cast<const aligned *>(p);
  }
  static auto loadu(const T *p) -> simd {
    return *reinterpret_cast<const unaligned *>(p);
  }

  // Assignment operators
  template <class V>
  auto operator=(const V &x) -> simd & {
    r = simd(x).r;
    return *this;
  }
  template <class V>
  auto operator+=(const V &x) -> simd & {
    r += simd(x).r;
    return *this;
  }
  template <class V>
  auto operator-=(const V &x) -> simd & {
    r -= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator*=(const V &x) -> simd & {
    r *= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator/=(const V &x) -> simd & {
    r /= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator%=(const V &x) -> simd & {
    r %= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator&=(const V &x) -> simd & {
    r &= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator|=(const V &x) -> simd & {
    r |= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator^=(const V &x) -> simd & {
    r ^= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator<<=(const V &x) -> simd & {
    r <<= simd(x).r;
    return *this;
  }
  template <class V>
  auto operator>>=(const V &x) -> simd & {
    r >>= simd(x).r;
    return *this;
  }

  // Conversion operator
  constexpr operator aligned() const {
    return r;
  }

  // Unary operators
  constexpr auto operator+() const -> simd {
    return +r;
  }
  constexpr auto operator-() const -> simd {
    return -r;
  }
  constexpr auto operator~() const -> simd {
    return ~r;
  }

  // Access operators
  auto operator[](unsigned int i) -> T & {
    return r[i];
  }
  constexpr auto operator[](unsigned int i) const -> T {
    return r[i];
  }

  // Shuffle operators
  template <class V, unsigned int M>
  constexpr auto operator[](const simd<V, M> &s) const -> simd<T, M> {
    return __builtin_shuffle(r, r, s.r);
  }

  template <class V>
  constexpr auto shuffle(const simd<V, N> &a, const simd<V, N> &b) const -> simd<V, N> {
    return __builtin_shuffle(a.r, b.r, r);
  }

  // Integer type of the same length
  using int_type = simd<decltype((r < r)[0]), N>;
};

namespace std {
// std::common_type definitions for simd objects
template <class T, class V, unsigned int N>
struct common_type<simd<T, N>, simd<V, N>> {
  using type = simd<common_type_t<T, V>, N>;
};

template <class T, class V, unsigned int N>
struct common_type<simd<T, N>, V> {
  using type = simd<common_type_t<T, V>, N>;
};

template <class T, class V, unsigned int N>
struct common_type<T, simd<V, N>> {
  using type = simd<common_type_t<T, V>, N>;
};

#ifdef _GLIBCXX_OSTREAM
template <class T, unsigned int N>
ostream &operator<<(ostream &o, const simd<T, N> &s) {
  o << '(' << s[0];

  for (int i = 1; i < N; i++)
    o << ',' << s[i];

  return o << ')';
}
#endif

#ifdef _GLIBCXX_ISTREAM
template <class T, unsigned int N>
istream &operator>>(istream &is, simd<T, N> &s) {
  for (int i = 0; i < N; i++)
    is >> s[i];

  return is;
}
#endif
} // namespace std

template <class T>
constexpr bool is_simd = false;

template <class T, unsigned int N>
constexpr bool is_simd<simd<T, N>> = true;

template <class T>
constexpr bool is_simd_or_scalar = std::is_arithmetic<T>::value || is_simd<T>;

// Binary operators
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator+(const T &x, const V &y) -> R {
  return R(x).r + R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator-(const T &x, const V &y) -> R {
  return R(x).r - R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator*(const T &x, const V &y) -> R {
  return R(x).r * R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator/(const T &x, const V &y) -> R {
  return R(x).r / R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator%(const T &x, const V &y) -> R {
  return R(x).r % R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator&&(const T &x, const V &y) -> R {
  return R(x).r && R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator||(const T &x, const V &y) -> R {
  return R(x).r || R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator&(const T &x, const V &y) -> R {
  return R(x).r & R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator|(const T &x, const V &y) -> R {
  return R(x).r | R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator^(const T &x, const V &y) -> R {
  return R(x).r ^ R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator<<(const T &x, const V &y) -> R {
  return R(x).r << R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>>
constexpr auto operator>>(const T &x, const V &y) -> R {
  return R(x).r >> R(y).r;
}

// Comparison operators
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>, class M = typename R::int_type>
constexpr auto operator==(const T &x, const V &y) -> M {
  return R(x).r == R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>, class M = typename R::int_type>
constexpr auto operator!=(const T &x, const V &y) -> M {
  return R(x).r != R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>, class M = typename R::int_type>
constexpr auto operator<(const T &x, const V &y) -> M {
  return R(x).r < R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>, class M = typename R::int_type>
constexpr auto operator<=(const T &x, const V &y) -> M {
  return R(x).r <= R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>, class M = typename R::int_type>
constexpr auto operator>(const T &x, const V &y) -> M {
  return R(x).r > R(y).r;
}
template <class T, class V,
    class E = std::enable_if_t<is_simd_or_scalar<T> && is_simd_or_scalar<V>>,
    class R = std::common_type_t<T, V>, class M = typename R::int_type>
constexpr auto operator>=(const T &x, const V &y) -> M {
  return R(x).r >= R(y).r;
}

template <class R, class T, unsigned int N>
inline auto map(R (*function)(T), const simd<T, N> &s) -> simd<R, N> {
  simd<R, N> out;

  for (unsigned int i = 0; i < N; i++)
    out[i] = function(s[i]);

  return out;
}

template <class T, unsigned int N, class F, class R = std::result_of_t<F(T)>>
inline auto map(const F &function, const simd<T, N> &s) -> simd<R, N> {
  simd<R, N> out;

  for (unsigned int i = 0; i < N; i++)
    out[i] = function(s[i]);

  return out;
}

template <class R, class T, unsigned int N>
inline auto map(R (*function)(T, T), const simd<T, N> &a, const simd<T, N> &b) -> simd<R, N> {
  simd<R, N> out;

  for (unsigned int i = 0; i < N; i++)
    out[i] = function(a[i], b[i]);

  return out;
}

template <class T, unsigned int N, class F, class R>
inline auto reduce(const F &function, const simd<T, N> &s, const R &initial) -> R {
  R out = initial;

  for (unsigned int i = 0; i < N; i++)
    out = function(out, s[i]);

  return out;
}

template <class T, class V, class W, class E = std::enable_if_t<is_simd<T>>,
    class R = simd<typename std::common_type_t<T, V, W>::type, T::size>>
constexpr auto blend(const T &test, const V &yes, const W &no) -> R {
  return test.r ? R(yes).r : R(no).r;
}

template <class T, unsigned int N>
inline auto any(const simd<T, N> &s) -> bool {
  bool r = s[0];
  for (unsigned int i = 1; i < N; i++)
    r = r || s[i];
  return r;
}
template <class T, unsigned int N>
inline auto all(const simd<T, N> &s) -> bool {
  bool r = s[0];
  for (unsigned int i = 1; i < N; i++)
    r = r && s[i];
  return r;
}
template <class T, unsigned int N>
inline auto sum(const simd<T, N> &s) -> T {
  T r = s[0];
  for (unsigned int i = 1; i < N; i++)
    r = r + s[i];
  return r;
}
template <class T, unsigned int N>
inline auto prod(const simd<T, N> &s) -> T {
  T r = s[0];
  for (unsigned int i = 1; i < N; i++)
    r = r * s[i];
  return r;
}
template <class T, unsigned int N>
inline auto max(const simd<T, N> &s) -> T {
  T r = s[0];
  for (unsigned int i = 1; i < N; i++)
    r = r > s[i] ? r : s[i];
  return r;
}
template <class T, unsigned int N>
inline auto min(const simd<T, N> &s) -> T {
  T r = s[0];
  for (unsigned int i = 1; i < N; i++)
    r = r < s[i] ? r : s[i];
  return r;
}

namespace std {
template <class T, unsigned int N>
inline auto cos(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::cos, s);
}
template <class T, unsigned int N>
inline auto sin(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::sin, s);
}
template <class T, unsigned int N>
inline auto tan(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::tan, s);
}
template <class T, unsigned int N>
inline auto acos(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::acos, s);
}
template <class T, unsigned int N>
inline auto asin(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::asin, s);
}
template <class T, unsigned int N>
inline auto atan(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::atan, s);
}

template <class T, unsigned int N>
inline auto cosh(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::cosh, s);
}
template <class T, unsigned int N>
inline auto sinh(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::sinh, s);
}
template <class T, unsigned int N>
inline auto tanh(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::tanh, s);
}
template <class T, unsigned int N>
inline auto acosh(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::acosh, s);
}
template <class T, unsigned int N>
inline auto asinh(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::asinh, s);
}
template <class T, unsigned int N>
inline auto atanh(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::atanh, s);
}

template <class T, unsigned int N>
inline auto exp(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::exp, s);
}
template <class T, unsigned int N>
inline auto log(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::log, s);
}
template <class T, unsigned int N>
inline auto log10(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::log10, s);
}
template <class T, unsigned int N>
inline auto exp2(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::exp2, s);
}
template <class T, unsigned int N>
inline auto log1p(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::log1p, s);
}
template <class T, unsigned int N>
inline auto log2(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::log2, s);
}

template <class T, unsigned int N>
inline auto sqrt(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::sqrt, s);
}
template <class T, unsigned int N>
inline auto cbrt(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::cbrt, s);
}
template <class T, unsigned int N>
inline auto hypot(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::hypot, s);
}

template <class T, unsigned int N>
inline auto erf(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::erf, s);
}
template <class T, unsigned int N>
inline auto erfc(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::erfc, s);
}
template <class T, unsigned int N>
inline auto tgamma(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::tgamma, s);
}
template <class T, unsigned int N>
inline auto lgamma(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::lgamma, s);
}

template <class T, unsigned int N>
inline auto ceil(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::ceil, s);
}
template <class T, unsigned int N>
inline auto floor(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::floor, s);
}
template <class T, unsigned int N>
inline auto trunc(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::trunc, s);
}
template <class T, unsigned int N>
inline auto round(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::round, s);
}
template <class T, unsigned int N>
inline auto rint(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::rint, s);
}
template <class T, unsigned int N>
inline auto abs(const simd<T, N> &s) -> simd<T, N> {
  return map<T>(std::abs, s);
}

template <class T, unsigned int N>
inline auto atan2(const simd<T, N> &a, const simd<T, N> &b) -> simd<T, N> {
  return map<T>(std::atan2, a, b);
}
template <class T, unsigned int N>
inline auto pow(const simd<T, N> &a, const simd<T, N> &b) -> simd<T, N> {
  return map<T>(std::pow, a, b);
}

template <class T, unsigned int N>
inline auto max(const simd<T, N> &a, const simd<T, N> &b) -> simd<T, N> {
  return a.r > b.r ? a.r : b.r;
}
template <class T, unsigned int N>
inline auto min(const simd<T, N> &a, const simd<T, N> &b) -> simd<T, N> {
  return a.r < b.r ? a.r : b.r;
}
} // namespace std

#endif
