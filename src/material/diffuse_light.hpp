#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

#include "material.hpp"
#include "../texture/texture.hpp"
#include "../texture/solid_color.hpp"

class diffuse_light : public material {
public:
  std::shared_ptr<texture> emit;

public:
  diffuse_light(std::shared_ptr<texture> a) : emit(std::move(a)) {}
  diffuse_light(color c) : emit(std::make_shared<solid_color>(c)) {}

  auto scatter(const ray &, const hit_record &, color &, ray &) const -> bool override {
    return false;
  }

  [[nodiscard]] auto emitted(double u, double v, const point3 &p) const -> color override {
    return emit->value(u, v, p);
  }
};

#endif