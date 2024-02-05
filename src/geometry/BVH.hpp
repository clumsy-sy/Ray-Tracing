#ifndef BVH_HPP
#define BVH_HPP

#include "../global.hpp"
#include "hittablelist.hpp"
#include "interval.hpp"

inline auto box_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b, int axis) -> bool;
inline auto box_x_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b) -> bool;
inline auto box_y_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b) -> bool;
inline auto box_z_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b) -> bool;

/*
  BVH 的结点（也是可背光线击中的）
  左右儿子 + 自己的 AABB
*/
class bvh_node : public hittable {
public:
  std::unique_ptr<hittable> left;
  std::unique_ptr<hittable> right;
  aabb bbox;

public:
  bvh_node();

  bvh_node(hittable_list &list) : bvh_node(list.objects, 0, list.objects.size()) {}

  bvh_node(std::vector<std::unique_ptr<hittable>> &src_objects, size_t start, size_t end);

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override;
  auto printTree(std::ostream &os, const bvh_node &m, int type, const std::string &prefix = "")
      -> void {
    if (type == 0) {
      os << prefix << "└─ root\n";
    } else if (type == 1) {
      os << prefix << "└─ left\n";
    } else {
      os << prefix << "└─ right\n";
    }

    if (m.left != nullptr) {
      std::string childPrefix = prefix + "   ";
      m.left->print(os);
    }
    if (m.right != nullptr) {
      std::string childPrefix = prefix + "│  ";
      m.right->print(os);
    }

    // for (size_t i = 0; i < node.children.size(); ++i) {
    //     const Node& child = node.children[i];
    //     std::string childPrefix = prefix + (i == node.children.size() - 1 ? "   " : "│  ");
    //     printTree(child, childPrefix);
    // }
  }
  auto print(std::ostream &os, const std::string &prefix = "") const -> void override {
    os << prefix << "|--[bvh_node]\n";
    auto left_prefix = prefix + "L";
    if (left != nullptr)
      left->print(os, left_prefix);
    else
      os << "null";
    os << "\n";
    auto right_prefix = prefix + "R";
    if (right != nullptr)
      right->print(os, right_prefix);
    else
      os << "null";
  }
  friend auto operator<<(std::ostream &os, const bvh_node &m) -> std::ostream & {
    os << "|--[bvh_node]\n";
    std::string prefix_l = "L";
    m.left->print(os, prefix_l);
    os << "\n";
    std::string prefix_r = "R";
    m.right->print(os, prefix_r);
    return os;
  }
};
bvh_node::bvh_node(
    std::vector<std::unique_ptr<hittable>> &src_objects, size_t start, size_t end) {
  auto &objects = src_objects; // Create a modifiable array of the source scene objects
  // 随机一个轴，按这个轴排序
  int axis = random_int(0, 2)();
  auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

  size_t object_span = end - start;
  // 根据当前结点大小分类处理，1 为 叶子结点， 2 单独处理
  if (object_span == 1) {
    left = std::move(objects[start]);
    right = nullptr;
    bbox = left->bounding_box();
    return;
  } else if (object_span == 2) {
    if (comparator(objects[start], objects[start + 1])) {
      left = std::move(objects[start]);
      right = std::move(objects[start + 1]);
    } else {
      left = std::move(objects[start + 1]);
      right = std::move(objects[start]);
    }
  } else {
    std::sort(objects.begin() + start, objects.begin() + end, comparator);
    // 按随机的轴排序后，递归建树
    auto mid = start + object_span / 2;
    left = std::make_unique<bvh_node>(objects, start, mid);
    right = std::make_unique<bvh_node>(objects, mid, end);
  }

  bbox = surrounding_box(left->bounding_box(), right->bounding_box());
}

auto bvh_node::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  if (!bbox.hit(r, ray_t))
    return false;
  // 递归查找光线与 AABB 的交
  bool hit_left = false, hit_right = false;
  hit_left = left->hit(r, ray_t, rec);
  if (right != nullptr) {
    hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);
  }

  return hit_left || hit_right;
}

auto bvh_node::bounding_box() const -> aabb {
  return bbox;
}

// sort cmp
inline auto box_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b, int axis) -> bool {
  return a->bounding_box().axis[axis].min < b->bounding_box().axis[axis].min;
}
inline auto box_x_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b) -> bool {
  return box_compare(a, b, 0);
}
inline auto box_y_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b) -> bool {
  return box_compare(a, b, 1);
}
inline auto box_z_compare(
    const std::unique_ptr<hittable> &a, const std::unique_ptr<hittable> &b) -> bool {
  return box_compare(a, b, 2);
}

#endif