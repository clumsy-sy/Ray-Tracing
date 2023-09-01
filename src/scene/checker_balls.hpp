#ifndef CHECKER_BALLS_HPP
#define CHECKER_BALLS_HPP

#include "../global.hpp"
#include "../material/dielectric.hpp"
#include "../material/lambertian.hpp"
#include "../material/metal.hpp"
#include "../geometry/hittablelist.hpp"
#include "../geometry/sphere.hpp"
#include "../geometry/BVH.hpp"
#include "../texture/checker_texture.hpp"

void random_checkerANDballs(hittable_list &world, [[maybe_unused]] hittable_list &light) {

  auto checker = new checker_texture(color(0.01, 0.01, 0.01), color(0.99, 0.99, 0.99));
  world.add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, new lambertian(checker)));

  hittable_list box;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        material* sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = new lambertian(albedo);
          box.add(std::make_unique<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5)();
          sphere_material = new metal(albedo, fuzz);
          box.add(std::make_unique<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = new dielectric(1.5);
          box.add(std::make_unique<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  world.add(std::make_unique<bvh_node>(box));

  auto material1 = new dielectric(1.5);
  world.add(std::make_unique<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = new lambertian(color(0.4, 0.2, 0.1));
  world.add(std::make_unique<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = new metal(color(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_unique<sphere>(point3(4, 1, 0), 1.0, material3));
}

#endif