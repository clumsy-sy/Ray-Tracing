#ifndef METAL_HPP
#define METAL_HPP

#include "material.hpp"
#include "../vector/Vec3dx4.hpp"
#include "../geometry/hittable.hpp"

class metal : public material {
public:
  color albedo;
  double fuzz; // 模糊参数 0 为不扰动

public:
  metal(color a) : albedo(std::move(a)) {
    fuzz = 0;
  }
  metal(color a, double f) : albedo(std::move(a)), fuzz(f < 1 ? f : 1) {}

  auto scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const -> bool override {
    Vec3d reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }
};

#endif