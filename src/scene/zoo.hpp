#ifndef BUNNY_WORLD_HPP
#define BUNNY_WORLD_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/meshTriangle.hpp"
#include "../geometry/box.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/translate.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"
#include "../material/dielectric.hpp"
#include "../texture/image_texture.hpp"
#include <memory>

auto bunny_world() -> hittable_list {
  hittable_list objects;

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(color(5, 5, 5));

  objects.add(std::make_shared<yz_rect>(0, 10, -10, 10, 10, green));
  objects.add(std::make_shared<yz_rect>(0, 10, -10, 10, -10, red));
  objects.add(std::make_shared<xz_rect>(-3, 3, -3, 3, 9.999999, light)); // light
  objects.add(std::make_shared<xz_rect>(-10, 10, -10, 10, 0, white));    // bottom
  objects.add(std::make_shared<xz_rect>(-10, 10, -10, 10, 10, white));   // top
  objects.add(std::make_shared<xy_rect>(-10, 10, -10, 10, -6, white));   // back

  auto bunny = MeshTriangle("src/models/bunny/bunny.obj", 60.0f);
  std::cout << "bunny bounding box : " << bunny.box.minimum << "    ||    " << bunny.box.maximum << std::endl;

  auto bunnybox = std::make_shared<bvh_node>(bunny.triangles);
  auto box = std::make_shared<translate>(
      std::make_shared<scale>(bunnybox, Vec3d(1, 1, 1)), Vec3d(0, -bunny.box.minimum.y(), 0));
  objects.add(box);
  return objects;
}

auto cow_world() -> hittable_list {
  hittable_list objects;

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(color(10, 10, 10));

  objects.add(std::make_shared<yz_rect>(0, 10, -10, 10, 10, green));
  objects.add(std::make_shared<yz_rect>(0, 10, -10, 10, -10, red));
  objects.add(std::make_shared<xz_rect>(-3, 3, -3, 3, 9.999999, light)); // light
  objects.add(std::make_shared<xz_rect>(-10, 10, -10, 10, 0, white));    // bottom
  objects.add(std::make_shared<xz_rect>(-10, 10, -10, 10, 10, white));   // top
  objects.add(std::make_shared<xy_rect>(-10, 10, -10, 10, -6, white));   // back

  auto cow_texture = std::make_shared<image_texture>("src/models/spot/spot_texture.png");
  // auto cow_texture = std::make_shared<image_texture>("src/models/earth/earthmap.jpg");
  auto cow_surface = std::make_shared<lambertian>(cow_texture);
  auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 4, cow_surface);
  // auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 4);
  std::cout << "cow bounding box : " << cow.box.minimum << "    ||    " << cow.box.maximum << std::endl;
  auto cowbox = std::make_shared<bvh_node>(cow.triangles);
  auto rotatecow = std::make_shared<rotate_y>(cowbox, 150);
  aabb cowab;
  if (rotatecow->bounding_box(cowab)) {
    std::cout << cowab.center() << std::endl;
    auto box = std::make_shared<translate>(rotatecow, Vec3d(0, -cowab.minimum[1], 2));
    objects.add(box);
  } else {
    std::cout << "ERROR in bunny bounding box!" << std::endl;
  }

  return objects;
}

auto cornell_box_bunny_rotate() -> hittable_list {
  hittable_list objects;

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(color(20, 20, 20));

  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  objects.add(std::make_shared<xz_rect>(180, 376, 194, 365, 554, light));
  objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
  objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

  std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = std::make_shared<rotate_y>(box1, 15);
  box1 = std::make_shared<translate>(box1, Vec3d(265, 0, 295));
  objects.add(box1);

  std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = std::make_shared<rotate_y>(box2, -18);
  box2 = std::make_shared<translate>(box2, Vec3d(130, 0, 65));
  objects.add(box2);

  auto bunny = MeshTriangle("src/models/bunny/bunny.obj", 60.0f);
  std::cout << "bunny bounding box : " << bunny.box.minimum << "    ||    " << bunny.box.maximum << std::endl;
  auto bunnybox = std::make_shared<bvh_node>(bunny.triangles);
  auto rotatebunny = std::make_shared<rotate_y>(bunnybox, 180);
  auto bigbunny = std::make_shared<scale>(rotatebunny, Vec3d(15, 15, 15));
  aabb bunnyaabb;
  if (bigbunny->bounding_box(bunnyaabb)) {
    auto center = bunnyaabb.center();

    auto box = std::make_shared<translate>(bigbunny, Vec3d(330 + center[0], 300, 400));
    objects.add(box);
  } else {
    std::cout << "ERROR in bunny bounding box!" << std::endl;
  }

  auto cow_texture = std::make_shared<image_texture>("src/models/spot/spot_texture.png");
  auto cow_surface = std::make_shared<lambertian>(cow_texture);
  auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 80, cow_surface);
  // auto cow = MeshTriangle("src/models/spot/spot_triangulated_good.obj", 4);
  std::cout << "cow bounding box : " << cow.box.minimum << "    ||    " << cow.box.maximum << std::endl;
  auto cowbox = std::make_shared<bvh_node>(cow.triangles);
  auto rotatecow = std::make_shared<rotate_y>(cowbox, 45);
  aabb cowab;
  if (rotatecow->bounding_box(cowab)) {
    // std::cout << cowab.center() << std::endl;
    auto box = std::make_shared<translate>(rotatecow, Vec3d(165, 165 - cowab.minimum[1], 145));
    objects.add(box);
  } else {
    std::cout << "ERROR in bunny bounding box!" << std::endl;
  }

  return objects;
}

#endif