#ifndef CAMERAORIENT_HPP
#define CAMERAORIENT_HPP

#include "camerabase.hpp"

class cameraOrient : public camerabase {
public:
  cameraOrient(const point3 &lookfrom, const point3 &lookat, const vec3d &vup, double vfov,
      double aspect_ratio) {
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    auto w = unit_vector(lookfrom - lookat);
    auto u = unit_vector(cross(vup, w));
    auto v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
  }

  [[nodiscard]] auto get_ray(double s, double t) const -> ray override {
    return {origin, lower_left_corner + s * horizontal + t * vertical - origin};
  }
};

#endif