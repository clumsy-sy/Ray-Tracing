#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/triangle.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"

auto test_scene(hittable_list &world, hittable_list &light) -> void {

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto wlight = std::make_shared<diffuse_light>(color(15, 15, 15));

  world.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  light.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, wlight));
  world.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

  auto mat_ptr = std::make_shared<lambertian>(color(.12, .45, .15));
  auto v0 = vec3d(1, 100, 100);
  auto v1 = vec3d(1, 450, 100);
  auto v2 = vec3d(1, 450, 450);
  world.add(std::make_shared<triangle>(v0, v1, v2, mat_ptr));
}

#endif