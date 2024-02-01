#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "material.hpp"
#include "../vector/vec3dx4.h"
#include "../geometry/hittable.hpp"

class dielectric : public material {
public:
  double ir; // 折射率

public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  auto scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const
      -> bool override {
    srec.attenuation = color(1.0, 1.0, 1.0);
    srec.pdf_ptr = nullptr;
    srec.skip_pdf = true;
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3d unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3d direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, refraction_ratio);

    srec.skip_pdf_ray = ray(rec.p, direction);
    return true;
  }

private:
  static auto reflectance(double cosine, double ref_idx) -> double {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

#endif