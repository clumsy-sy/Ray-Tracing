#ifndef AARECT_HPP
#define AARECT_HPP

#include "hittable.hpp"
#include "interval.hpp"
// 在 xy 屏幕上的矩形，z 固定
class xy_rect : public hittable {
public:
  material *mp;
  double x0, x1, y0, y1, k;

public:
  xy_rect() = default;

  xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, material *mat)
      : mp(mat), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k){};

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount.
    return {point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001)};
  }
  [[nodiscard]] auto random(const point3 &origin) const -> vec3d override {
    auto random_point = point3(random_double(x0, x1)(), k, random_double(y0, y1)());
    return random_point - origin;
  }
  friend auto operator<<(std::ostream &os, const xy_rect &m) -> std::ostream & {
    os << "[xy_rect]| x0 - x1 : " << m.x0 << " ~ " << m.x1 << "  y0 - y1 : " << m.y0 << " ~ "
       << m.y1 << " z : " << m.k << "\n";
    return os;
  }
};

auto xy_rect::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  auto t = (k - r.origin().z()) / r.direction().z();
  if (ray_t.outside(t))
    return false;
  auto x = r.origin().x() + t * r.direction().x();
  auto y = r.origin().y() + t * r.direction().y();
  if (x < x0 || x > x1 || y < y0 || y > y1)
    return false;
  rec.u = (x - x0) / (x1 - x0);
  rec.v = (y - y0) / (y1 - y0);
  rec.t = t;
  auto outward_normal = vec3d(0, 0, 1);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}

class xz_rect : public hittable {
public:
  material *mp;
  double x0, x1, z0, z1, k;

public:
  xz_rect() = default;

  xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, material *mat)
      : mp(mat), x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k){};

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount.
    return {point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1)};
  }
  [[nodiscard]] auto random(const point3 &origin) const -> vec3d override {
    auto random_point = point3(random_double(x0, x1)(), k, random_double(z0, z1)());
    return random_point - origin;
  }
  friend auto operator<<(std::ostream &os, const xz_rect &m) -> std::ostream & {
    os << "[xz_rect]| x0 - x1 : " << m.x0 << " ~ " << m.x1 << "  z0 - z1 : " << m.z0 << " ~ "
       << m.z1 << " y : " << m.k << "\n";
    return os;
  }
};

auto xz_rect::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  auto t = (k - r.origin().y()) / r.direction().y();
  if (ray_t.outside(t))
    return false;
  auto x = r.origin().x() + t * r.direction().x();
  auto z = r.origin().z() + t * r.direction().z();
  if (x < x0 || x > x1 || z < z0 || z > z1)
    return false;
  rec.u = (x - x0) / (x1 - x0);
  rec.v = (z - z0) / (z1 - z0);
  rec.t = t;
  auto outward_normal = vec3d(0, 1, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}

class yz_rect : public hittable {
public:
  material *mp;
  double y0, y1, z0, z1, k;

public:
  yz_rect() = default;

  yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, material *mat)
      : mp(mat), y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k){};

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    return {point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1)};
  }
  [[nodiscard]] auto random(const point3 &origin) const -> vec3d override {
    auto random_point = point3(random_double(y0, y1)(), k, random_double(z0, z1)());
    return random_point - origin;
  }
  friend auto operator<<(std::ostream &os, const yz_rect &m) -> std::ostream & {
    os << "[yz_rect]| y0 - y1 : " << m.y0 << " ~ " << m.y1 << "  z0 - z1 : " << m.z0 << " ~ "
       << m.z1 << " x : " << m.k << "\n";
    return os;
  }
};

auto yz_rect::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  auto t = (k - r.origin().x()) / r.direction().x();
  if (ray_t.outside(t))
    return false;
  auto y = r.origin().y() + t * r.direction().y();
  auto z = r.origin().z() + t * r.direction().z();
  if (y < y0 || y > y1 || z < z0 || z > z1)
    return false;
  rec.u = (y - y0) / (y1 - y0);
  rec.v = (z - z0) / (z1 - z0);
  rec.t = t;
  auto outward_normal = vec3d(1, 0, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}

#endif