#ifndef AABB_HPP
#define AABB_HPP

#include "../vector/Vec3dx4.hpp"
#include "ray.hpp"
#include "interval.hpp"

// Axis-Aligned Bounding Boxes
// 三维空间的盒子，最小坐标 + 最大坐标 的 矩形
class aabb {
public:
  // point3 minimum, maximum;
  std::array<interval, 3> axis; // x : 0, y : 1, z : 2

public:
  aabb() = default; // 初始化为 interval 的默认构造 即 [INF, -INF]
  aabb(const point3 &p) {
    axis[0] = interval(p[0], p[0]);
    axis[1] = interval(p[1], p[1]);
    axis[2] = interval(p[2], p[2]);
  }
  aabb(const point3 &a, const point3 &b) {
    axis[0] = interval(std::min(a[0], b[0]), std::max(a[0], b[0]));
    axis[1] = interval(std::min(a[1], b[1]), std::max(a[1], b[1]));
    axis[2] = interval(std::min(a[2], b[2]), std::max(a[2], b[2]));
  }
  aabb(const aabb &box0, const aabb &box1) {
    axis[0] = interval(box0.x(), box1.x());
    axis[1] = interval(box0.y(), box1.y());
    axis[2] = interval(box0.z(), box1.z());
  }
  aabb(const interval &ix, const interval &iy, const interval &iz) {
    axis[0] = ix;
    axis[1] = iy;
    axis[2] = iz;
  }
  aabb(const aabb &other) = default;
  auto operator=(const aabb &other) -> aabb & = default;
  auto min() -> point3 const {
    return {axis[0].min, axis[1].min, axis[2].min};
  };
  auto max() -> point3 const {
    return {axis[0].max, axis[1].max, axis[2].max};
  };
  auto center() -> point3 const {
    return {axis[0].average(), axis[1].average(), axis[2].average()};
  }
  auto x() -> interval & {
    return axis[0];
  }
  auto y() -> interval & {
    return axis[1];
  }
  auto z() -> interval & {
    return axis[2];
  }
  [[nodiscard]] auto x() const -> interval {
    return axis[0];
  }
  [[nodiscard]] auto y() const -> interval {
    return axis[1];
  }
  [[nodiscard]] auto z() const -> interval {
    return axis[2];
  }
  auto pad() -> aabb {
    // Return an AABB that has no side narrower than some delta, padding if necessary.
    double delta = 0.0001;
    interval new_x = (x().size() >= delta) ? x() : x().expand(delta);
    interval new_y = (y().size() >= delta) ? y() : y().expand(delta);
    interval new_z = (z().size() >= delta) ? z() : z().expand(delta);

    return {new_x, new_y, new_z};
  }
  // old version
  [[nodiscard]] auto hit_old(const ray &r, interval ray_t) const -> bool {
    // 判断光线 与 AABB 是否相交，判断与三个面的交面，是否有重合
    for (int i = 0; i < 3; i++) {
      auto t0 =
          std::min((axis[i].min - r.origin()[i]) / r.direction()[i], (axis[i].max - r.origin()[i]) / r.direction()[i]);
      auto t1 =
          std::max((axis[i].min - r.origin()[i]) / r.direction()[i], (axis[i].max - r.origin()[i]) / r.direction()[i]);
      ray_t.min = std::max(t0, ray_t.min);
      ray_t.max = std::min(t1, ray_t.max);
      if (ray_t.max <= ray_t.min)
        return false;
    }
    return true;
  }
  // Andrew Kensler 优化
  [[nodiscard]] auto hit(const ray &r, interval ray_t) const -> bool {
    for (int i = 0; i < 3; i++) {
      auto invD = 1.0f / r.direction()[i];
      auto t0 = (axis[i].min - r.origin()[i]) * invD;
      auto t1 = (axis[i].max - r.origin()[i]) * invD;
      if (invD < 0.0f)
        std::swap(t0, t1);
      if (t0 > ray_t.min)
        ray_t.min = t0;
      if (t1 < ray_t.max)
        ray_t.max = t1;
      if (ray_t.max <= ray_t.min)
        return false;
    }
    return true;
  }
};
// 包围和
inline auto surrounding_box(const aabb &box0, const aabb &box1) -> aabb {
  // clang-format off
  return {
    interval(box0.x(), box1.x()),
    interval(box0.y(), box1.y()),
    interval(box0.z(), box1.z())
  };
}
#endif