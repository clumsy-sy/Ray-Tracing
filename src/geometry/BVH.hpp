#ifndef BVH_HPP
#define BVH_HPP

#include "../global.hpp"
#include "hittablelist.hpp"
#include "interval.hpp"

inline auto box_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b, int axis) -> bool;
inline auto box_x_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b) -> bool;
inline auto box_y_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b) -> bool;
inline auto box_z_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b) -> bool;

/*
  BVH 的结点（也是可背光线击中的）
  左右儿子 + 自己的 AABB
*/
class bvh_node : public hittable {
public:
  std::shared_ptr<hittable> left;
  std::shared_ptr<hittable> right;
  aabb bbox;

public:
  bvh_node();

  bvh_node(const hittable_list &list) : bvh_node(list.objects, 0, list.objects.size()) {}

  bvh_node(const std::vector<std::shared_ptr<hittable>> &src_objects, size_t start, size_t end);

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override;
};
bvh_node::bvh_node(const std::vector<std::shared_ptr<hittable>> &src_objects, size_t start, size_t end) {
  auto objects = src_objects; // Create a modifiable array of the source scene objects
  // 随机一个轴，按这个轴排序
  int axis = random_int(0, 2)();
  auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

  size_t object_span = end - start;
  // 根据当前结点大小分类处理，1 为 叶子结点， 2 单独处理
  if (object_span == 1) {
    left = right = objects[start];
  } else if (object_span == 2) {
    if (comparator(objects[start], objects[start + 1])) {
      left = objects[start];
      right = objects[start + 1];
    } else {
      left = objects[start + 1];
      right = objects[start];
    }
  } else {
    std::sort(objects.begin() + start, objects.begin() + end, comparator);
    // 按随机的轴排序后，递归建树
    auto mid = start + object_span / 2;
    left = make_shared<bvh_node>(objects, start, mid);
    right = make_shared<bvh_node>(objects, mid, end);
  }

  bbox = surrounding_box(left->bounding_box(), right->bounding_box());
}

auto bvh_node::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  if (!bbox.hit(r, ray_t))
    return false;
  // 递归查找光线与 AABB 的交
  bool hit_left = left->hit(r, ray_t, rec);
  bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

  return hit_left || hit_right;
}

auto bvh_node::bounding_box() const -> aabb {
  return bbox;
}

// sort cmp
inline auto box_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b, int axis) -> bool {
  return a->bounding_box().axis[axis].min < b->bounding_box().axis[axis].min;
}
inline auto box_x_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b) -> bool {
  return box_compare(a, b, 0);
}
inline auto box_y_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b) -> bool {
  return box_compare(a, b, 1);
}
inline auto box_z_compare(const std::shared_ptr<hittable> &a, const std::shared_ptr<hittable> &b) -> bool {
  return box_compare(a, b, 2);
}

#endif