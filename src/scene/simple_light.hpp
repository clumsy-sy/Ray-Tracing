#ifndef SIMPLE_LIGHT_HPP
#define SIMPLE_LIGHT_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/sphere.hpp"
#include "../geometry/aarect.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"
#include "../texture/noise_texture.hpp"
// #include "../texture/checker_texture.hpp"

auto simple_light() -> hittable_list {
  hittable_list objects;

  auto pertext = std::make_shared<noise_texture>(4);
  objects.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
  objects.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

  // auto checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
  // objects.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

  auto difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
  // auto difflight = std::make_shared<diffuse_light>(color(16, 16, 16));
  objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
  objects.add(std::make_shared<sphere>(point3(0, 6, 0), 1.2, difflight));

  return objects;
}

#endif