#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "../global.hpp"
#include "hittable.hpp"
#include "../material/material.hpp"
#include "../material/isotropic.hpp"
#include "../texture/texture.hpp"
#include "interval.hpp"
#include <memory>

class constant_medium : public hittable {
public:
  std::unique_ptr<hittable> boundary; // 边界
  material *phase_function;           // 作用效果
  double neg_inv_density;

public:
  constant_medium(std::unique_ptr<hittable> b, double d, texture *a)
      : boundary(std::move(b)), phase_function(new isotropic(a)), neg_inv_density(-1 / d) {}

  constant_medium(std::unique_ptr<hittable> b, double d, color c)
      : boundary(std::move(b)), phase_function(new isotropic(c)), neg_inv_density(-1 / d) {}

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    return boundary->bounding_box();
  }
  auto print(std::ostream& os, const std::string& prefix = "") const -> void override {
    os << prefix << "[const_med]: neg_inv_density = " << neg_inv_density;
    boundary->print(os, prefix);
  }
  friend auto operator<<(std::ostream &os, const constant_medium &m) -> std::ostream & {
    os << "[const_med]: neg_inv_density = " << m.neg_inv_density;
    m.boundary->print(os);
    return os;
  }
};

auto constant_medium::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  // Print occasional samples when debugging. To enable, set enableDebug true.
  const bool enableDebug = false;
  const bool debugging = enableDebug && random_double() < 0.00001;

  hit_record rec1, rec2;

  if (!boundary->hit(r, empty, rec1))
    return false;

  if (!boundary->hit(r, interval(rec1.t + 0.0001, infinity), rec2))
    return false;

  if (debugging)
    std::cerr << "\nray_t.min=" << rec1.t << ", ray_t.max=" << rec2.t << '\n';

  if (rec1.t < ray_t.min)
    rec1.t = ray_t.min;
  if (rec2.t > ray_t.max)
    rec2.t = ray_t.max;

  if (rec1.t >= rec2.t)
    return false;

  if (rec1.t < 0)
    rec1.t = 0;

  const auto ray_length = r.direction().length();
  const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
  const auto hit_distance = neg_inv_density * log(random_double());

  if (hit_distance > distance_inside_boundary)
    return false;

  rec.t = rec1.t + hit_distance / ray_length;
  rec.p = r.at(rec.t);

  if (debugging) {
    std::cerr << "hit_distance = " << hit_distance << '\n'
              << "rec.t = " << rec.t << '\n'
              << "rec.p = " << rec.p << '\n';
  }

  rec.normal = vec3d(1, 0, 0); // arbitrary
  rec.front_face = true;       // also arbitrary
  rec.mat_ptr = phase_function;

  return true;
}

#endif