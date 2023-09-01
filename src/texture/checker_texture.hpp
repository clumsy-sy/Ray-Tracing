#ifndef CHECKER_TEXTURE_HPP
#define CHECKER_TEXTURE_HPP

#include "texture.hpp"
#include "solid_color.hpp"

class checker_texture : public texture {
public:
  texture* odd;
  texture* even;

public:
  checker_texture() = default;

  checker_texture(texture* _even, texture* _odd)
      : odd(_odd), even(_even) {}

  checker_texture(color c1, color c2)
      : odd(new solid_color(c2)), even(new solid_color(c1)) {}

  [[nodiscard]] auto value(double u, double v, const point3 &p) const -> color override {
    // 通过正弦余弦来交替
    auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0)
      return odd->value(u, v, p);
    else
      return even->value(u, v, p);
  }
};
#endif