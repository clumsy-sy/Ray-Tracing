/**
 * @file hittablelist.hpp
 * @brief 可命中对象的向量
 */
#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"
#include "interval.hpp"
#include <memory>
/**
 * @class hittable_list
 * @brief 可命中对象的向量
 * 成员：std::vector<std::unique_ptr<hittable>>, aabb
 */
class hittable_list : public hittable {
public:
  std::vector<std::unique_ptr<hittable>> objects;
  aabb bbox;

public:
  hittable_list() = default;
  hittable_list(std::unique_ptr<hittable> object) {
    add(std::move(object));
  }

  void clear() {
    objects.clear();
  }
  void add(std::unique_ptr<hittable> object) {
    bbox = aabb(bbox, object->bounding_box());
    objects.emplace_back(std::move(object));
  }

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override {
    return bbox;
  };

  [[nodiscard]] auto pdf_value(const point3 &o, const vec3d &v) const -> double override {
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
      sum += weight * object->pdf_value(o, v);

    return sum;
  }

  [[nodiscard]] auto random(const vec3d &o) const -> vec3d override {
    auto int_size = static_cast<int>(objects.size());
    return objects[(uint32_t)random_int(0, int_size - 1)()]->random(o);
  }
  auto print(std::ostream& os, const std::string& prefix = "") const -> void override {
    os << prefix << "[hit_list]: " << objects.size() << "\n";
    auto now_prefix = prefix + "  |-";
    for (auto const &t : objects) {
      t->print(os, now_prefix);
      // os << '\n';
    }
  }
  friend auto operator<<(std::ostream &os, const hittable_list &m) -> std::ostream & {
    os << "[hit_list]: " << m.objects.size() << "\n";
    auto prefix = "  |-";
    for (auto const &t : m.objects) {
      t->print(os, prefix);
      os << '\n';
    }
    return os;
  }
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

#endif