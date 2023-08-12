#ifndef EARTH_HPP
#define EARTH_HPP

#include "../material/lambertian.hpp"
#include "../geometry/hittablelist.hpp"
#include "../geometry/sphere.hpp"
#include "../texture/image_texture.hpp"

auto earth() -> hittable_list {
  auto earth_texture = std::make_shared<image_texture>("src/models/earth/earthmap.jpg");
  auto earth_surface = std::make_shared<lambertian>(earth_texture);
  auto globe = std::make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

  return {globe};
}

#endif