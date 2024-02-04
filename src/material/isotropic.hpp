#ifndef ISOTROPIC_HPP
#define ISOTROPIC_HPP

#include "../geometry/hittable.hpp"
#include "../texture/texture.hpp"
#include "../texture/solid_color.hpp"
#include "material.hpp"
#include <memory>

// 随机方向散射材料
class isotropic : public material {
public:
  texture *albedo;

public:
  isotropic(color c) : albedo(new solid_color(c)) {}
  isotropic(texture *a) : albedo(a) {}

  auto scatter([[maybe_unused]] const ray &r_in, const hit_record &rec,
      scatter_record &srec) const -> bool override {
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    if(!srec.pdf_ptr)
      srec.pdf_ptr = std::make_shared<sphere_pdf>();
    if (srec.pdf_ptr == nullptr) {
      std::cout << "NULL" << std::endl;
    }
    srec.skip_pdf = false;
    return true;
  }
  [[nodiscard]] auto scattering_pdf([[maybe_unused]] const ray &r_in,
      [[maybe_unused]] const hit_record &rec, [[maybe_unused]] const ray &scattered) const
      -> double override {
    return 1 / (4 * PI);
  }
};

#endif