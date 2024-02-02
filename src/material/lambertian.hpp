/**
 * @file lambertian.hpp
 * @brief 兰伯特模型
 * 
 */
#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"
#include "../texture/texture.hpp"
#include "../texture/solid_color.hpp"
#include "../geometry/hittable.hpp"

/**
 * @class lambertian
 * @brief 兰伯特类
 * 成员：texture *albedo; // 以某种概率分布衰减，
 */
class lambertian : public material {
public:
  texture *albedo; // 以某种概率分布衰减，albedo / p

public:
  lambertian(const color &a) : albedo(new solid_color(a)) {}
  lambertian(texture *a) : albedo(a) {}

  auto scatter([[maybe_unused]] const ray &r_in, const hit_record &rec,
      scatter_record &srec) const -> bool override {
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = new cosine_pdf(rec.normal);
    srec.skip_pdf = false;
    return true;
  }
  // pdf = cos(\theta) / PI, theta = scatter_dir \dots suf_normal
  [[nodiscard]] auto scattering_pdf([[maybe_unused]] const ray &r_in, const hit_record &rec,
      const ray &scattered) const -> double override {
    // std::cout << "use lambertian scattering_pdf" << std::endl;
    auto cosine = dot(rec.normal, scattered.direction().unit_vector());
    return cosine < 0 ? 0 : cosine / PI;
  }
};

#endif