/**
 * @file ray.hpp
 * @brief ray 类，由原点和一个方向确定
 *
 */
#ifndef RAY_HPP
#define RAY_HPP

#include "../vector/vec3dx4.h"

/**
 * @class ray
 * @brief 光线 ray {origin, direction} 
 */
class ray {
private:
  point3 orig;
  vec3d dir;

public:
  ray() = default;
  ray(point3 origin, vec3d direction) : orig(std::move(origin)), dir(std::move(direction)) {}

  [[nodiscard]] inline auto origin() const -> point3 {
    return orig;
  }
  [[nodiscard]] inline auto direction() const -> vec3d {
    return dir;
  }
  [[nodiscard]] inline auto at(double t) const -> point3 {
    return orig + t * dir;
  }
};

#endif