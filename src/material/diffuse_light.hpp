#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

#include "material.hpp"
#include "../texture/texture.hpp"
#include "../texture/solid_color.hpp"

class diffuse_light : public material {
public:
  texture* emit;

public:
  diffuse_light(texture* a) : emit(a) {}
  diffuse_light(color c) : emit(new solid_color(c)) {}

  auto scatter([[maybe_unused]] const ray &r_in, [[maybe_unused]] const hit_record &rec,
      [[maybe_unused]] scatter_record &srec) const -> bool override {
    return false;
  }

  [[nodiscard]] auto emitted([[maybe_unused]] const ray &r_in, [[maybe_unused]] const hit_record &rec, double u,
      double v, [[maybe_unused]] const point3 &p) const -> color override {
    if (!rec.front_face)
      return {0, 0, 0};
    return emit->value(u, v, p);
  }
};

#endif