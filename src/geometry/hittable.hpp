/**
 * @file hittable.hpp
 * @brief 可与光线交互的对象
 */
#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "../vector/vec3dx4.h"
#include "AABB.hpp"
#include "interval.hpp"
#include "ray.hpp"

class material;
/**
 * @class hit_record
 * @brief 光线与对象相交的记录
 * 记录的信息：交点、交点法线、交点材质、光线在相交时的 t，交点对应的纹理、法线朝向
 */
struct hit_record {
  point3 p;                    // 交点坐标
  vec3d normal;                // 法线
  material *mat_ptr = nullptr; // 材质
  double t;                    // 光线的 t
  double u, v;                 // 材质
  bool front_face;             // 朝向

  inline void set_face_normal(const ray &r, const vec3d &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};
/**
 * @class hittable
 * @brief 可与光线交互的对象
 * 虚函数：hit()求交, pdf_value()概率密度函数, random()随机
 */
class hittable {
public:
  virtual auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool = 0;
  [[nodiscard]] virtual auto bounding_box() const -> aabb = 0;
  [[nodiscard]] virtual auto pdf_value(
      [[maybe_unused]] const point3 &o, [[maybe_unused]] const vec3d &v) const -> double {
    return 0.0;
  }
  [[nodiscard]] virtual auto random([[maybe_unused]] const vec3d &o) const -> vec3d {
    return {1, 0, 0};
  }
  virtual auto print(std::ostream &os, const std::string &prefix = "") const -> void = 0;
  virtual ~hittable() = default;
};

auto operator<<(std::ostream &os, const hittable &obj) -> std::ostream & {
  obj.print(os);
  return os;
}

#endif