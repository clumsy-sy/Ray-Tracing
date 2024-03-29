#ifndef FLIP_FACE
#define FLIP_FACE

#include "hittable.hpp"

class flip_face : public hittable {
public:
  std::shared_ptr<hittable> ptr;

public:
  flip_face(std::unique_ptr<hittable> p) : ptr(std::move(p)) {}

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override {
    if (!ptr->hit(r, ray_t, rec))
      return false;

    rec.front_face = !rec.front_face;
    return true;
  }

  [[nodiscard]] auto bounding_box() const -> aabb override {
    return ptr->bounding_box();
  }
  auto print(std::ostream &os, const std::string &prefix = "") const -> void override {
    os << prefix << "[flip_face]: ";
    ptr->print(os, prefix);
  }
  friend auto operator<<(std::ostream &os, const flip_face &m) -> std::ostream & {
    os << "[flip_face]: ";
    m.ptr->print(os);
    return os;
  }
};

#endif