#ifndef ONB_HPP
#define ONB_HPP

#include "../global.hpp"
#include "../vector/Vec3dx4.hpp"

class onb {
public:
  Vec3d axis[3];

public:
  onb() = default;

  auto operator[](int i) const -> Vec3d {
    return axis[i];
  }
  auto operator[](int i) -> Vec3d & {
    return axis[i];
  }

  [[nodiscard]] auto u() const -> Vec3d {
    return axis[0];
  }
  [[nodiscard]] auto v() const -> Vec3d {
    return axis[1];
  }
  [[nodiscard]] auto w() const -> Vec3d {
    return axis[2];
  }

  [[nodiscard]] auto local(double a, double b, double c) const -> Vec3d {
    return a * u() + b * v() + c * w();
  }

  [[nodiscard]] auto local(const Vec3d &a) const -> Vec3d {
    return a.x() * u() + a.y() * v() + a.z() * w();
  }

  void build_from_w(const Vec3d &w) {
    Vec3d unit_w = unit_vector(w);
    Vec3d a = (fabs(unit_w.x()) > 0.9) ? Vec3d(0, 1, 0) : Vec3d(1, 0, 0);
    Vec3d v = unit_vector(cross(unit_w, a));
    Vec3d u = cross(unit_w, v);
    axis[0] = u;
    axis[1] = v;
    axis[2] = unit_w;
  }
};

#endif