#include "scene/scene.hpp"
#include "camera/camera.hpp"
#include "renderer/Renderer.hpp"

auto main(int argc, const char **argv) -> int {
  std::string photoname = "image.bmp";
  if (argc >= 2) {
    photoname = std::string(argv[1]);
  }
  // photo size
  auto aspect_ratio = 3.0 / 2.0;
  int image_width = 1200;

  point3 lookfrom, lookat;
  color background;
  double vfov = 20.0;
  auto world = choose_scene(9, aspect_ratio, image_width, vfov, lookfrom, lookat, background);

  // Camera
  Vec3d vup(0, 1, 0);
  auto dist_to_focus = (lookfrom - lookat).length();
  auto aperture = 0.1;

  camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);
  // Renderer
  Renderer<camera> renderer(world, aspect_ratio, image_width);
  renderer.set_camera(cam);
  renderer.set_photo_name(photoname);
  renderer.set_samples_per_pixel(64);
  renderer.set_max_depth(5);
  renderer.set_background(background);
  renderer.render();

  return 0;
}
