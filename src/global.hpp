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

/**
 * @brief 角度转弧度
 */
inline auto degrees_to_radians(double degrees) -> double {
  return degrees * PI / 180.0;
}

/**
 * @brief 随机数生成 double(0.0, 1.0)
 */
inline auto random_double() -> double {
  thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  thread_local std::mt19937 generator{std::random_device{}()};
  // thread_local std::mt19937 generator(10085);
  return distribution(generator);
}
/**
 * @brief 随机数生成 double(min, max)
 */
inline auto random_double(double min, double max) -> auto {
  return [min, max]() -> double {
    thread_local std::uniform_real_distribution<double> distribution(min, max);
    thread_local std::mt19937 generator{std::random_device{}()};
    // thread_local std::mt19937 generator(10085);
    return distribution(generator);
  };
}
/**
 * @brief 随机数生成 Int(min, max)
 */
inline auto random_int(int min, int max) -> auto {
  return [min, max]() -> int {
    thread_local std::uniform_int_distribution<> distribution(min, max);
    thread_local std::mt19937 generator{std::random_device{}()};
    // thread_local std::mt19937 generator(10085);
    return distribution(generator);
  };
}
/**
 * @brief 判断 x 的范围是否在 [min, max] 之间 否则现在边界
 */
inline auto clamp(const double &x, const double &min, const double &max) -> double {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

/**
 * @brief 求根公式
 */
inline auto solveQuadratic(
    const double &a, const double &b, const double &c, double &x0, double &x1) -> bool {
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
/**
 * @brief b = 2 * h 情况下的求根公式
 */
inline auto solveQuadratic_halfb(
    const double &a, const double &half_b, const double &c, double &x0, double &x1) -> bool {
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
/**
 * @brief 进度条 百分比（需要加线程锁）
 */
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
/**
 * @brief 进度条[当前进度，总进度]（需要加线程锁）
 */
inline void UpdateProgress(std::int32_t now, std::int32_t total) {
  UpdateProgress(double(now) / total);
}

using point2D = std::pair<double, double>;

inline auto magnitude(point2D p) -> double {
  return p.first * p.first + p.second * p.second;
}

/**
 * @brief 产生从围绕中心的（r，2*r）之间的圆环中均匀选择的样本。
 *
 * @param centre  中心
 * @param r 半径
 * @return point2D
 */
auto sample_annulus(point2D centre, double radius) -> point2D {
  point2D res;
  for (;;) { // simple rejection sampling approach
    res = {4 * (random_double() - 0.5), 4 * (random_double() - 0.5)};
    double r2 = magnitude(res); // magnitude squared of r
    if (r2 > 1 && r2 <= 4)
      break;
  }
  return {centre.first + radius * res.first, centre.second + radius * res.second};
}

auto distance2(const point2D &p1, const point2D &p2) -> double {
  double dx = p1.first - p2.first;
  double dy = p1.second - p2.second;
  return dx * dx + dy * dy;
}

auto isPointInsideRectangle(double x, double y, double width, double height) -> bool {
  return x >= 0.0 && x <= width && y >= 0.0 && y <= height;
}
/**
 * @brief 快速泊松分布采样
 *
 * @param width 区间宽度
 * @param height 区间长度
 * @param numSamples 需要采样数
 * @return std::vector<point3> 输出点
 */
auto fastPoissonDiscSampling(
    double width, double height, double threshold) -> std::vector<point2D> {
  // 重试上限(论文推荐)
  constexpr int32_t max_retry = 30;
  // 采样数组和活动数组
  std::vector<point2D> samples;
  std::vector<uint32_t> activeList;
  // 随机数生成器
  static std::random_device rd;
  static std::mt19937 gen(rd());
  // 分块（2D）
  double cellSize = threshold / std::sqrt(2);
  int gridWidth = std::ceil(width / cellSize);
  int gridHeight = std::ceil(height / cellSize);
  // printf("gridWidth = %d, gridHeight = %d\n", gridWidth, gridHeight);
  // 网格数组，-1 = 未被占，>= 0 代表已被占
  std::vector<std::vector<int>> grid(gridWidth, std::vector<int>(gridHeight, -1));
  // 初始化
  point2D initialSample = {random_double() * width, random_double() * height};
  samples.push_back(initialSample);
  activeList.push_back(0);
  grid[std::floor(initialSample.first / cellSize)][std::floor(initialSample.second / cellSize)] = 0;
  // printf("Initial sample: (%.2f, %.2f)\n", initialSample.first, initialSample.second);
  double radius2 = threshold * threshold;

  while (!activeList.empty()) {
    // 随机选取一个点
    int index = std::uniform_int_distribution<int>(0, activeList.size() - 1)(gen);
    uint32_t idxP = activeList[index];
    point2D currentPoint = samples[idxP];
    bool found = false;
    // printf("[choose point] = {%lf, %lf}\n", currentPoint.first, currentPoint.second);

    for (int i = 0; i < max_retry; ++i) {
      // 新生成一个点
      point2D newPoint =
          sample_annulus(currentPoint, threshold);
      int gridX = std::floor(newPoint.first / cellSize);
      int gridY = std::floor(newPoint.second / cellSize);
      // printf("[try %d]gridX = %d, gridY = %d \n", i, gridX, gridY);

      if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight &&
          grid[gridX][gridY] == -1 && isPointInsideRectangle(newPoint.first, newPoint.second, width, height)) {
        bool close = false;

        for (int x = std::max(0, gridX - 1); x <= std::min(gridX + 1, gridWidth - 1) && !close; ++x) {
          for (int y = std::max(0, gridY - 1); y <= std::min(gridY + 1, gridHeight - 1) && !close; ++y) {
            if (grid[x][y] >= 0 && distance2(newPoint, samples[grid[x][y]]) < radius2) {
              close = true;
            }
          }
        }

        if (!close) {
          uint32_t q = samples.size();
          activeList.push_back(q);
          samples.push_back(newPoint);
          grid[gridX][gridY] = q;
          found = true;
          break;
        }
      }
    }

    if (!found) {
      activeList[index] = activeList[activeList.size() - 1];
      activeList.pop_back();
    }
  }

  // for (int i = 0; i < gridWidth; i++) {
  //   for (int j = 0; j < gridHeight; j++) {
  //     if (grid[i][j] == -1)
  //       printf(".");
  //     else
  //       printf("*");
  //   }
  //   printf("\n");
  // }

  return samples;
}

#endif