#ifndef ONB_HPP
#define ONB_HPP

#include "../global.hpp"
#include "../vector/vec3dx4.h"

class onb {
public:
  vec3d axis[3];

public:
  onb() = default;

  auto operator[](int i) const -> vec3d {
    return axis[i];
  }
  auto operator[](int i) -> vec3d & {
    return axis[i];
  }

  [[nodiscard]] auto u() const -> vec3d {
    return axis[0];
  }
  [[nodiscard]] auto v() const -> vec3d {
    return axis[1];
  }
  [[nodiscard]] auto w() const -> vec3d {
    return axis[2];
  }

  [[nodiscard]] auto local(double a, double b, double c) const -> vec3d {
    return a * u() + b * v() + c * w();
  }

  [[nodiscard]] auto local(const vec3d &a) const -> vec3d {
    return a.x() * u() + a.y() * v() + a.z() * w();
  }

  void build_from_w(const vec3d &w) {
    vec3d unit_w = unit_vector(w);
    vec3d a = (fabs(unit_w.x()) > 0.9) ? vec3d(0, 1, 0) : vec3d(1, 0, 0);
    vec3d v = unit_vector(cross(unit_w, a));
    vec3d u = cross(unit_w, v);
    axis[0] = u;
    axis[1] = v;
    axis[2] = unit_w;
  }
};

#endif