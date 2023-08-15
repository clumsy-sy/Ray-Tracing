#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "../global.hpp"

class interval {
public:
  double min, max;

  interval() : min(+infinity), max(-infinity) {} // Default interval is empty
  interval(double num) : min(num), max(num) {}
  interval(double _min, double _max) : min(_min), max(_max) {}
  interval(const interval &a, const interval &b) : min(std::min(a.min, b.min)), max(std::max(a.max, b.max)) {}

  [[nodiscard]] auto size() const -> double {
    return max - min;
  }

  [[nodiscard]] auto expand(double delta) const -> interval {
    auto padding = delta / 2;
    return {min - padding, max + padding};
  }

  [[nodiscard]] inline auto average() const -> double {
    return (min + max) / 2;
  }

  [[nodiscard]] inline auto contains(double x) const -> bool {
    return min <= x && x <= max;
  }

  [[nodiscard]] inline auto surrounds(double x) const -> bool {
    return min < x && x < max;
  }

  [[nodiscard]] inline auto outside(double x) const -> bool {
    return min > x || x > max;
  }

  [[nodiscard]] auto clamp(double x) const -> double {
    if (x < min)
      return min;
    if (x > max)
      return max;
    return x;
  }

  static const interval empty, universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

auto operator+(const interval &ival, double displacement) -> interval {
  return {ival.min + displacement, ival.max + displacement};
}

auto operator+(double displacement, const interval &ival) -> interval {
  return ival + displacement;
}

#endif