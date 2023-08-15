#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "../vector/Vec3dx4.hpp"
#include "AABB.hpp"
#include "interval.hpp"
#include "ray.hpp"

class material;
/*
  光线与物体交的记录
*/
struct hit_record {
  point3 p;                          // 交点坐标
  Vec3d normal;                      // 法线
  std::shared_ptr<material> mat_ptr; // 材质
  double t;                          // 光线的 t
  double u, v;                       // 材质
  bool front_face;                   // 朝向

  inline void set_face_normal(const ray &r, const Vec3d &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool = 0;
  [[nodiscard]] virtual auto bounding_box() const -> aabb = 0;
  virtual ~hittable() = default;
};

#endif