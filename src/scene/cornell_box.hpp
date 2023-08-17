#ifndef CORNELL_BOX_HPP
#define CORNELL_BOX_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/box.hpp"
#include "../geometry/BVH.hpp"
#include "../geometry/quad.hpp"
#include "../geometry/sphere.hpp"
#include "../geometry/translate.hpp"
#include "../geometry/constant_medium.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/dielectric.hpp"
#include "../material/lambertian.hpp"
#include "../material/metal.hpp"
#include <memory>

// auto cornell_box_new(hittable_list &world, hittable_list &lights) -> void {
//   auto red = std::make_shared<lambertian>(color(.65, .05, .05));
//   auto white = std::make_shared<lambertian>(color(.73, .73, .73));
//   auto green = std::make_shared<lambertian>(color(.12, .45, .15));
//   auto wlight = std::make_shared<diffuse_light>(color(15, 15, 15));

//   // Cornell box sides
//   world.add(std::make_shared<quad>(point3(555, 0, 0), vec3d(0, 0, 555), vec3d(0, 555, 0), green));
//   world.add(std::make_shared<quad>(point3(0, 0, 555), vec3d(0, 0, -555), vec3d(0, 555, 0), red));
//   world.add(std::make_shared<quad>(point3(0, 555, 0), vec3d(555, 0, 0), vec3d(0, 0, 555), white));
//   world.add(std::make_shared<quad>(point3(0, 0, 555), vec3d(555, 0, 0), vec3d(0, 0, -555), white));
//   world.add(std::make_shared<quad>(point3(555, 0, 555), vec3d(-555, 0, 0), vec3d(0, 555, 0), white));

//   // Light
//   world.add(std::make_shared<quad>(point3(213, 554, 227), vec3d(130, 0, 0), vec3d(0, 0, 105), wlight));

//   // Box
//   auto aluminum = std::make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
//   auto box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), aluminum);
//   // auto box1 = std::make_shared<box>(point3(0,0,0), point3(165,330,165), white);
//   auto box2 = std::make_shared<rotate_y>(box1, 15);
//   auto box3 = std::make_shared<translate>(box2, vec3d(265, 0, 295));
//   world.add(box3);

//   // Glass Sphere
//   auto glass = std::make_shared<dielectric>(1.5);
//   world.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));

//   // Light Sources
//   auto m = std::shared_ptr<material>();
//   lights.add(make_shared<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
//   lights.add(make_shared<sphere>(point3(190, 90, 190), 90, m));
// }

auto cornell_box(hittable_list &world, hittable_list &light) -> void {
  std::cout << 1 << std::endl;
  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto wlight = std::make_shared<diffuse_light>(color(15, 15, 15));
  auto m = std::shared_ptr<material>();

  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 0, red));
  
  light.add(std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
  light.add(std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), wlight));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(std::make_unique<xy_rect>(0, 555, 0, 555, 555, white));
}

// auto cornell_box_rotate(hittable_list &world, hittable_list &light) -> void {
//   hittable_list boxbox;
//   auto red = std::make_shared<lambertian>(color(.65, .05, .05));
//   auto white = std::make_shared<lambertian>(color(.73, .73, .73));
//   auto green = std::make_shared<lambertian>(color(.12, .45, .15));
//   auto wlight = std::make_shared<diffuse_light>(color(15, 15, 15));

//   boxbox.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
//   boxbox.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
//   light.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, wlight));
//   boxbox.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
//   boxbox.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
//   boxbox.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

//   std::unique_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
//   box1 = std::make_shared<rotate_y>(box1, 15);
//   box1 = std::make_shared<translate>(box1, vec3d(265, 0, 295));
//   boxbox.add(box1);

//   std::unique_ptr<hittable> box2 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
//   box2 = std::make_shared<rotate_y>(box2, -18);
//   box2 = std::make_shared<translate>(box2, vec3d(130, 0, 65));
//   boxbox.add(box2);
//   world.add(std::make_shared<bvh_node>(boxbox));
// }

// auto cornell_smoke(hittable_list &world, hittable_list &light) -> void {

//   auto red = std::make_shared<lambertian>(color(.65, .05, .05));
//   auto white = std::make_shared<lambertian>(color(.73, .73, .73));
//   auto green = std::make_shared<lambertian>(color(.12, .45, .15));
//   auto wlight = std::make_shared<diffuse_light>(color(7, 7, 7));

//   world.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
//   world.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
//   light.add(std::make_shared<xz_rect>(113, 443, 127, 432, 554, wlight));
//   world.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
//   world.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
//   world.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

//   std::unique_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
//   box1 = std::make_shared<rotate_y>(box1, 15);
//   box1 = std::make_shared<translate>(box1, vec3d(265, 0, 295));

//   std::unique_ptr<hittable> box2 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
//   box2 = std::make_shared<rotate_y>(box2, -18);
//   box2 = std::make_shared<translate>(box2, vec3d(130, 0, 65));

//   world.add(std::make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
//   world.add(std::make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));
// }

#endif
