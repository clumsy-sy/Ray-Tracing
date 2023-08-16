#ifndef SOLID_COLOR_HPP
#define SOLID_COLOR_HPP

#include "texture.hpp"
// 固定颜色
class solid_color : public texture {
private:
  color color_value;

public:
  solid_color() = default;
  solid_color(color c) : color_value(std::move(c)) {}
  solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

  [[nodiscard]] auto value(double, double, const vec3d &) const -> color override {
    return color_value;
  }
};

#endif