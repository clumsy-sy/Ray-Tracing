#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material.hpp"
#include "../texture/texture.hpp"
#include "../texture/solid_color.hpp"
#include "../geometry/hittable.hpp"

class lambertian : public material {
public:
  std::shared_ptr<texture> albedo; // 以某种概率分布衰减，albedo / p

public:
  lambertian(const color &a) : albedo(std::make_shared<solid_color>(a)) {}
  lambertian(std::shared_ptr<texture> a) : albedo(std::move(a)) {}

  auto scatter(const ray &, const hit_record &rec, color &attenuation, ray &scattered) const -> bool override {
    // 得到一个在交点的单位相切圆上的随机散射方向向量
    auto scatter_direction = rec.normal + random_unit_vector();

    // 捕获衰减散射方向
    if (scatter_direction.near_zero()) // 浮点误差，判断是否是法线方向
      scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    if (rec.u <= 0 && rec.u >= 1 && rec.v <= 0 && rec.v >= 1)
      printf("%.3lf  %.3lf\n", rec.u, rec.v);
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    if (rec.u <= 0 && rec.u >= 1 && rec.v <= 0 && rec.v >= 1)
      printf("attenuation : %.5lf  %.5lf  %.5lf\n", attenuation[0], attenuation[1], attenuation[2]);
    return true;
  }
};

#endif