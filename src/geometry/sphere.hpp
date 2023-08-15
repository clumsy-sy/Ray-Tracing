#ifndef SPHERE_H
#define SPHERE_H

#include "../global.hpp"
#include "hittable.hpp"

class sphere : public hittable {

public:
  point3 center;
  double radius, radius2;
  std::shared_ptr<material> mat_ptr;
  aabb bbox;

private:
  static void get_sphere_uv(const point3 &p, double &u, double &v) {
    /*
      纹理坐标 $(u, v) \in [0, 1]$ 需要被映射到球面上，使用极坐标就是 $(\theta, \phi)$
      u = \phi / 2 * PI , v = \theta / PI
      由于圆是 (x, y, z) 形式存储，所以需要从笛卡尔坐标系转为极坐标系
      https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures/texturecoordinatesforspheres
    */
    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + PI;

    u = phi / (2 * PI);
    v = theta / PI;
  }

public:
  sphere() = default;
  sphere(point3 c, double r, std::shared_ptr<material> m)
      : center(std::move(c)), radius(r), radius2(r * r), mat_ptr(std::move(m)) {
    // 圆的 AABB 就是(圆心 - r)三个方向 和 （圆心 + r）三个方向
    bbox = aabb(center - Vec3d(radius, radius, radius), center + Vec3d(radius, radius, radius));
  };

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override;
};

auto sphere::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  Vec3d oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius2;
  // Find the nearest root that lies in the acceptable range.
  double x0, x1, root;
  if (solveQuadratic_halfb(a, half_b, c, x0, x1)) {
    if (ray_t.surrounds(x0))
      root = x0;
    else if (ray_t.surrounds(x1))
      root = x1;
    else
      return false;
  } else
    return false;
  rec.t = root;
  rec.p = r.at(rec.t);
  Vec3d &&outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  get_sphere_uv(outward_normal, rec.u, rec.v);
  rec.mat_ptr = mat_ptr;

  return true;
}

auto sphere::bounding_box() const -> aabb {
  return bbox;
}

#endif