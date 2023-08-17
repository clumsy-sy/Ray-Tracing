#ifndef BALLS_WORLD_HPP
#define BALLS_WORLD_HPP

#include "../global.hpp"
#include "../material/dielectric.hpp"
#include "../material/lambertian.hpp"
#include "../material/metal.hpp"
#include "../geometry/hittablelist.hpp"
#include "../geometry/sphere.hpp"
#include "../geometry/BVH.hpp"
#include <memory>

auto random_balls_world(hittable_list &world, [[maybe_unused]] hittable_list &light) -> void {

  auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
  std::unique_ptr<hittable> ground = std::make_unique<sphere>(point3(0, -2000, 0), 2000, ground_material);
  world.add(ground);

  hittable_list box;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = std::make_shared<lambertian>(albedo);
          std::unique_ptr<hittable> sphere1 = std::make_unique<sphere>(center, 0.2, sphere_material);
          box.add(sphere1);
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5)();
          sphere_material = std::make_shared<metal>(albedo, fuzz);
          std::unique_ptr<hittable> sphere2 = std::make_unique<sphere>(center, 0.2, sphere_material);
          box.add(sphere2);
        } else {
          // glass
          sphere_material = std::make_shared<dielectric>(1.5);
          std::unique_ptr<hittable> sphere3 = std::make_unique<sphere>(center, 0.2, sphere_material);
          box.add(sphere3);
        }
      }
    }
  }
  world.add(std::make_unique<bvh_node>(box));

  auto material1 = std::make_shared<dielectric>(1.5);
  world.add(std::make_unique<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(std::make_unique<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_unique<sphere>(point3(4, 1, 0), 1.0, material3));
}

#endif