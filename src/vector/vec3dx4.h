/**
 * @file vec3dx4.h
 * @brief vec3 double类，可以用来代表{点、颜色、向量} SIMD 加速
 *
 */
#ifndef VECTOR3DX4_HPP
#define VECTOR3DX4_HPP

#include "../global.hpp"
#include <ostream>

#include "immintrin.h"

const double dZero = 0.0;
const double dOne = 1.0;

/**
 * @class vec double
 * @brief 向量|点|颜色 {x, y, z, w}
 *
 * 类型：simd<double, 4> e
 * 构造函数：(),(d),(d,d,d,0.0),(d,d,d,d)
 * 取值: [],{x,y,z,w},{r,g,b,a}
 * 运算：-,+,-,*,/,+=,*=,/=,sum3,取模,模方,随机,开方
 * 输出格式 [x] = , [y] = , [z] =
 */
class vec3d {
public:
  using f64x4 = __m256d;
  f64x4 e;

public:
  // 构造、析构与赋值
  vec3d() : e() {}
  vec3d(const vec3d &) = default;
  vec3d(vec3d &&) = default;
  auto operator=(const vec3d &v) -> vec3d & {
    e = v.e;
    return *this;
  };
  auto operator=(vec3d &&v) noexcept -> vec3d & {
    e = v.e;
    return *this;
  };
  vec3d(const f64x4 &f4) : e(f4){};
  vec3d(double xx) : e{xx, xx, xx, dZero} {}
  vec3d(double xx, double yy, double zz) : e{xx, yy, zz, dZero} {}
  vec3d(double xx, double yy, double zz, double ww) : e{xx, yy, zz, ww} {}
  ~vec3d() = default;
  // clang-format off
  auto operator-() const -> vec3d { return -e; }
  auto operator[](int i) const -> double { return e[i]; }
  auto operator[](const int i) -> double & {
    auto *values = reinterpret_cast<double *>(&e);
    return values[i];
  }
  [[nodiscard]] inline auto x() const -> double { return e[0]; }
  [[nodiscard]] inline auto y() const -> double { return e[1]; }
  [[nodiscard]] inline auto z() const -> double { return e[2]; }
  [[nodiscard]] inline auto w() const -> double { return e[3]; }
  [[nodiscard]] inline auto r() const -> double { return e[0]; }
  [[nodiscard]] inline auto g() const -> double { return e[1]; }
  [[nodiscard]] inline auto b() const -> double { return e[2]; }
  [[nodiscard]] inline auto a() const -> double { return e[3]; }
  // 求和 x + y + z
  [[nodiscard]] auto sum3() const -> double { return e[0] + e[1] + e[2]; }
  [[nodiscard]] static auto raw_sum3(f64x4 e) -> double { return e[0] + e[1] + e[2]; }
  // 乘 r {x * r, y * r, z * r}
  auto operator*(const double &r) const -> vec3d { return e * r; }
  // 除 r {x / r, y / r, z / r}
  auto operator/(const double &r) const -> vec3d { return e / r; }
  // vec * vec 对应位置相乘 {x * x, y * y, z * z}
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
  auto operator/=(const double &r) -> vec3d & { return *this *= dOne / r; }
  //  r * vec {x * r, y * r, z * r}
  friend auto operator*(const double &r, const vec3d &v) -> vec3d { return v * r; }
  //  vec / r {x * r, y * r, z * r}
  friend auto operator/(const vec3d &v, const vec3d &div) -> vec3d { return v.e / div.e; }
  // 模的平方
  [[nodiscard]] auto length_squared() const -> double { return raw_sum3(e * e); }
  // 模
  [[nodiscard]] auto length() const -> double { return std::sqrt(length_squared()); }
  // 随机vec (x, y, z) \in (0, 1.0)
  inline static auto random() -> vec3d { return {random_double(), random_double(), random_double()}; }
  // clang-format on
  // 随机vec (x, y, z) \in (min, max)
  inline static auto random(double min, double max) -> vec3d {
    auto fun = random_double(min, max);
    return {fun(), fun(), fun()};
  }
  // 是否为 0 向量
  [[nodiscard]] inline auto near_zero() const -> bool {
    // 方法 1
    return (std::fabs(e[0]) < esp) && (std::fabs(e[1]) < esp) && (std::fabs(e[2]) < esp);
    // 方法 2
    // return length_squared() < esp3;
  }
  // {x, y, z} 开方
  inline auto sqrt() -> vec3d & {
    e = _mm256_sqrt_pd(e);
    return *this;
  }
  // 变为单位向量
  inline auto unit_vector() -> vec3d {
    // if (length() == dZero)
    //   return {1, 1, 1, 0};
    return e /= length();
  }

