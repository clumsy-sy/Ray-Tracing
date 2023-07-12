#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"

class hittable_list : public hittable {
public:
  std::vector<std::shared_ptr<hittable>> objects;

public:
  hittable_list() = default;
  hittable_list(const std::shared_ptr<hittable> &object) {
    add(object);
  }

  void clear() {
    objects.clear();
  }
  void add(const std::shared_ptr<hittable> &object) {
    // objects.push_back(object);
    objects.emplace_back(object);
  }

  auto hit(const ray &r, double t_min, double t_max, hit_record &rec) const -> bool override;
  auto bounding_box(aabb &output_box) const -> bool override;
};

auto hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const -> bool {
  hit_record temp_rec;
  bool hit_anything = false;
  auto closest_so_far = t_max;
  // 遍历列表，查找与光线交的物体，更新最近的交点和 tmax
  for (const auto &object : objects) {
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

auto hittable_list::bounding_box(aabb &output_box) const -> bool {
  // 遍历数组，求整体的 AABB（不断更新）
  if (objects.empty())
    return false;

  aabb temp_box;
  bool first_box = true;

  for (const auto &object : objects) {
    if (!object->bounding_box(temp_box))
      return false;
    output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
    first_box = false;
  }

  return true;
}
#endif