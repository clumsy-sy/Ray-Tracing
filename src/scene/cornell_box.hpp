#ifndef CORNELL_BOX_HPP
#define CORNELL_BOX_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/box.hpp"
#include "../geometry/BVH.hpp"
#include "../geometry/quad.hpp"
#include "../geometry/sphere.hpp"
#include "../geometry/translate.hpp"
#include "../geometry/meshTriangle.hpp"
#include "../geometry/constant_medium.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/dielectric.hpp"
#include "../material/lambertian.hpp"
#include "../texture/image_texture.hpp"
#include "../texture/noise_texture.hpp"
#include "../material/metal.hpp"
#include <memory>

auto cornell_box_new(hittable_list &world, hittable_list &lights) -> void {
  auto red = new lambertian(color(.65, .05, .05));
  auto white = new lambertian(color(.73, .73, .73));
  auto green = new lambertian(color(.12, .45, .15));
  auto wlight = new diffuse_light(color(15, 15, 15));

  // Cornell box sides
  world.add(
      std::make_unique<quad>(point3(555, 0, 0), vec3d(0, 0, 555), vec3d(0, 555, 0), green));
  world.add(
      std::make_unique<quad>(point3(0, 0, 555), vec3d(0, 0, -555), vec3d(0, 555, 0), red));
  world.add(
      std::make_unique<quad>(point3(0, 555, 0), vec3d(555, 0, 0), vec3d(0, 0, 555), white));
  world.add(
      std::make_unique<quad>(point3(0, 0, 555), vec3d(555, 0, 0), vec3d(0, 0, -555), white));
  world.add(
      std::make_unique<quad>(point3(555, 0, 555), vec3d(-555, 0, 0), vec3d(0, 555, 0), white));

  // Light
  world.add(std::make_unique<quad>(
      point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), wlight));

  // Box
  auto aluminum = new metal(color(0.8, 0.85, 0.88), 0.0);
  auto box1 = std::make_unique<box>(point3(0, 0, 0), point3(165, 330, 165), aluminum);
  // auto box1 = new box(point3(0,0,0), point3(165,330,165), white);
  auto box2 = std::make_unique<rotate_y>(std::move(box1), 15);
  world.add(std::make_unique<translate>(std::move(box2), vec3d(265, 0, 295)));

  // Glass Sphere
  auto glass = new dielectric(1.5);
  world.add(std::make_unique<sphere>(point3(190, 90, 190), 90, glass));

  // Light Sources
  auto m = nullptr;
  lights.add(
      std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
  lights.add(std::make_unique<sphere>(point3(190, 90, 190), 90, m));
}

auto cornell_box(hittable_list &world, hittable_list &light) -> void {
  auto red = new lambertian(color(.65, .05, .05));
  auto white = new lambertian(color(.73, .73, .73));
  auto green = new lambertian(color(.12, .45, .15));
  auto wlight = new diffuse_light(color(15, 15, 15));

  world.add(
      std::make_unique<quad>(point3(555, 0, 0), point3(0, 0, 555), point3(0, 555, 0), green));
  // world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 0, red));
  auto m = nullptr;
  light.add(
      std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
  world.add(std::make_unique<quad>(
      point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), wlight));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(std::make_unique<xy_rect>(0, 555, 0, 555, 555, white));
}

auto cornell_box_rotate(hittable_list &world, hittable_list &light) -> void {
  auto red = new lambertian(color(.65, .05, .05));
  auto white = new lambertian(color(.73, .73, .73));
  auto green = new lambertian(color(.12, .45, .15));
  auto wlight = new diffuse_light(color(50, 50, 50));

  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 0, red));
  auto m = nullptr;
  light.add(
      std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
  world.add(std::make_unique<quad>(
      point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), wlight));
  // light.add(new xz_rect(213, 343, 227, 332, 554, wlight));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(std::make_unique<xy_rect>(0, 555, 0, 555, 555, white));

  auto box1 = std::make_unique<box>(point3(0, 0, 0), point3(165, 330, 165), white);
  auto box11 = std::make_unique<rotate_y>(std::move(box1), 15);
  world.add(std::make_unique<translate>(std::move(box11), vec3d(265, 0, 295)));

  auto box2 = std::make_unique<box>(point3(0, 0, 0), point3(165, 165, 165), white);
  auto box22 = std::make_unique<rotate_y>(std::move(box2), -18);
  world.add(std::make_unique<translate>(std::move(box22), vec3d(130, 0, 65)));
}

