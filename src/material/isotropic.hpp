#ifndef ISOTROPIC_HPP
#define ISOTROPIC_HPP

#include "../geometry/hittable.hpp"
#include "../texture/texture.hpp"
#include "../texture/solid_color.hpp"
#include "material.hpp"

// 随机方向散射材料
class isotropic : public material {
public:
  std::shared_ptr<texture> albedo;

public:
  isotropic(color c) : albedo(std::make_shared<solid_color>(c)) {}
  isotropic(std::shared_ptr<texture> a) : albedo(std::move(a)) {}

  auto scatter(const ray &, const hit_record &rec, color &attenuation, ray &scattered) const -> bool override {
    scattered = ray(rec.p, random_in_unit_sphere());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }
};

#endif