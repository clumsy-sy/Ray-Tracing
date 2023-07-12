#ifndef RAY_HPP
#define RAY_HPP

#include "../vector/Vec3dx4.hpp"
class ray {
public:
  point3 orig;
  Vec3d dir;

public:
  ray() = default;
  ray(point3 origin, Vec3d direction) : orig(std::move(origin)), dir(std::move(direction)) {}

  [[nodiscard]] auto origin() const -> point3 {
    return orig;
  }
  [[nodiscard]] auto direction() const -> Vec3d {
    return dir;
  }
  [[nodiscard]] auto at(double t) const -> point3 {
    return orig + t * dir;
  }
};

#endif