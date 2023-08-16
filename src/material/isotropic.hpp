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

  auto scatter([[maybe_unused]] const ray &r_in, const hit_record &rec, scatter_record &srec) const -> bool override {
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = std::make_shared<sphere_pdf>();
    srec.skip_pdf = false;
    return true;
  }
  [[nodiscard]] auto scattering_pdf([[maybe_unused]] const ray &r_in, [[maybe_unused]] const hit_record &rec,
      [[maybe_unused]] const ray &scattered) const -> double override {
    return 1 / (4 * PI);
  }
};

#endif