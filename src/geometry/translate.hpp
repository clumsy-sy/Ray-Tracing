#ifndef TRANSLATE_HPP
#define TRANSLATE_HPP

#include "hittable.hpp"
#include "interval.hpp"

class translate : public hittable {
public:
  std::unique_ptr<hittable> ptr;
  vec3d offset;

public:
  translate(std::unique_ptr<hittable> p, vec3d displacement)
      : ptr(std::move(p)), offset(std::move(displacement)) {}

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override;
  auto print(std::ostream &os, const std::string &prefix = "") const -> void override {
    os << prefix << "[trans]ofs=" << offset << " ";
    ptr->print(os, prefix);
  }
  friend auto operator<<(std::ostream &os, const translate &t) -> std::ostream & {
    os << "[trans]ofs=" << t.offset << " ";
    t.ptr->print(os);
    return os;
  }
};

auto translate::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  ray moved_r(r.origin() - offset, r.direction());
  if (!ptr->hit(moved_r, ray_t, rec))
    return false;

  rec.p += offset;
  rec.set_face_normal(moved_r, rec.normal);

  return true;
}

auto translate::bounding_box() const -> aabb {
  return ptr->bounding_box().min() + offset;
}

class scale : public hittable {
public:
  std::unique_ptr<hittable> ptr;
  vec3d vec;

public:
  scale(std::unique_ptr<hittable> p, vec3d coefficient)
      : ptr(std::move(p)), vec(std::move(coefficient)) {}

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override;
  auto print(std::ostream &os, const std::string &prefix = "") const -> void override {
    os << prefix << "[scale]vec=" << vec << " ";
    ptr->print(os, prefix);
  }
  friend auto operator<<(std::ostream &os, const scale &t) -> std::ostream & {
    os << "[scale]vec=" << t.vec << " ";
    t.ptr->print(os);
    return os;
  }
};

auto scale::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  ray moved_r(r.origin() / vec, r.direction() / vec);
  if (!ptr->hit(moved_r, ray_t, rec))
    return false;

  rec.p = rec.p * vec;
  rec.set_face_normal(moved_r, rec.normal);

  return true;
}

auto scale::bounding_box() const -> aabb {
  return {ptr->bounding_box().min() * vec, ptr->bounding_box().max() * vec};
}

class rotate_y : public hittable {
public:
  std::unique_ptr<hittable> ptr;
  double angle;
  double sin_theta;
  double cos_theta;
  aabb bbox;

public:
  rotate_y(std::unique_ptr<hittable> p, double angle);

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    return bbox;
  }
  auto print(std::ostream &os, const std::string &prefix = "") const -> void override {
    os << prefix << "[rotate_y]ang=" << angle << " ";
    ptr->print(os, prefix);
  }
  friend auto operator<<(std::ostream &os, const rotate_y &t) -> std::ostream & {
    os << "[rotate_y]ang=" << t.angle << " ";
    t.ptr->print(os);
    return os;
  }
};

rotate_y::rotate_y(std::unique_ptr<hittable> p, double angle)
    : ptr(std::move(p)), angle(angle) {
  auto radians = degrees_to_radians(angle);
  sin_theta = sin(radians);
  cos_theta = cos(radians);
  bbox = ptr->bounding_box();

  point3 min(infinity, infinity, infinity);
  point3 max(-infinity, -infinity, -infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
        auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
        auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

        auto newx = cos_theta * x + sin_theta * z;
        auto newz = -sin_theta * x + cos_theta * z;

        vec3d tester(newx, y, newz);

        min = merge_min(min, tester);
        max = merge_max(max, tester);
      }
    }
  }

  bbox = aabb(min, max);
}
auto rotate_y::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  auto rot = [s = sin_theta, c = cos_theta](const vec3d &v) {
    double r0 = c * v.x() - s * v.z();
    double r2 = s * v.x() + c * v.z();
    return vec3d{r0, v.y(), r2};
  };
  auto origin = rot(r.origin());
  auto direction = rot(r.direction());

  ray rotated_r(origin, direction);

  if (!ptr->hit(rotated_r, ray_t, rec))
    return false;

  auto roti = [s = -sin_theta, c = cos_theta](const vec3d &v) {
    double r0 = c * v.x() - s * v.z();
    double r2 = s * v.x() + c * v.z();
    return vec3d{r0, v.y(), r2};
  };
  auto p = roti(rec.p);
  auto normal = roti(rec.normal);

  rec.p = p;
  rec.set_face_normal(rotated_r, normal);

  return true;
}

