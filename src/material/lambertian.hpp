#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"
#include "../texture/texture.hpp"
#include "../texture/solid_color.hpp"
#include "../geometry/hittable.hpp"

class lambertian : public material {
public:
  texture *albedo; // 以某种概率分布衰减，albedo / p

public:
  lambertian(const color &a) : albedo(new solid_color(a)) {}
  lambertian(texture *a) : albedo(a) {}

  auto scatter([[maybe_unused]] const ray &r_in, const hit_record &rec, scatter_record &srec) const -> bool override {
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal);
    srec.skip_pdf = false;
    return true;
  }
  [[nodiscard]] auto scattering_pdf([[maybe_unused]] const ray &r_in, const hit_record &rec, const ray &scattered) const
      -> double override {
    auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
    return cosine < 0 ? 0 : cosine / PI;
  }
};

#endif