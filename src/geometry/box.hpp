#ifndef BOX_HPP
#define BOX_HPP

#include "aarect.hpp"
#include "hittablelist.hpp"
#include "interval.hpp"
#include <memory>

class box : public hittable {
public:
  point3 box_min;
  point3 box_max;
  hittable_list sides;

public:
  box() = default;
  box(const point3 &p0, const point3 &p1, std::shared_ptr<material> &ptr);

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    return {box_min, box_max};
  }
};

box::box(const point3 &p0, const point3 &p1, std::shared_ptr<material> &ptr) {
  box_min = p0;
  box_max = p1;
  sides.add(std::make_unique<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
  sides.add(std::make_unique<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
  sides.add(std::make_unique<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
  sides.add(std::make_unique<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
  sides.add(std::make_unique<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
  sides.add(std::make_unique<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

auto box::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  return sides.hit(r, ray_t, rec);
}

#endif