auto cornell_smoke(hittable_list &world, hittable_list &light) -> void {

  auto red = new lambertian(color(.65, .05, .05));
  auto white = new lambertian(color(.73, .73, .73));
  auto green = new lambertian(color(.12, .45, .15));
  auto wlight = new diffuse_light(color(15, 15, 15));

  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 0, red));
  auto m = nullptr;
  light.add(
      std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
  world.add(std::make_unique<quad>(
      point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), wlight));
  // light.add(new xz_rect(213, 343, 227, 332, 554, wlight));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(std::make_unique<xy_rect>(0, 555, 0, 555, 555, white));

  auto box1 = std::make_unique<translate>(
      std::make_unique<rotate_y>(
          std::make_unique<box>(point3(0, 0, 0), point3(165, 330, 165), white), 15),
      vec3d(265, 0, 295));

  auto box2 = std::make_unique<translate>(
      std::make_unique<rotate_y>(
          std::make_unique<box>(point3(0, 0, 0), point3(165, 165, 165), white), -18),
      vec3d(130, 0, 65));
  // world.add(std::move(box1));
  // world.add(std::move(box2));
  world.add(std::make_unique<constant_medium>(std::move(box1), 0.01, color(0, 0, 0)));
  world.add(std::make_unique<constant_medium>(std::move(box2), 0.01, color(1, 1, 1)));
}

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
  light.add(std::make_unique<quad>(
      point3(123, 554, 147), vec3d(300, 0, 0), vec3d(0, 0, 265), nullptr));
  world.add(std::make_unique<quad>(
      point3(123, 554, 147), vec3d(300, 0, 0), vec3d(0, 0, 265), wlight));
  // light.add(std::make_unique<xz_rect>(123, 423, 147, 412, 554, wlight));

  auto sphere_material = new lambertian(color(0.7, 0.3, 0.1));
  world.add(std::make_unique<sphere>(point3(400, 400, 200), 50, sphere_material));

  world.add(std::make_unique<sphere>(point3(260, 150, 45), 50, new dielectric(1.5)));
  world.add(
      std::make_unique<sphere>(point3(0, 150, 145), 50, new metal(color(0.8, 0.8, 0.9), 1.0)));

  world.add(std::make_unique<sphere>(point3(360, 150, 145), 70, new dielectric(1.5)));
  world.add(std::make_unique<constant_medium>(
      std::make_unique<sphere>(point3(360, 150, 145), 70, new dielectric(1.5)), 0.2,
      color(0.2, 0.4, 0.9)));
  world.add(std::make_unique<constant_medium>(
      std::make_unique<sphere>(point3(0, 0, 0), 5000, new dielectric(1.5)), .0001,
      color(1, 1, 1)));

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
      std::make_unique<rotate_y>(std::make_unique<bvh_node>(boxes2), 15),
      vec3d(-100, 270, 395)));
}

auto cornell_box_bunny_rotate(hittable_list &world, hittable_list &light) -> void {

  auto red = new lambertian(color(.65, .05, .05));
  auto white = new lambertian(color(.73, .73, .73));
  auto green = new lambertian(color(.12, .45, .15));
  auto wlight = new diffuse_light(color(20, 20, 20));
  //   auto aluminum = new metal(color(0.8, 0.85, 0.88), 0.0);

  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(std::make_unique<yz_rect>(0, 555, 0, 555, 0, red));
  auto m = nullptr;
  light.add(
      std::make_unique<quad>(point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), m));
  world.add(std::make_unique<quad>(
      point3(343, 554, 332), vec3d(-130, 0, 0), vec3d(0, 0, -105), wlight));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(std::make_unique<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(std::make_unique<xy_rect>(0, 555, 0, 555, 555, white));

  // auto aluminum = new metal(color(0.8, 0.85, 0.88), 0.0);
  // auto box1 = new box(point3(0,0,0), point3(165,330,165), aluminum);
  auto box1 = std::make_unique<translate>(
      std::make_unique<rotate_y>(
          std::make_unique<box>(point3(0, 0, 0), point3(165, 330, 165), white), 30),
      vec3d(265, 0, 295));
  world.add(std::move(box1));

  auto box2 = std::make_unique<translate>(
      std::make_unique<rotate_y>(
          std::make_unique<box>(point3(0, 0, 0), point3(165, 165, 165), white), -18),
      vec3d(130, 0, 65));
  world.add(std::move(box2));

  auto bunny = MeshTriangle("src/models/bunny/bunny.obj", 60.0f);
  std::cout << "bunny bounding box : " << bunny.bbox.min() << "    ||    " << bunny.bbox.max()
            << std::endl;
  auto bigbunny = std::make_unique<scale>(
      std::make_unique<rotate_y>(std::make_unique<bvh_node>(bunny.triangles), 180),
      vec3d(15, 15, 15));
  aabb bunnyaabb = bigbunny->bounding_box();
  auto center = bunnyaabb.center();
  auto box = std::make_unique<translate>(std::move(bigbunny), vec3d(330 + center[0], 300, 400));
  world.add(std::move(box));
  auto cow_texture = new image_texture("src/models/spot/spot_texture.png");
  auto cow_surface = new lambertian(cow_texture);
  auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 80, cow_surface);
  // auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 4);
  std::cout << "cow bounding box : " << cow.bbox.min() << "    ||    " << cow.bbox.max()
            << std::endl;
  auto rotatecow = std::make_unique<rotate_y>(std::make_unique<bvh_node>(cow.triangles), 45);
  aabb cowab = rotatecow->bounding_box();

  auto boxcow =
      std::make_unique<translate>(std::move(rotatecow), vec3d(165, 165 - cowab.min()[1], 145));
  world.add(std::move(boxcow));
}

#endif
