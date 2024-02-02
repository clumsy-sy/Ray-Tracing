#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "hittable.hpp"
#include "interval.hpp"

using pdd = std::pair<double, double>;

class triangle : public hittable {
public:
  /*
    齐次坐标下三角形的信息
    1. 三个点的坐标 V0，V1，V2，1.0 顺时针
    2. 三个点的颜色信息（R，G，B）
    3. 三个点的材质信息 （U，V）
    4. 三个点的法向量信息（Vector3f）
  */
  point3 v0, v1, v2; // vertices A, B ,C , counter-clockwise order
  vec3d e1, e2;      // 2 edges v1-v0, v2-v0; 求光线交有用
  pdd t0, t1, t2;    // texture coords
  vec3d normal;
  material *mat_ptr;
  aabb bbox;

public:
  triangle() = default;
  triangle(point3 _v0, point3 _v1, point3 _v2, material *m)
      : v0(std::move(_v0)), v1(std::move(_v1)), v2(std::move(_v2)), mat_ptr(m) {
    e1 = v1 - v0;
    e2 = v2 - v0;
    normal = unit_vector(cross(e1, e2));
    bbox = getaabb();
  }
  triangle(std::array<vec3d, 3> &vec, std::array<pdd, 3> &tex, material *m) : mat_ptr(m) {
    setVector(vec);
    setTexture(tex);
    e1 = v1 - v0;
    e2 = v2 - v0;
    normal = unit_vector(cross(e1, e2));
    bbox = getaabb();
  }
  auto setVector(std::array<vec3d, 3> &vec) -> void;
  auto setTexture(std::array<pdd, 3> &tex) -> void;
  ~triangle() override = default;
  [[nodiscard]] inline auto getHitPoint(double u, double v) const -> point3;
  inline auto interpolate(double &u, double &v, vec3d &Barycentr, double weight) const -> void;
  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override;
  inline auto getaabb() -> aabb {
    auto ix = interval(std::min({v0[0], v1[0], v2[0]}), std::max({v0[0], v1[0], v2[0]}));
    auto iy = interval(std::min({v0[1], v1[1], v2[1]}), std::max({v0[1], v1[1], v2[1]}));
    auto iz = interval(std::min({v0[2], v1[2], v2[2]}), std::max({v0[2], v1[2], v2[2]}));
    return {ix, iy, iz};
  }
  [[nodiscard]] auto pdf_value(const point3 &origin, const vec3d &v) const -> double override;
  [[nodiscard]] auto random(const point3 &origin) const -> vec3d override {
    auto p = v0 + (random_double() * e1) + (random_double() * e2);
    return p - origin;
  }
  auto print(std::ostream& os, const std::string& prefix = "") const -> void override {
    os << prefix << "[Tri]{" << v0 << "," << v1 << "," << v2 << "}";
  }
  friend auto operator<<(std::ostream &os, const triangle &t) -> std::ostream & {
    return os << "[Tri]{" << t.v0 << "," << t.v1 << "," << t.v2 << "}";
  }
};
auto triangle::setVector(std::array<vec3d, 3> &vec) -> void {
  v0 = vec[0], v1 = vec[1], v2 = vec[2];
}
auto triangle::setTexture(std::array<pdd, 3> &tex) -> void {
  t0 = tex[0], t1 = tex[1], t2 = tex[2];
}
inline auto triangle::getHitPoint(double u, double v) const -> point3 {
  return (1 - u - v) * v0 + u * v1 + v * v2;
}

inline auto triangle::interpolate(double &u, double &v, vec3d &Barycentr, double weight) const
    -> void {
  u = (Barycentr[0] * t0.first + Barycentr[1] * t1.first + Barycentr[2] * t2.first);
  v = (Barycentr[0] * t0.second + Barycentr[1] * t1.second + Barycentr[2] * t2.second);
  if (weight != 1.0) {
    u /= weight;
    v /= weight;
  }
}

// Möller Trumbore Algorithm 同时求 光线与三角形的交与 u，v
auto triangle::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  auto s = r.origin() - v0;
  auto s1 = cross(r.direction(), e2);
  auto s2 = cross(s, e1);
  auto D = dot(s1, e1);
  if (std::abs(D) < esp)
    return false;
  D = 1 / D;
  auto t = dot(s2, e2) * D;
  auto u = dot(s1, s) * D;
  auto v = dot(s2, r.direction()) * D;

  if (t < ray_t.min || t > ray_t.max || u < esp || v < esp || 1 - u - v < esp)
    return false;

  rec.t = t;
  rec.p = r.at(t);
  rec.set_face_normal(r, normal);
  auto Barycentr = point3(1 - u - v, u, v);
  interpolate(rec.u, rec.v, Barycentr, 1.0);
  rec.mat_ptr = mat_ptr;
  return true;
}

auto triangle::bounding_box() const -> aabb {
  return bbox;
}

[[nodiscard]] auto triangle::pdf_value(const point3 &origin, const vec3d &v) const -> double {
  hit_record rec;
  if (!this->hit(ray(origin, v), interval(0.001, infinity), rec))
    return 0;

  auto distance_squared = rec.t * rec.t * v.length_squared();
  auto cosine = fabs(dot(v, rec.normal) / v.length());
  // 三角形面积
  return distance_squared / (cosine * 0.5 * cross(e1, e2).length());
}

#endif