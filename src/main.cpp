#define LOG

#include "scene/scene.hpp"
#include "camera/camera.hpp"
#include "renderer/Renderer.hpp"
#include "geometry/hittablelist.hpp"

auto main(int argc, const char **argv) -> int {
  // 命令行读入参数为生成的图片命名
  std::string photoname = "Img.bmp";
  if (argc >= 2) {
    photoname = std::string(argv[1]);
  }
  // photo size
  auto aspect_ratio = 3.0 / 2.0;
  int image_width = 1200;
  // 选择场景，同时确定摄像机位置，焦点位置，视角，背景辐射 等信息
  point3 lookfrom, lookat;
  color background;
  double vfov = 40.0;
  auto world = new hittable_list();
  auto light = new hittable_list();
  choose_scene(11, *world, *light, aspect_ratio, image_width, vfov, lookfrom, lookat, background);

  // Camera
  vec3d vup(0, 1, 0);
  auto aperture = 0.1;

  camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture);
  // Renderer
  Renderer<camera> renderer(*world, *light, aspect_ratio, image_width);
  renderer.set_camera(cam);
  renderer.set_photo_name(photoname);
  renderer.set_samples_per_pixel(512);
  renderer.set_max_depth(5);
  renderer.set_background(background);
  // renderer.set_async_num(1);
  renderer.render();

  return 0;
}
