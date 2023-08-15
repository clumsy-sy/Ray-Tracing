#ifndef VECTOR3DX4_HPP
#define VECTOR3DX4_HPP

#include "../global.hpp"
#include "../external/simd.hpp"
/*
  向量
*/
class Vec3d {
public:
  using f64x4 = simd<double, 4>;
  f64x4 e;

public:
  Vec3d() : e() {}
  Vec3d(const Vec3d &) = default;
  Vec3d(Vec3d &&) = default;
  auto operator=(const Vec3d &v) -> Vec3d & {
    e.r = v.e.r;
    return *this;
  };
  auto operator=(Vec3d &&v) noexcept -> Vec3d & {
    e.r = v.e.r;
    return *this;
  };
  Vec3d(double xx) : e(xx, xx, xx, 0) {}
  Vec3d(double xx, double yy, double zz) : e(xx, yy, zz, 0) {}
  Vec3d(const f64x4 &other) : e(other.r) {}

  auto operator-() const -> Vec3d {
    return -e;
  }
  auto operator[](int i) const -> double {
    return i < 3 ? e[i] : 0;
  }
  auto operator[](int i) -> double & {
    return i < 3 ? e[i] : e[3];
  }
  [[nodiscard]] auto x() const -> double {
    return e[0];
  }
  [[nodiscard]] auto y() const -> double {
    return e[1];
  }
  [[nodiscard]] auto z() const -> double {
    return e[2];
  }
  [[nodiscard]] auto sum3() const -> double {
    return x() + y() + z();
  }
  // 对应位置相乘 x * x, y * y, z * z
  auto operator*(const double &r) const -> Vec3d {
    return e * r;
  }
  // 对应位置相除 x / x, y / y, z / z
  auto operator/(const double &r) const -> Vec3d {
    return e / r;
  }
  auto operator*(const Vec3d &v) const -> Vec3d {
    return e * v.e;
  }
  auto operator-(const Vec3d &v) const -> Vec3d {
    return e - v.e;
  }
  auto operator+(const Vec3d &v) const -> Vec3d {
    return e + v.e;
  }
  auto operator+=(const Vec3d &v) -> Vec3d & {
    e += v.e;
    return *this;
  }
  auto operator*=(const double &r) -> Vec3d & {
    e *= r;
    return *this;
  }
  auto operator/=(const double &r) -> Vec3d & {
    return *this *= 1 / r;
  }
  friend auto operator*(const double &r, const Vec3d &v) -> Vec3d {
    return v * r;
  }
  friend auto operator/(const Vec3d &v, const Vec3d &div) -> Vec3d {
    return v.e / div.e;
  }
  friend auto operator<<(std::ostream &os, const Vec3d &v) -> std::ostream & {
    return os << v.x() << ", " << v.y() << ", " << v.z();
  }
  // 模的平方
  [[nodiscard]] auto length_squared() const -> double {
    return Vec3d(e * e).sum3();
  }
  // 模
  [[nodiscard]] auto length() const -> double {
    return std::sqrt(length_squared());
  }
  // 随机 (x, y, z) \in (0, 1.0)
  inline static auto random() -> Vec3d {
    return {random_double(), random_double(), random_double()};
  }
  // 随机 (x, y, z) \in (min, max)
  inline static auto random(double min, double max) -> Vec3d {
    auto fun = random_double(min, max);
    return {fun(), fun(), fun()};
  }
  // 是否为 0 向量
  [[nodiscard]] auto near_zero() const -> bool {
    return (std::fabs(x()) < esp) && (std::fabs(y()) < esp) && (std::fabs(z()) < esp);
    // return length_squared() < esp3;
  }
  // x, y, z 开方
  inline auto sqrt() -> Vec3d & {
    auto res = std::sqrt(e);
    this->e = res.r;
    return *this;
  }
  // 变为单位向量
  inline auto unit_vector() -> Vec3d {
    return e /= length();
  }
};

// 点乘
inline auto dot(const Vec3d &a, const Vec3d &b) -> double {
  return (a * b).sum3();
}
// 叉乘
inline auto cross(const Vec3d &a, const Vec3d &b) -> Vec3d {
  return {a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()};
}

// 变为单位向量
inline auto unit_vector(Vec3d v) -> Vec3d {
  return v /= v.length();
}

// 求镜面反射光线方向向量 (入射光方向向量，法线方向向量)
inline auto reflect(const Vec3d &v, const Vec3d &n) -> Vec3d {
  return v - 2 * dot(v, n) * n;
}

// 聂耳定律 -> 求折射光的方向（入射光方向，法线，折射率）
inline auto refract(const Vec3d &uv, const Vec3d &n, double etai_over_etat) -> Vec3d {
  auto cos_theta = std::min(dot(-uv, n), 1.0);
  Vec3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
  Vec3d r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

// 随机生成一个在单位圆内的点
inline auto random_in_unit_sphere() -> Vec3d {
  while (true) {
    auto p = Vec3d::random(-1, 1);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

// 生成一个随机方向的单位向量
inline auto random_unit_vector() -> Vec3d {
  return unit_vector(random_in_unit_sphere());
}

// 生成与法线方向同向的单位向量
inline auto random_in_hemisphere(const Vec3d &normal) -> Vec3d {
  // 判断与法线的位置关系
  Vec3d in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

// 平面单位圆内随机一点（z = 0）；
auto random_in_unit_disk() -> Vec3d {
  auto fun = random_double(-1, 1);
  while (true) {
    auto p = Vec3d(fun(), fun(), 0);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

inline auto random_cosine_direction() -> Vec3d {
  auto r1 = random_double();
  auto r2 = random_double();

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);
  auto z = sqrt(1 - r2);

  return {x, y, z};
}

// 线性插值
inline auto lerp(const Vec3d &a, const Vec3d &b, const float &t) -> Vec3d {
  return a * (1 - t) + b * t;
}

// point3 and color can use vector to representation
using point3 = Vec3d;
using color = Vec3d;

#endif