#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"
#include "interval.hpp"

class hittable_list : public hittable {
public:
  std::vector<std::shared_ptr<hittable>> objects;
  aabb bbox;

public:
  hittable_list() = default;
  hittable_list(const std::shared_ptr<hittable> &object) {
    add(object);
  }

  void clear() {
    objects.clear();
  }
  void add(const std::shared_ptr<hittable> &object) {
    objects.emplace_back(object);
    bbox = aabb(bbox, object->bounding_box());
  }

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override;
};

auto hittable_list::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  hit_record temp_rec;
  bool hit_anything = false;
  auto closest_so_far = ray_t.max;
  // 遍历列表，查找与光线交的物体，更新最近的交点和 tmax
  for (const auto &object : objects) {
    if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

auto hittable_list::bounding_box() const -> aabb {
  return bbox;
}
#endif