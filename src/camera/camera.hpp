#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "camerabase.hpp"

/*
  camerabase {
    point3 origin ;
    point3 lower_left_corner;
    vec3d horizontal, vertical;
  }
  camera 是一个有透镜的摄像机
*/

class camera : public camerabase {
public:
  vec3d u, v, w;      // w:看向方向，u：镜头平面的 x，v：镜头屏幕的 y
  double lens_radius; // 镜头半径

public:
  camera() = default;
  camera(const point3 &lookfrom, const point3 &lookat, const vec3d &vup, double vfov,
      double aspect_ratio, double aperture) {
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto focus_dist = (lookfrom - lookat).length();
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

    lens_radius = aperture / 2;
#ifdef LOG
    printf("[Cam LOG] : lookfrom[x: %5.3lf  y: %5.3lf  z: %5.3lf]\
  -> lookat[x: %5.3lf  y: %5.3lf  z: %5.3lf] vup[x: %5.3lf  y: %5.3lf  z: %5.3lf]\n",
        lookfrom[0], lookfrom[1], lookfrom[2], lookat[0], lookat[1], lookat[2], vup[0], vup[1],
        vup[2]);
    printf("          | lookdir[x: %5.3lf  y: %5.3lf  z: %5.3lf],\
  xdir[x: %5.3lf  y: %5.3lf  z: %5.3lf], ydir[x: %5.3lf  y: %5.3lf  z: %5.3lf]\n",
        w[0], w[1], w[2], u[0], u[1], u[2], v[0], v[1], v[2]);
    printf("          | horizontal[x: %5.3lf  y: %5.3lf  z: %5.3lf],\
  vertical[x: %5.3lf  y: %5.3lf  z: %5.3lf], lower_left_corner[x: %5.3lf  y: %5.3lf  z: %5.3lf]\n",
        horizontal[0], horizontal[1], horizontal[2], vertical[0], vertical[1], vertical[2],
        lower_left_corner[0], lower_left_corner[1], lower_left_corner[2]);
    printf("          | vfov: %5.3lf, theta: %5.3lf,h: %5.3lf, viewport_height: %5.3lf, "
           "viewport_width: % 5.3f\n",
        vfov, theta, h, viewport_height, viewport_width);
    printf("--------------------------------------------------\n");
#endif
  }
  [[nodiscard]] auto get_ray(double s, double t) const -> ray override {
    vec3d rd = lens_radius * random_in_unit_disk();
    vec3d offset = u * rd.x() + v * rd.y(); // 镜头的光圈中随机一个点

    return {
        origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset};
  }
};
#endif