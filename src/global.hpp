#ifndef GROBAL_HPP
#define GROBAL_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <iomanip>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Constants

// double inf
const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;
const double esp = 1e-8;
const double esp3 = esp * esp * esp;

// 角度转弧度
inline auto degrees_to_radians(double degrees) -> double {
  return degrees * PI / 180.0;
}

// 随机数生成
inline auto random_double() -> double {
  thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  thread_local std::mt19937 generator{std::random_device{}()};
  // thread_local std::mt19937 generator(10085);
  return distribution(generator);
}
inline auto random_double(double min, double max) -> auto{
  return [min, max]() -> double {
    thread_local std::uniform_real_distribution<double> distribution(min, max);
    thread_local std::mt19937 generator{std::random_device{}()};
    // thread_local std::mt19937 generator(10085);
    return distribution(generator);
  };
}
inline auto random_int(int min, int max) -> auto{
  return [min, max]() -> int {
    thread_local std::uniform_int_distribution<> distribution(min, max);
    thread_local std::mt19937 generator{std::random_device{}()};
    // thread_local std::mt19937 generator(10085);
    return distribution(generator);
  };
}
// 判断 x 的范围是否在 [min, max] 之间 否则现在边界
inline auto clamp(const double &x, const double &min, const double &max) -> double {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

// 求根公式
inline auto solveQuadratic(const double &a, const double &b, const double &c, double &x0, double &x1) -> bool {
  double discr = b * b - 4 * a * c;
  if (discr < 0)
    return false;
  else if (discr == 0)
    x0 = x1 = -0.5 * b / a;
  else {
    double q = (b > 0) ? -0.5 * (b + std::sqrt(discr)) : -0.5 * (b - std::sqrt(discr));
    x0 = q / a;
    x1 = c / q;
  }
  if (x0 > x1)
    std::swap(x0, x1);
  return true;
}
// b = 2 * h 情况下的求根公式
inline auto solveQuadratic_halfb(const double &a, const double &half_b, const double &c, double &x0, double &x1)
    -> bool {
  double discr = half_b * half_b - a * c;
  if (discr < 0)
    return false;
  else if (discr == 0)
    x0 = x1 = -half_b / a;
  else {
    double q = -half_b + std::sqrt(discr);
    x0 = q / a;
    x1 = c / q;
  }
  if (x0 > x1)
    std::swap(x0, x1);
  return true;
}
// 进度条（需要加线程锁）
inline void UpdateProgress(double progress) {
  int barWidth = 100;

  std::cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r";
  std::cout.flush();
}
inline void UpdateProgress(std::int32_t now, std::int32_t total) {
  UpdateProgress(double(now) / total);
}

#endif