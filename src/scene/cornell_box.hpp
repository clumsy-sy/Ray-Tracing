#ifndef CORNELL_BOX_HPP
#define CORNELL_BOX_HPP

#include "../geometry/hittablelist.hpp"
#include "../geometry/aarect.hpp"
#include "../geometry/box.hpp"
#include "../geometry/translate.hpp"
#include "../geometry/constant_medium.hpp"
#include "../material/diffuse_light.hpp"
#include "../material/lambertian.hpp"

auto cornell_box() -> hittable_list {
  hittable_list objects;

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
  objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
  objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

  return objects;
}

auto cornell_box_rotate() -> hittable_list {
  hittable_list objects;

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
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

  return objects;
}

auto cornell_smoke() -> hittable_list {
  hittable_list objects;

  auto red = std::make_shared<lambertian>(color(.65, .05, .05));
  auto white = std::make_shared<lambertian>(color(.73, .73, .73));
  auto green = std::make_shared<lambertian>(color(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(color(7, 7, 7));

  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  objects.add(std::make_shared<xz_rect>(113, 443, 127, 432, 554, light));
  objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
  objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

  std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = std::make_shared<rotate_y>(box1, 15);
  box1 = std::make_shared<translate>(box1, Vec3d(265, 0, 295));

  std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = std::make_shared<rotate_y>(box2, -18);
  box2 = std::make_shared<translate>(box2, Vec3d(130, 0, 65));

  objects.add(std::make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
  objects.add(std::make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

  return objects;
}

#endif
