#ifndef TOTAL_TEXTURE_HPP
#define TOTAL_TEXTURE_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/sphere.hpp"
#include "../geometry/box.hpp"
#include "../geometry/BVH.hpp"
#include "../geometry/translate.hpp"
#include "../geometry/constant_medium.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"
#include "../material/dielectric.hpp"
#include "../material/metal.hpp"
#include "../texture/image_texture.hpp"
#include "../texture/noise_texture.hpp"

auto total_texture(hittable_list &world, hittable_list &light) -> void {
  hittable_list boxes1;
  auto ground = new lambertian(color(0.48, 0.83, 0.53));

  const int boxes_per_side = 20;
  for (int i = 0; i < boxes_per_side; i++) {
    for (int j = 0; j < boxes_per_side; j++) {
      auto w = 100.0;
      auto x0 = -1000.0 + i * w;
      auto z0 = -1000.0 + j * w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = random_double(1, 101)();
      auto z1 = z0 + w;

      boxes1.add(std::make_unique<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
    }
  }
  world.add(std::make_unique<bvh_node>(boxes1));

  auto wlight = new diffuse_light(color(7, 7, 7));
  light.add(std::make_unique<xz_rect>(123, 423, 147, 412, 554, wlight));

  auto sphere_material = new lambertian(color(0.7, 0.3, 0.1));
  world.add(std::make_unique<sphere>(point3(400, 400, 200), 50, sphere_material));

  world.add(std::make_unique<sphere>(point3(260, 150, 45), 50, new dielectric(1.5)));
  world.add(std::make_unique<sphere>(point3(0, 150, 145), 50, new metal(color(0.8, 0.8, 0.9), 1.0)));

  world.add(std::make_unique<sphere>(point3(360, 150, 145), 70, new dielectric(1.5)));
  world.add(std::make_unique<constant_medium>(
      std::make_unique<sphere>(point3(360, 150, 145), 70, new dielectric(1.5)), 0.2, color(0.2, 0.4, 0.9)));
  world.add(std::make_unique<constant_medium>(
      std::make_unique<sphere>(point3(0, 0, 0), 5000, new dielectric(1.5)), .0001, color(1, 1, 1)));

  auto emat = new lambertian(new image_texture("src/models/earth/earthmap.jpg"));
  world.add(std::make_unique<sphere>(point3(400, 200, 400), 100, emat));
  auto pertext = new noise_texture(0.1);
  world.add(std::make_unique<sphere>(point3(220, 280, 300), 80, new lambertian(pertext)));
  hittable_list boxes2;
  auto white = new lambertian(color(.73, .73, .73));
  int ns = 1000;
  // int ns = 200;
  for (int j = 0; j < ns; j++) {
    boxes2.add(std::make_unique<sphere>(point3::random(0, 165), 10, white));
  }

  world.add(std::make_unique<translate>(
      std::make_unique<rotate_y>(std::make_unique<bvh_node>(boxes2), 15), vec3d(-100, 270, 395)));
}

#endif