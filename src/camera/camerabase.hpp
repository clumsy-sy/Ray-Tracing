#ifndef CAMERABASE_HPP
#define CAMERABASE_HPP

#include "../vector/vec3dx4.h"
#include "../geometry/ray.hpp"

class camerabase {
public:
  point3 origin = point3(0, 0, 0);
  point3 lower_left_corner;
  vec3d horizontal, vertical;

public:
  camerabase() {
    auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin = point3(0, 0, 0);
    horizontal = vec3d(viewport_width, 0.0, 0.0);
    vertical = vec3d(0.0, viewport_height, 0.0);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3d(0, 0, focal_length);
  }
  auto operator=(const camerabase &c) -> camerabase & = default;

  [[nodiscard]] virtual auto get_ray(double u, double v) const -> ray {
    return {origin, lower_left_corner + u * horizontal + v * vertical - origin};
  }
};
#endif