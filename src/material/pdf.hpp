#ifndef PDF_HPP
#define PDF_HPP

#include <utility>

#include "../vector/Vec3dx4.hpp"
#include "../geometry/ONB.hpp"
#include "../geometry/hittable.hpp"

/*
  根据方向生成 概率密度函数
*/
class pdf {
public:
  virtual ~pdf() = default;

  [[nodiscard]] virtual auto value(const Vec3d &direction) const -> double = 0;
  [[nodiscard]] virtual auto generate() const -> Vec3d = 0;
};

class sphere_pdf : public pdf {
public:
  sphere_pdf() = default;

  [[nodiscard]] auto value([[maybe_unused]] const Vec3d &direction) const -> double override {
    return 1 / (4 * PI);
  }

  [[nodiscard]] auto generate() const -> Vec3d override {
    return random_unit_vector();
  }
};

class cosine_pdf final : public pdf {
public:
  cosine_pdf(const Vec3d &w) {
    uvw.build_from_w(w);
  }

  [[nodiscard]] auto value(const Vec3d &direction) const -> double override {
    const auto cosine = dot(unit_vector(direction), uvw.w());
    return (cosine <= 0) ? 0 : cosine / PI;
  }

  [[nodiscard]] auto generate() const -> Vec3d override {
    return uvw.local(random_cosine_direction());
  }

  onb uvw;
};

class mixture_pdf : public pdf {
public:
  std::array<std::shared_ptr<pdf>, 2> p;

public:
  mixture_pdf(std::shared_ptr<pdf> p0, std::shared_ptr<pdf> p1) {
    p[0] = std::move(p0);
    p[1] = std::move(p1);
  }

  [[nodiscard]] auto value(const Vec3d &direction) const -> double override {
    return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
  }

  [[nodiscard]] auto generate() const -> Vec3d override {
    if (random_double() < 0.5)
      return p[0]->generate();
    else
      return p[1]->generate();
  }
};

class hittable_pdf : public pdf {
private:
  const hittable &objects;
  point3 origin;

public:
  hittable_pdf(const hittable &_objects, point3 _origin) : objects(_objects), origin(std::move(_origin)) {}

  [[nodiscard]] auto value(const Vec3d &direction) const -> double override {
    return objects.pdf_value(origin, direction);
  }

  [[nodiscard]] auto generate() const -> Vec3d override {
    return objects.random(origin);
  }
};

#endif