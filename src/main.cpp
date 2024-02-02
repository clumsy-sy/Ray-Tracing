#define LOG
#define DEBUG

#include "scene/scene_parse.hpp"
#include "scene/scene.hpp"
#include "camera/camera.hpp"
#include "renderer/Renderer.hpp"
#include "geometry/hittablelist.hpp"



auto main(int argc, const char **argv) -> int {
  scene Scene;

  // 命令行读入参数为生成的图片命名
  std::string json_file = "examples/test.json";
  if (argc >= 2) {
    json_file = std::string(argv[1]);
  }
  if(!Scene.scene_parse(json_file)) {
    std::cerr << "can not parse json_file " << json_file << "\n";
  }
  Scene.generate();

  // // photo size
  // auto aspect_ratio = 3.0 / 2.0;
  // uint32_t image_width = 1200;
  // // 选择场景，同时确定摄像机位置，焦点位置，视角，背景辐射 等信息
  // point3 lookfrom, lookat;
  // color background;
  // double vfov = 40.0;
  // auto world = new hittable_list();
  // auto light = new hittable_list();
  // choose_scene(
  //     4, *world, *light, aspect_ratio, image_width, vfov, lookfrom, lookat, background);

  // // Camera
  // vec3d vup(0, 1, 0);
  // auto aperture = 0.1;

  // camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture);
  // // Renderer
  // Renderer<camera> renderer(*world, *light, aspect_ratio, image_width);
  // renderer.set_camera(cam);
  // renderer.set_photo_name("Img.bmp");
  // renderer.set_samples_per_pixel(1000);
  // renderer.set_max_depth(10);
  // renderer.set_background(background);
  // renderer.set_async_num(16 * 16);
  // renderer.render();

  return 0;
}
