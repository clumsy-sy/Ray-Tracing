#ifndef SCENE_HPP
#define SCENE_HPP

#include "../geometry/hittablelist.hpp"
// #include "balls_world.hpp"
// #include "checker_balls.hpp"
#include "cornell_box.hpp"
// #include "simple_light.hpp"
// #include "test_scene.hpp"
// #include "total_texture.hpp"
// #include "zoo.hpp"

inline auto choose_scene(uint32_t opt, hittable_list &world, hittable_list &light, double &aspect_ratio,
    int &image_width, double &vfov, point3 &lookfrom, point3 &lookat, color &background) -> void {
  // lookfrom = point3(13, 2, 3);
  lookfrom = point3(13, 3, 3);
  lookat = point3(0, 0, 0);
  background = color(0.70, 0.80, 1.00);

  switch (opt) {
  // case 1:
  //   random_balls_world(world, light);
  //   break;
  // case 2:
  //   random_checkerANDballs(world, light);
  //   break;
  // case 3:
  //   background = color(0, 0, 0);
  //   lookfrom = point3(26, 3, 6);
  //   lookat = point3(0, 2, 0);
  //   simple_light(world, light);
  //   break;
  // case 4:
  //   aspect_ratio = 1.0;
  //   image_width = 600;
  //   vfov = 40.0;
  //   background = color(0, 0, 0);
  //   lookfrom = point3(278, 278, -800);
  //   lookat = point3(278, 278, 0);
  //   cornell_box_new(world, light);
    break;
  case 5:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    cornell_box(world, light);
    break;
  // case 6:
  //   aspect_ratio = 1.0;
  //   image_width = 600;
  //   vfov = 40.0;
  //   background = color(0, 0, 0);
  //   lookfrom = point3(278, 278, -800);
  //   lookat = point3(278, 278, 0);
  //   cornell_box_rotate(world, light);
  //   break;
  // case 7:
  //   aspect_ratio = 1.0;
  //   image_width = 600;
  //   background = color(0, 0, 0);
  //   lookfrom = point3(278, 278, -800);
  //   lookat = point3(278, 278, 0);
  //   vfov = 40.0;
  //   cornell_smoke(world, light);
  //   break;
  // case 8:
  //   aspect_ratio = 1.0;
  //   image_width = 800;
  //   background = color(0, 0, 0);
  //   lookfrom = point3(478, 278, -600);
  //   lookat = point3(278, 278, 0);
  //   vfov = 40.0;
  //   total_texture(world, light);
  //   break;
  // case 9:
  //   aspect_ratio = (double)1280 / 960;
  //   image_width = 1280;
  //   vfov = 60.0;
  //   background = color(0.0, 0.0, 0.0);
  //   lookfrom = point3(-1, 5, 16);
  //   lookat = point3(-1, 5, 0);
  //   bunny_world(world, light);
  //   break;
  // case 10:
  //   aspect_ratio = (double)1280 / 960;
  //   image_width = 1280;
  //   vfov = 60.0;
  //   background = color(0.0, 0.0, 0.0);
  //   lookfrom = point3(-1, 5, 16);
  //   lookat = point3(-1, 5, 0);
  //   cow_world(world, light);
  //   break;
  // case 11:
  //   aspect_ratio = 1.0;
  //   image_width = 600;
  //   vfov = 40.0;
  //   background = color(0, 0, 0);
  //   lookfrom = point3(278, 278, -800);
  //   lookat = point3(278, 278, 0);
  //   cornell_box_bunny_rotate(world, light);
  //   break;
  // case 12:
  //   aspect_ratio = 1.0;
  //   image_width = 600;
  //   vfov = 40.0;
  //   background = color(0, 0, 0);
  //   lookfrom = point3(278, 278, -800);
  //   lookat = point3(278, 278, 0);
  //   test_scene(world, light);
  //   break;
  // default:
  //   random_checkerANDballs(world, light);
    break;
  }
}

#endif