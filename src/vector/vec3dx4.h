#ifndef VECTOR3DX4_HPP
#define VECTOR3DX4_HPP

#include "../global.hpp"
#include "../external/simd.hpp"
#include <ostream>
/*
  向量|点|颜色 {x, y, z, w}
  simd<double, 4> e
*/
class vec3d {
public:
  using f64x4 = simd<double, 4>;
  f64x4 e;

public:
  vec3d() : e() {}
  vec3d(const vec3d &) = default;
  vec3d(vec3d &&) = default;
  auto operator=(const vec3d &v) -> vec3d & {
    e.r = v.e.r;
    return *this;
  };
  auto operator=(vec3d &&v) noexcept -> vec3d & {
    e.r = v.e.r;
    return *this;
  };
  vec3d(double xx) : e(xx, xx, xx, 0) {}
  vec3d(double xx, double yy, double zz) : e(xx, yy, zz, 0) {}
  vec3d(const f64x4 &other) : e(other.r) {}
  // clang-format off
  auto operator-() const -> vec3d { return -e; }
  auto operator[](int i) const -> double { return i < 3 ? e[i] : 0; }
  auto operator[](int i) -> double & { return i < 3 ? e[i] : e[3]; }
  [[nodiscard]] auto x() const -> double { return e[0]; }
  [[nodiscard]] auto y() const -> double { return e[1]; }
  [[nodiscard]] auto z() const -> double { return e[2]; }
  // 求和 x + y + z
  [[nodiscard]] auto sum3() const -> double { return x() + y() + z(); }
  // 乘 r {x * r, y * r, z * r}
  auto operator*(const double &r) const -> vec3d { return e * r; }
  // 除 r {x / r, y / r, z / r}
  auto operator/(const double &r) const -> vec3d { return e / r; }
  // vec * vec 对应位置想乘 {x * x, y * y, z * z}
  auto operator*(const vec3d &v) const -> vec3d { return e * v.e; }
  // 对应位置相减
  auto operator-(const vec3d &v) const -> vec3d { return e - v.e; }
  // 对应位置相加
  auto operator+(const vec3d &v) const -> vec3d { return e + v.e; }
  // 对应位置相加
  auto operator+=(const vec3d &v) -> vec3d & { e += v.e; return *this; }
  // 乘 r {x * r, y * r, z * r}
  auto operator*=(const double &r) -> vec3d & { e *= r; return *this; }
  // 除 r {x / r, y / r, z / r}
  auto operator/=(const double &r) -> vec3d & { return *this *= 1 / r; }
  //  r * vec {x * r, y * r, z * r}
  friend auto operator*(const double &r, const vec3d &v) -> vec3d { return v * r; }
  //  vec / r {x * r, y * r, z * r}
  friend auto operator/(const vec3d &v, const vec3d &div) -> vec3d { return v.e / div.e; }
  
  // 模的平方
  [[nodiscard]] auto length_squared() const -> double { return vec3d(e * e).sum3(); }
  // 模
  [[nodiscard]] auto length() const -> double { return std::sqrt(length_squared()); }
  // 随机vec (x, y, z) \in (0, 1.0)
  inline static auto random() -> vec3d { return {random_double(), random_double(), random_double()}; }
  // 随机vec (x, y, z) \in (min, max)
  inline static auto random(double min, double max) -> vec3d {
    auto fun = random_double(min, max);
    return {fun(), fun(), fun()};
  }
  // 是否为 0 向量
  [[nodiscard]] inline auto near_zero() const -> bool {
    // 方法 1
    return (std::fabs(x()) < esp) && (std::fabs(y()) < esp) && (std::fabs(z()) < esp);
    // 方法 2
    // return length_squared() < esp3;
  }
  // {x, y, z} 开方
  inline auto sqrt() -> vec3d & {
    auto res = std::sqrt(e);
    this->e = res.r;
    return *this;
  }
  // 变为单位向量
  inline auto unit_vector() -> vec3d { return e /= length(); }
  // clang-format on
  // 输出{x, y, z}
  friend auto operator<<(std::ostream &os, const vec3d &v) -> std::ostream & {
    return os << "[x] = " << std::setw(6) << v.x() << ", [y] = " << std::setw(6) << v.y() << ", [z] = " << std::setw(6)
              << v.z();
  }
};

// 点乘
inline auto dot(const vec3d &a, const vec3d &b) -> double {
  return (a * b).sum3();
}
// 叉乘
inline auto cross(const vec3d &a, const vec3d &b) -> vec3d {
  return {a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()};
}

// 变为单位向量
inline auto unit_vector(vec3d v) -> vec3d {
  return v /= v.length();
}

// 求镜面反射光线方向向量 (入射光方向向量，法线方向向量)
inline auto reflect(const vec3d &v, const vec3d &n) -> vec3d {
  return v - 2 * dot(v, n) * n;
}

// 聂耳定律 -> 求折射光的方向（入射光方向，法线，折射率）
inline auto refract(const vec3d &uv, const vec3d &n, double etai_over_etat) -> vec3d {
  auto cos_theta = std::min(dot(-uv, n), 1.0);
  vec3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3d r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

// 随机生成一个在单位圆内的点
auto random_in_unit_sphere() -> vec3d {
  while (true) {
    auto p = vec3d::random(-1, 1);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

// 生成一个随机方向的单位向量
inline auto random_unit_vector() -> vec3d {
  return unit_vector(random_in_unit_sphere());
}

// 生成与法线方向同向的单位向量
inline auto random_in_hemisphere(const vec3d &normal) -> vec3d {
  // 判断与法线的位置关系
  vec3d in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

// 平面单位圆内随机一点（z = 0）；
auto random_in_unit_disk() -> vec3d {
  auto fun = random_double(-1, 1);
  while (true) {
    auto p = vec3d(fun(), fun(), 0);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}
//
inline auto random_cosine_direction() -> vec3d {
  auto r1 = random_double();
  auto r2 = random_double();

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);
  auto z = sqrt(1 - r2);

  return {x, y, z};
}
//
inline auto random_to_sphere(double radius, double distance_squared) -> vec3d {
  auto r1 = random_double();
  auto r2 = random_double();
  auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(1 - z * z);
  auto y = sin(phi) * sqrt(1 - z * z);

  return {x, y, z};
}

// 线性插值
inline auto lerp(const vec3d &a, const vec3d &b, const float &t) -> vec3d {
  return a * (1 - t) + b * t;
}

using point3 = vec3d;
using color = vec3d;

#endif