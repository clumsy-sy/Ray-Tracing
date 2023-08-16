#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../vector/vec3dx4.h"

class texture {
public:
  [[nodiscard]] virtual auto value(double u, double v, const point3 &p) const -> color = 0;
};

#endif