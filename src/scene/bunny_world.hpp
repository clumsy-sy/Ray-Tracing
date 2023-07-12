#ifndef BUNNY_WORLD_HPP
#define BUNNY_WORLD_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/meshTriangle.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/translate.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"

auto bunny_world() -> hittable_list {
  hittable_list objects;

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

  // objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  // objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  // objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
  // objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  // objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
  // objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));


  auto bunny = MeshTriangle("src/models/bunny/bunny.obj");
  std::cout << "bunnt bounding box : " << bunny.box.minimum << "    ||    "  <<  bunny.box.maximum << std::endl;
  // MeshTriangle rotatebunny;
  // for(auto &now : bunny.triangles.objects) {
  //   rotatebunny.triangles.add(std::make_shared<rotate_y>(now, 180));
  // }
  objects.add(std::make_shared<bvh_node>(bunny.triangles));
  return objects;
}

#endif