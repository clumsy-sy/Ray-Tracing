#ifndef AARECT_HPP
#define AARECT_HPP

#include "hittable.hpp"
#include "interval.hpp"
// 在 xy 屏幕上的矩形，z 固定
class xy_rect : public hittable {
public:
  std::shared_ptr<material> mp;
  double x0, x1, y0, y1, k;

public:
  xy_rect() = default;

  xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<material> mat)
      : mp(std::move(mat)), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k){};

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount.
    return {point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001)};
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
  auto outward_normal = Vec3d(0, 0, 1);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}

class xz_rect : public hittable {
public:
  std::shared_ptr<material> mp;
  double x0, x1, z0, z1, k;

public:
  xz_rect() = default;

  xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<material> mat)
      : mp(std::move(mat)), x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k){};

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount.
    return {point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1)};
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
  auto outward_normal = Vec3d(0, 1, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}

class yz_rect : public hittable {
public:
  std::shared_ptr<material> mp;
  double y0, y1, z0, z1, k;

public:
  yz_rect() = default;

  yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<material> mat)
      : mp(std::move(mat)), y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k){};

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    return {point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1)};
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
  auto outward_normal = Vec3d(1, 0, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}

#endif