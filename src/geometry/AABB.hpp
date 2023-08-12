#ifndef AABB_HPP
#define AABB_HPP

#include "../vector/Vec3dx4.hpp"
#include "ray.hpp"

// Axis-Aligned Bounding Boxes
// 三维空间的盒子，最小坐标 + 最大坐标 的 矩形
class aabb {
public:
  point3 minimum, maximum;

public:
  aabb() = default;
  aabb(const point3 &p) : minimum(p), maximum(p) {}
  aabb(point3 a, point3 b) : minimum(std::move(a)), maximum(std::move(b)) {}
  aabb(const aabb &other) = default;
  auto operator=(const aabb &other) -> aabb & = default;
  auto min() -> point3 const {
    return minimum;
  };
  auto max() -> point3 const {
    return maximum;
  };
  auto center() -> point3 const {
    return {(minimum[0] + maximum[0]) / 2, (minimum[1] + maximum[1]) / 2, (minimum[2] + maximum[2]) / 2};
  }
  // old version
  auto hit_old(const ray &r, double t_min, double t_max) -> bool {
    // 判断光线 与 AABB 是否相交，判断与三个面的交面，是否有重合
    for (int i = 0; i < 3; i++) {
      auto t0 =
          std::min((minimum[i] - r.origin()[i]) / r.direction()[i], (maximum[i] - r.origin()[i]) / r.direction()[i]);
      auto t1 =
          std::max((minimum[i] - r.origin()[i]) / r.direction()[i], (maximum[i] - r.origin()[i]) / r.direction()[i]);
      t_min = std::max(t0, t_min);
      t_max = std::min(t1, t_max);
      if (t_max <= t_min)
        return false;
    }
    return true;
  }
  // Andrew Kensler 优化
  [[nodiscard]] auto hit(const ray &r, double t_min, double t_max) const -> bool {
    for (int i = 0; i < 3; i++) {
      auto invD = 1.0f / r.direction()[i];
      auto t0 = (minimum[i] - r.origin()[i]) * invD;
      auto t1 = (maximum[i] - r.origin()[i]) * invD;
      if (invD < 0.0f)
        std::swap(t0, t1);
      t_min = t0 > t_min ? t0 : t_min;
      t_max = t1 < t_max ? t1 : t_max;
      if (t_max <= t_min)
        return false;
    }
    return true;
  }
};
// 包围和
inline auto surrounding_box(aabb box0, aabb box1) -> aabb {
  // clang-format off
  point3 small(std::min(box0.min().x(), box1.min().x()),
               std::min(box0.min().y(), box1.min().y()),
               std::min(box0.min().z(), box1.min().z()));

  point3 big(std::max(box0.max().x(), box1.max().x()), 
             std::max(box0.max().y(), box1.max().y()),
             std::max(box0.max().z(), box1.max().z()));
  // clang-format on
  return {small, big};
}
#endif