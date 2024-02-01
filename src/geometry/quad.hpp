#ifndef QUAD_HPP
#define QUAD_HPP

#include <utility>

#include "hittable.hpp"
#include "../vector/vec3dx4.h"

/*
  特殊的平行四边形类：
  1. Q is the lower-left corner.
  2. u: a vector representing the first side. Q + u gives one of the corners adjacent to Q;
  3. v: a vector representing the second side. Q + v gives the other corner adjacent to
  so Q opposite is Q + u + v (Q, Q + u + v 是两个对角)
  aabb::pad() 是给四边形一点点厚度
  hit 判断方法是 hit 三角形判断的拓展
*/
class quad : public hittable {
public:
  point3 Q;
  vec3d u, v;
  material *mat;
  aabb bbox;
  // 用来表示这个四边形所在的平面  Ax + By + Cz = D;
  vec3d normal;
  double D;
  vec3d w;
  double area;

public:
  quad(point3 _Q, vec3d _u, vec3d _v, material *m)
      : Q(std::move(_Q)), u(std::move(_u)), v(std::move(_v)), mat(m) {
    auto n = cross(u, v);
    normal = unit_vector(n);
    D = dot(normal, Q);
    w = n / dot(n, n);
    area = n.length();
    set_bounding_box();
  }

  virtual void set_bounding_box() {
    bbox = aabb(Q, Q + u + v).pad();
  }

  [[nodiscard]] auto bounding_box() const -> aabb override {
    return bbox;
  }
  /*
    1. finding the plane that contains that quad,
    2. solving for the intersection of a ray and the quad-containing plane,
    3. determining if the hit point lies inside the quad.
  */
  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override {
    auto denom = dot(normal, r.direction());

    // No hit if the ray is parallel to the plane.
    if (fabs(denom) < 1e-8)
      return false;

    // Return false if the hit point parameter t is outside the ray interval.
    auto t = (D - dot(normal, r.origin())) / denom;
    if (!ray_t.contains(t))
      return false;

    // Determine the hit point lies within the planar shape using its plane coordinates.
    auto intersection = r.at(t);
    vec3d planar_hitpt_vector = intersection - Q;
    auto alpha = dot(w, cross(planar_hitpt_vector, v));
    auto beta = dot(w, cross(u, planar_hitpt_vector));

    if (!is_interior(alpha, beta, rec))
      return false;

    // Ray hits the 2D shape; set the rest of the hit record and return true.

    rec.t = t;
    rec.p = intersection;
    rec.mat_ptr = mat;
    rec.set_face_normal(r, normal);

    return true;
  }
  virtual auto is_interior(double a, double b, hit_record &rec) const -> bool {
    // Given the hit point in plane coordinates, return false if it is outside the
    // primitive, otherwise set the hit record UV coordinates and return true.

    if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
      return false;

    rec.u = a;
    rec.v = b;
    return true;
  }
  [[nodiscard]] auto pdf_value(const point3 &origin, const vec3d &v) const -> double override {
    hit_record rec;
    if (!this->hit(ray(origin, v), interval(0.001, infinity), rec))
      return 0;

    auto distance_squared = rec.t * rec.t * v.length_squared();
    auto cosine = fabs(dot(v, rec.normal) / v.length());

    return distance_squared / (cosine * area);
  }

  [[nodiscard]] auto random(const point3 &origin) const -> vec3d override {
    auto p = Q + (random_double() * u) + (random_double() * v);
    return p - origin;
  }
  friend auto operator<<(std::ostream &os, const quad &m) -> std::ostream & {
    os << "[Quad]| Q : " << m.Q << " u :" << m.u << " v: " << m.v << "\n";
    return os;
  }
};

#endif