class rotate_x : public hittable {
public:
  std::unique_ptr<hittable> ptr;
  double angle;
  double sin_theta;
  double cos_theta;
  aabb bbox;

public:
  rotate_x(std::unique_ptr<hittable> p, double angle);

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    return bbox;
  }
  auto print(std::ostream &os, const std::string &prefix = "") const -> void override {
    os << prefix << "[rotate_x]ang=" << angle << " ";
    ptr->print(os, prefix);
  }
  friend auto operator<<(std::ostream &os, const rotate_x &t) -> std::ostream & {
    os << "[rotate_x]ang=" << t.angle << " ";
    t.ptr->print(os);
    return os;
  }
};

rotate_x::rotate_x(std::unique_ptr<hittable> p, double angle)
    : ptr(std::move(p)), angle(angle) {
  auto radians = degrees_to_radians(angle);
  sin_theta = sin(radians);
  cos_theta = cos(radians);
  bbox = ptr->bounding_box();

  point3 min(infinity, infinity, infinity);
  point3 max(-infinity, -infinity, -infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
        auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
        auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

        auto newy = cos_theta * y - sin_theta * z;
        auto newz = sin_theta * y + cos_theta * z;

        vec3d tester(x, newy, newz);

        min = merge_min(min, tester);
        max = merge_max(max, tester);
      }
    }
  }

  bbox = aabb(min, max);
}
auto rotate_x::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  auto rot = [s = sin_theta, c = cos_theta](const vec3d &v) {
    double r1 = c * v.y() - s * v.z();
    double r2 = s * v.y() + c * v.z();
    return vec3d{v.x(), r1, r2};
  };

  auto origin = rot(r.origin());
  auto direction = rot(r.direction());

  ray rotated_r(origin, direction);

  if (!ptr->hit(rotated_r, ray_t, rec))
    return false;

  auto roti = [s = -sin_theta, c = cos_theta](const vec3d &v) {
    double r1 = c * v.y() - s * v.z();
    double r2 = s * v.y() + c * v.z();
    return vec3d{v.x(), r1, r2};
  };
  auto p = roti(rec.p);
  auto normal = roti(rec.normal);

  rec.p = p;
  rec.set_face_normal(rotated_r, normal);

  return true;
}

class rotate_z : public hittable {
public:
  std::unique_ptr<hittable> ptr;
  double angle;
  double sin_theta;
  double cos_theta;
  aabb bbox;

public:
  rotate_z(std::unique_ptr<hittable> p, double angle);

  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;

  [[nodiscard]] auto bounding_box() const -> aabb override {
    return bbox;
  }
  auto print(std::ostream &os, const std::string &prefix = "") const -> void override {
    os << prefix << "[rotate_z]ang=" << angle << " ";
    ptr->print(os, prefix);
  }
  friend auto operator<<(std::ostream &os, const rotate_z &t) -> std::ostream & {
    os << "[rotate_z]ang=" << t.angle << " ";
    t.ptr->print(os);
    return os;
  }
};

rotate_z::rotate_z(std::unique_ptr<hittable> p, double angle)
    : ptr(std::move(p)), angle(angle) {
  auto radians = degrees_to_radians(angle);
  sin_theta = sin(radians);
  cos_theta = cos(radians);
  bbox = ptr->bounding_box();

  point3 min(infinity, infinity, infinity);
  point3 max(-infinity, -infinity, -infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
        auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
        auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

        auto newx = cos_theta * x - sin_theta * y;
        auto newy = cos_theta * y + sin_theta * x;

        vec3d tester(newx, newy, z);

        min = merge_min(min, tester);
        max = merge_max(max, tester);
      }
    }
  }

  bbox = aabb(min, max);
}
auto rotate_z::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  auto rot = [s = sin_theta, c = cos_theta](const vec3d &v) {
    double r1 = c * v.y() - s * v.x();
    double r0 = s * v.y() + c * v.x();
    return vec3d{r0, r1, v.z()};
  };

  auto origin = rot(r.origin());
  auto direction = rot(r.direction());

  ray rotated_r(origin, direction);

  if (!ptr->hit(rotated_r, ray_t, rec))
    return false;

  auto roti = [s = sin_theta, c = cos_theta](const vec3d &v) {
    double r1 = c * v.y() - s * v.x();
    double r0 = s * v.y() + c * v.x();
    return vec3d{r0, r1, v.z()};
  };
  auto p = roti(rec.p);
  auto normal = roti(rec.normal);

  rec.p = p;
  rec.set_face_normal(rotated_r, normal);

  return true;
}

#endif