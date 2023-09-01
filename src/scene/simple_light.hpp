#ifndef SIMPLE_LIGHT_HPP
#define SIMPLE_LIGHT_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/sphere.hpp"
#include "../geometry/aarect.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"
#include "../texture/noise_texture.hpp"
// #include "../texture/checker_texture.hpp"

auto simple_light(hittable_list &world, hittable_list &light) -> void {

  auto pertext = new noise_texture(4);
  world.add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, new lambertian(pertext)));
  world.add(std::make_unique<sphere>(point3(0, 2, 0), 2, new lambertian(pertext)));

  // auto checker = new checker_texture(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
  // world.add(new sphere(point3(0, -1000, 0), 1000, new lambertian(checker)));

  auto difflight = new diffuse_light(color(4, 4, 4));
  // auto difflight = new diffuse_light(color(16, 16, 16));
  light.add(std::make_unique<xy_rect>(3, 5, 1, 3, -2, difflight));
  light.add(std::make_unique<sphere>(point3(0, 6, 0), 1.2, difflight));
}

#endif