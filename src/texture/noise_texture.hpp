#ifndef NOISE_TEXTURE_HPP
#define NOISE_TEXTURE_HPP

#include "texture.hpp"
#include "perlin.hpp"

class noise_texture : public texture {
public:
  perlin noise;
  double scale;

public:
  noise_texture() = default;
  noise_texture(double sc) : scale(sc) {}

  [[nodiscard]] auto value(double, double, const point3 &p) const -> color override {
    // return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
    // return color(1, 1, 1) * noise.turb(scale * p);
    return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
  }
};

#endif