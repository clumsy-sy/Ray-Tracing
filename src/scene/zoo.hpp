#ifndef BUNNY_WORLD_HPP
#define BUNNY_WORLD_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/meshTriangle.hpp"
#include "../geometry/box.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/BVH.hpp"
#include "../geometry/translate.hpp"
#include "../geometry/quad.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"
#include "../texture/image_texture.hpp"
#include <memory>

auto bunny_world(hittable_list &world, hittable_list &light) -> void {

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto wlight = std::make_shared<diffuse_light>(color(5, 5, 5));

  world.add(std::make_unique<yz_rect>(0, 10, -10, 10, 10, green));
  world.add(std::make_unique<yz_rect>(0, 10, -10, 10, -10, red));
  world.add(std::make_unique<xz_rect>(-3, 3, -3, 3, 9.999999, wlight)); // light
  auto m = std::shared_ptr<material>();
  light.add(std::make_unique<xz_rect>(-3, 3, -3, 3, 9.999999, m));   // light
  world.add(std::make_unique<xz_rect>(-10, 10, -10, 10, 0, white));  // bottom
  world.add(std::make_unique<xz_rect>(-10, 10, -10, 10, 10, white)); // top
  world.add(std::make_unique<xy_rect>(-10, 10, -10, 10, -6, white)); // back

  auto bunny = MeshTriangle("src/models/bunny/bunny.obj", 60.0f);
  std::cout << "bunny bounding box : " << bunny.bbox.min() << "    ||    " << bunny.bbox.max() << std::endl;

  auto bunnybox = std::make_unique<bvh_node>(bunny.triangles);
  auto box = std::make_unique<translate>(
      std::make_unique<scale>(std::move(bunnybox), vec3d(1, 1, 1)), vec3d(0, -bunny.bbox.min().y(), 0));
  world.add(std::move(box));
}

auto cow_world(hittable_list &world, hittable_list &light) -> void {

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto wlight = std::make_shared<diffuse_light>(color(10, 10, 10));

  world.add(std::make_unique<yz_rect>(0, 10, -10, 10, 10, green));
  world.add(std::make_unique<yz_rect>(0, 10, -10, 10, -10, red));
  world.add(std::make_unique<xz_rect>(-3, 3, -3, 3, 9.999999, wlight)); // light
  auto m = std::shared_ptr<material>();
  light.add(std::make_unique<xz_rect>(-3, 3, -3, 3, 9.999999, m));   // light
  world.add(std::make_unique<xz_rect>(-10, 10, -10, 10, 0, white));  // bottom
  world.add(std::make_unique<xz_rect>(-10, 10, -10, 10, 10, white)); // top
  world.add(std::make_unique<xy_rect>(-10, 10, -10, 10, -6, white)); // back

  auto cow_texture = std::make_shared<image_texture>("src/models/spot/spot_texture.png");
  auto cow_surface = std::make_shared<lambertian>(cow_texture);
  auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 4, cow_surface);
  std::cout << "cow bounding box : " << cow.bbox.min() << "    ||    " << cow.bbox.max() << std::endl;
  auto rotatecow = std::make_unique<rotate_y>(std::make_unique<bvh_node>(cow.triangles), 150);
  aabb cowab = rotatecow->bounding_box();
  auto box = std::make_unique<translate>(std::move(rotatecow), vec3d(0, -cowab.min()[1], 2));
  world.add(std::move(box));
}

auto cornell_box_bunny_rotate(hittable_list &world, hittable_list &light) -> void {

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto wlight = std::make_shared<diffuse_light>(color(20, 20, 20));

  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 0, red));
  auto m = std::shared_ptr<material>();
  light.add(std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
  world.add(std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), wlight));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(std::make_unique<xy_rect>(0, 555, 0, 555, 555, white));

  // auto aluminum = std::make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
  // auto box1 = std::make_shared<box>(point3(0,0,0), point3(165,330,165), aluminum);
  auto box1 = std::make_unique<translate>(
      std::make_unique<rotate_y>(std::make_unique<box>(point3(0, 0, 0), point3(165, 330, 165), white), 15),
      vec3d(265, 0, 295));
  world.add(std::move(box1));

  auto box2 = std::make_unique<translate>(
      std::make_unique<rotate_y>(std::make_unique<box>(point3(0, 0, 0), point3(165, 165, 165), white), -18),
      vec3d(130, 0, 65));
  world.add(std::move(box2));

  auto bunny = MeshTriangle("src/models/bunny/bunny.obj", 60.0f);
  std::cout << "bunny bounding box : " << bunny.bbox.min() << "    ||    " << bunny.bbox.max() << std::endl;
  std::cout << 0 << std::endl;
  auto bigbunny = std::make_unique<scale>(
      std::make_unique<rotate_y>(std::make_unique<bvh_node>(bunny.triangles), 180), vec3d(15, 15, 15));
  std::cout << 1 << std::endl;
  aabb bunnyaabb = bigbunny->bounding_box();
  auto center = bunnyaabb.center();
  std::cout << 2 << std::endl;
  auto box = std::make_unique<translate>(std::move(bigbunny), vec3d(330 + center[0], 300, 400));
  world.add(std::move(box));
  std::cout << 3 << std::endl;
  auto cow_texture = std::make_shared<image_texture>("src/models/spot/spot_texture.png");
  auto cow_surface = std::make_shared<lambertian>(cow_texture);
  auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 80, cow_surface);
  // auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 4);
  std::cout << "cow bounding box : " << cow.bbox.min() << "    ||    " << cow.bbox.max() << std::endl;
  auto rotatecow = std::make_unique<rotate_y>(std::make_unique<bvh_node>(cow.triangles), 45);
  aabb cowab = rotatecow->bounding_box();

  auto boxcow = std::make_unique<translate>(std::move(rotatecow), vec3d(165, 165 - cowab.min()[1], 145));
  world.add(std::move(boxcow));
}

#endif