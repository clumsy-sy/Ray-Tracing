#ifndef RAY_HPP
#define RAY_HPP

#include "../vector/vec3dx4.h"
class ray {
public:
  point3 orig;
  vec3d dir;

public:
  ray() = default;
  ray(point3 origin, vec3d direction) : orig(std::move(origin)), dir(std::move(direction)) {}

  [[nodiscard]] auto origin() const -> point3 {
    return orig;
  }
  [[nodiscard]] auto direction() const -> vec3d {
    return dir;
  }
  [[nodiscard]] auto at(double t) const -> point3 {
    return orig + t * dir;
  }
};

#endif