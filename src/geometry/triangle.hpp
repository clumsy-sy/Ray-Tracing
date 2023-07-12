#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "hittable.hpp"

class triangle : public hittable {
public:
  /*
    齐次坐标下三角形的信息
    1. 三个点的坐标 V0，V1，V2，1.0 顺时针
    2. 三个点的颜色信息（R，G，B）
    3. 三个点的材质信息 （U，V）
    4. 三个点的法向量信息（Vector3f）
  */
  Vec3d v0, v1, v2; // vertices A, B ,C , counter-clockwise order
  Vec3d e1, e2;     // 2 edges v1-v0, v2-v0; 与光线交有用
  Vec3d t0, t1, t2; // texture coords
  Vec3d normal;
  std::shared_ptr<material> mat_ptr;

public:
  triangle() = default;
  triangle(Vec3d _v0, Vec3d _v1, Vec3d _v2, std::shared_ptr<material> m)
      : v0(std::move(_v0)), v1(std::move(_v1)), v2(std::move(_v2)), mat_ptr(std::move(m)) {
    e1 = v1 - v0;
    e2 = v2 - v0;
    normal = unit_vector(cross(e1, e2));
  }
  ~triangle() override = default;
  auto hit(const ray &r, double t_min, double t_max, hit_record &rec) const -> bool override;
  auto bounding_box(aabb &output_box) const -> bool override;

  friend auto operator<<(std::ostream &os, const triangle &t) -> std::ostream & {
    return os << "v0 : " << t.v0 << ", v1 " << t.v1 << ", v2 " << t.v2;
  }
};
// Möller Trumbore Algorithm 同时求 光线与三角形的交与 u，v
auto triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const -> bool {
  auto s = r.orig - v0;
  auto s1 = cross(r.dir, e2);
  auto s2 = cross(s, e1);
  auto D = dot(s1, e1);
  if(std::abs(D) < esp) return false;
  D = 1 / D;
  auto t = dot(s2, e2) * D;
  auto u = dot(s1, s) * D;
  auto v = dot(s2, r.dir) * D;

  if (t < t_min || t > t_max || u < esp || v < esp || 1-u-v < esp)
    return false;

  rec.t = t;
  rec.p = r.at(t);
  rec.set_face_normal(r, normal);
  rec.u = u;
  rec.v = v;
  rec.mat_ptr = mat_ptr;
  return true;
}

auto triangle::bounding_box(aabb &output_box) const -> bool {
  output_box = surrounding_box(surrounding_box(v0, v1), v2);
  return true;
}

#endif