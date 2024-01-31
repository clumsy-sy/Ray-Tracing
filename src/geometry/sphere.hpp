#ifndef SPHERE_H
#define SPHERE_H

#include "../global.hpp"
#include "ONB.hpp"
#include "hittable.hpp"

class sphere : public hittable {

public:
  point3 center;
  double radius, radius2;
  material *mat_ptr;
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
  sphere(point3 c, double r, material *m) : center(std::move(c)), radius(r), radius2(r * r), mat_ptr(m) {
    // 圆的 AABB 就是(圆心 - r)三个方向 和 （圆心 + r）三个方向
    bbox = aabb(center - vec3d(radius, radius, radius), center + vec3d(radius, radius, radius));
  };

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override;

  [[nodiscard]] auto pdf_value(const point3 &o, const vec3d &v) const -> double override {

    hit_record rec;
    if (!this->hit(ray(o, v), interval(0.001, infinity), rec))
      return 0;

    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).length_squared());
    auto solid_angle = 2 * PI * (1 - cos_theta_max);

    return 1 / solid_angle;
  }

  [[nodiscard]] auto random(const point3 &o) const -> vec3d override {
    vec3d direction = center - o;
    auto distance_squared = direction.length_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
  }
  friend auto operator<<(std::ostream &os, const sphere &m) -> std::ostream & {
    os << "[Sphere]| center : " << m.center << " radius :" << m.radius << "\n";
    return os;
  }
};

auto sphere::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  vec3d oc = r.origin() - center;
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
  vec3d &&outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  get_sphere_uv(outward_normal, rec.u, rec.v);
  rec.mat_ptr = mat_ptr;

  return true;
}

auto sphere::bounding_box() const -> aabb {
  return bbox;
}

#endif