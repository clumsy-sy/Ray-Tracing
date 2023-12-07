#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../geometry/ray.hpp"
#include "pdf.hpp"
struct hit_record;

class scatter_record {
public:
  color attenuation;
  pdf *pdf_ptr;
  bool skip_pdf;
  ray skip_pdf_ray;
};
/*
  base class: attenuation(颜色衰减) & scattered（散射光）& emited (光源发射)
*/
class material {
public:
  virtual auto scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const -> bool = 0;
  [[nodiscard]] virtual auto emitted([[maybe_unused]] const ray &r_in, [[maybe_unused]] const hit_record &rec,
      [[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const point3 &p) const -> color {
    return {0, 0, 0};
  }
  [[nodiscard]] virtual auto scattering_pdf([[maybe_unused]] const ray &r_in, [[maybe_unused]] const hit_record &rec,
      [[maybe_unused]] const ray &scattered) const -> double {
    return 0;
  }
};

#endif