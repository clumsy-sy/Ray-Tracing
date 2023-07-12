#ifndef CAMERAFOV_HPP
#define CAMERAFOV_HPP

#include "camerabase.hpp"

class cameraFOV : public camerabase {
public:
  cameraFOV(double vfov, // vertical field-of-view in degrees
      double aspect_ratio) {
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    auto focal_length = 1.0;

    origin = point3(0, 0, 0);
    horizontal = Vec3d(viewport_width, 0.0, 0.0);
    vertical = Vec3d(0.0, viewport_height, 0.0);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3d(0, 0, focal_length);
  }

  [[nodiscard]] auto get_ray(double u, double v) const -> ray override {
    return {origin, lower_left_corner + u * horizontal + v * vertical - origin};
  }
};

#endif