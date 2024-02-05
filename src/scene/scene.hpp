#ifndef SCENE_HPP
#define SCENE_HPP

#include "../geometry/hittablelist.hpp"
#include "balls_world.hpp"
#include "cornell_box.hpp"
#include "test_scene.hpp"
#include <cstdint>
#include <map>

enum OBJ {
  Sph,   // center:vec3d, radius:double, material
  Tri,   // v0:vec3d, v1:vec3d, v2:vec3d, material | t [9], material
  Quad,  // Q: vec3d, u: vec3d, v: vec3d, material | q [9], material
  List,  // list : {}
  BVH,   // list : {}
  Mesh,  // filename : "", scale, material
  Box,   // p_min : vec3d, p_max: vec3d, material
  Trans, // offset : vec3d, OBJ
  Scale, // offset : vec3d, OBJ
  R_x,   // angle : double, OBJ
  R_y,   // angle : double, OBJ
  R_z,   // angle : double, OBJ
};

//clang-format off
std::map<std::string, int> obj_map = {
    {"Sph", Sph},
    {"Tri", Tri},
    {"Quad", Quad},
    {"List", List},
    {"BVH", BVH},
    {"Mesh", Mesh},
    {"Box", Box},
    {"Trans", Trans},
    {"Scale", Scale},
    {"R_x", R_x},
    {"R_y", R_y},
    {"R_z", R_z},
    {"sphere", Sph},
    {"triangle", Tri},
    {"quad", Quad},
    {"list", List},
    {"bvh", BVH},
    {"mesh", Mesh},
    {"box", Box},
    {"transtion", Trans},
    {"scale", Trans},
    {"rotate_x", R_x},
    {"rotate_y", R_y},
    {"rotate_z", R_z},
};
//clang-format on

enum MATERIAL {
  Lambertian,    // texture
  Dielectric,    // ir: double
  Diffuse_light, // texture
  Isotropic,     // texture
  Metal,         // albedo:vec3d, fuzz: double
};
std::map<std::string, int> material_map = {
    {"LB", Lambertian}, // Texture
    {"DI", Dielectric}, //
    {"DL", Diffuse_light},
    {"IS", Isotropic},
    {"MT", Metal},
    {"Lambertian", Lambertian},
    {"Dielectric", Dielectric},
    {"Diffuse_light", Diffuse_light},
    {"Isotropic", Isotropic},
    {"Metal", Metal},
};

enum TEXTURE {
  Color,   // color : vec3d
  Checker, // Texture1, Texture2
  Image,   // filename: string
  Noise,   // scale: double
};

std::map<std::string, int> texture_map = {
    {"Color", Color}, {"Checker", Checker}, {"Image", Image}, {"Noise", Noise}};

inline auto choose_scene(uint32_t opt, hittable_list &world, hittable_list &light,
    double &aspect_ratio, uint32_t &image_width, double &vfov, point3 &lookfrom, point3 &lookat,
    color &background) -> void {

  switch (opt) {
  case 1:
    lookfrom = point3(13, 3, 3);
    lookat = point3(0, 0, 0);
    background = color(0.70, 0.80, 1.00);
    image_width = 1200;
    aspect_ratio = 3.0 / 2.0;
    vfov = 40;
    random_balls_world(world, light);
    break;
  case 2:
    lookfrom = point3(13, 3, 3);
    lookat = point3(0, 0, 0);
    background = color(0.70, 0.80, 1.00);
    image_width = 1200;
    aspect_ratio = 3.0 / 2.0;
    random_checkerANDballs(world, light);
    break;
  case 3:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    cornell_box_new(world, light);
    break;
  case 4:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    cornell_box(world, light);
    break;
  case 5:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    cornell_box_rotate(world, light);
    break;
  case 6:
    aspect_ratio = 1.0;
    image_width = 600;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    vfov = 40.0;
    cornell_smoke(world, light);
    break;
  case 7:
    aspect_ratio = 1.0;
    image_width = 800;
    background = color(0, 0, 0);
    lookfrom = point3(478, 278, -600);
    lookat = point3(278, 278, 0);
    vfov = 40.0;
    total_texture(world, light);
    break;
  case 8:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    cornell_box_bunny_rotate(world, light);
    break;
  case 9:
    aspect_ratio = 1.0;
    image_width = 600;
    vfov = 40.0;
    background = color(0, 0, 0);
    lookfrom = point3(278, 278, -800);
    lookat = point3(278, 278, 0);
    test_scene(world, light);
    break;
  default:
    random_checkerANDballs(world, light);
    break;
  }
}

#endif