  auto operator==(const vec3d &other) -> bool {
    auto result = _mm256_cmp_pd(e, other.e, _CMP_EQ_OQ);
    int mask = _mm256_movemask_pd(result);
    return mask == 0x0F;
  }
  // 输出{x, y, z}
  friend auto operator<<(std::ostream &os, const vec3d &v) -> std::ostream & {
    return os << "(" << std::fixed << std::setprecision(4) << v.x() << ", " << std::fixed
              << std::setprecision(4) << v.y() << ", " << std::fixed << std::setprecision(4)
              << v.z() << ")";
  }
};

// 点乘
inline auto dot(const vec3d &a, const vec3d &b) -> double {
  return (a * b).sum3();
}
// 叉乘
inline auto cross(const vec3d &a, const vec3d &b) -> vec3d {
  // std::cout << "cross" << a << "  " << b << "\n";
  return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}
// 变为单位向量
inline auto unit_vector(vec3d v) -> vec3d {
  return v.unit_vector();
}

// 求镜面反射光线方向向量 (入射光方向向量，法线方向向量)
inline auto reflect(const vec3d &v, const vec3d &n) -> vec3d {
  return v - 2 * dot(v, n) * n;
}

// 聂耳定律 -> 求折射光的方向（入射光方向，法线，折射率）
inline auto refract(const vec3d &uv, const vec3d &n, double etai_over_etat) -> vec3d {
  auto cos_theta = std::min(dot(-uv, n), dOne);
  vec3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3d r_out_parallel = -std::sqrt(std::abs(dOne - r_out_perp.length_squared())) * n;
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
  return random_in_unit_sphere().unit_vector();
}

// 生成与法线方向同向的单位向量
inline auto random_in_hemisphere(const vec3d &normal) -> vec3d {
  // 判断与法线的位置关系
  vec3d in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0) // 与法线同一半球
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

// 平面单位圆内随机一点（z = 0）；
auto random_in_unit_disk() -> vec3d {
  // double angle = random_double() * 2 * PI;  // 生成随机的角度
  // double radius = random_double();  // 生成随机的半径

  // #ifdef DEBUG
  //   auto p = vec3d(radius * cos(angle), radius * sin(angle), 0.0);
  //   if (p.length_squared() >= 1) {
  //     std::cerr << " not in a circle \n";
  //   }
  // #endif

  // return {radius * cos(angle), radius * sin(angle), 0.0};

  auto fun = random_double(-1, 1);
  while (true) {
    auto p = vec3d(fun(), fun(), 0);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

// 随机 cos 方向
inline auto random_cosine_direction() -> vec3d {
  auto r1 = random_double();
  auto r2 = random_double();

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);
  auto z = sqrt(1 - r2);

  return {x, y, z};
}

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
inline auto merge_min(const vec3d &l, const vec3d &r) -> vec3d {
  return _mm256_min_pd(l.e, r.e);
}
inline auto merge_max(const vec3d &l, const vec3d &r) -> vec3d {
  return _mm256_max_pd(l.e, r.e);
}

using point3 = vec3d;
using color = vec3d;

#endif