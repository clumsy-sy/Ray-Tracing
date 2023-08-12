#ifndef SCENE_HPP
#define SCENE_HPP

#include "../geometry/hittablelist.hpp"
#include "balls_world.hpp"
#include "checker_balls.hpp"
#include "cornell_box.hpp"
#include "earth.hpp"
#include "simple_light.hpp"
#include "test_scene.hpp"
#include "total_texture.hpp"
#include "zoo.hpp"

inline auto choose_scene(uint32_t opt, double &aspect_ratio, int &image_width, double &vfov, point3 &lookfrom,
    point3 &lookat, color &background) -> hittable_list {
  // lookfrom = point3(13, 2, 3);
  lookfrom = point3(13, 3, 3);
  lookat = point3(0, 0, 0);
  background = color(0.70, 0.80, 1.00);

  switch (opt) {
  case 1:
    return random_balls_world();
  case 2:
    background = color(0.70, 0.80, 1.00);
    return random_checkerANDballs();
  case 3:
    return earth();
  case 4:
    background = color(0, 0, 0);
    lookfrom = point3(26, 3, 6);
    lookat = point3(0, 2, 0);
    return simple_light();
  case 5:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    return cornell_box();
  case 6:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    return cornell_box_rotate();
  case 7:
    aspect_ratio = 1.0;
    image_width = 600;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    vfov = 40.0;
    return cornell_smoke();
  case 8:
    aspect_ratio = 1.0;
    image_width = 800;
    background = color(0, 0, 0);
    lookfrom = point3(478, 278, -600);
    lookat = point3(278, 278, 0);
    vfov = 40.0;
    return total_texture();
  case 9:
    aspect_ratio = (double)1280 / 960;
    image_width = 1280;
    vfov = 60.0;
    background = color(0.0, 0.0, 0.0);
    lookfrom = point3(-1, 5, 16);
    lookat = point3(-1, 5, 0);
    return bunny_world();
  case 10:
    aspect_ratio = (double)1280 / 960;
    image_width = 1280;
    vfov = 60.0;
    background = color(0.0, 0.0, 0.0);
    lookfrom = point3(-1, 5, 16);
    lookat = point3(-1, 5, 0);
    return cow_world();
  case 11:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    return cornell_box_bunny_rotate();
  case 12:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    return test_scene();
  default:
    return random_checkerANDballs();
  }
}

inline auto choose_scene(uint32_t opt) -> hittable_list {

  switch (opt) {
  case 1:
    return random_balls_world();
  case 2:
    return random_checkerANDballs();
  case 3:
    return earth();
  case 4:
    return simple_light();
  case 5:
    return cornell_box();
  case 6:
    return cornell_box_rotate();
  case 7:
    return cornell_smoke();
  case 8:
    return total_texture();
  case 9:
    return bunny_world();
  case 10:
    return test_scene();
  default:
    return random_checkerANDballs();
  }
}

#endif