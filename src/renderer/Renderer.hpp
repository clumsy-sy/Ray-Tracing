#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../external/rtw_stb_image_write.h"
#include "../global.hpp"
#include "../external/BMP.hpp"
#include "../camera/camerabase.hpp"
#include "../geometry/hittablelist.hpp"
#include "../material/material.hpp"

template <class Camera>
class Renderer {
public:
  std::string photoname = "Img.bmp";
  static_assert(
      std::is_base_of<camerabase, Camera>::value, "Camera not derived from camerabase");
  Camera cam;
  hittable_list &world, &light;
  double aspect_ratio = 16.0 / 9.0;
  uint32_t image_width = 1200;
  uint32_t image_height = static_cast<uint32_t>(image_width / aspect_ratio);
  uint32_t samples_per_pixel = 64;   // 单素采样数
  uint32_t max_depth = 10;           // 光线递归深度
  uint32_t async_num = 36;           // 线程数
  color background = color(0, 0, 0); // 背景辐射

public:
  Renderer() = default;
  Renderer(hittable_list &hitlist) : world(hitlist), cam(){};
  Renderer(hittable_list &hitlist, hittable_list &lightlist, double ratio, uint32_t width)
      : world(hitlist), light(lightlist), aspect_ratio(ratio), image_width(width) {
    image_height = static_cast<uint32_t>(image_width / aspect_ratio);
  }
  // clang-format off
  auto set_camera(Camera &c) { cam = c; }
  auto set_photo_name(std::string name) { photoname = std::move(name); }
  auto set_samples_per_pixel(uint32_t samples) { samples_per_pixel = samples; }
  auto set_max_depth(uint32_t depth) { max_depth = depth; }
  auto set_async_num(uint32_t num) { async_num = num; }
  auto set_background(const color &c) { background = c;}
  // clang-format on
  auto render() {
    bmp::bitmap photo(image_width, image_height); // photo
    std::vector<std::future<void>> deque;         // thread deque
    std::mutex cout_mutex;                        // lock the cnt and std::cout
    std::int32_t cnt = 0;
    // 开始渲染和显示进度
    UpdateProgress(cnt, image_height - 1);
    // 各像素片渲染函数，
    auto action = [&](uint32_t jl, uint32_t jr) -> void {
      for (uint32_t j = jl; j < jr; ++j) {
        for (uint32_t i = 0; i < image_width; ++i) {
          color pixel_color = simple_random_sampling(i, j);
          // color pixel_color = sqrt_random_sampling(i, j);
          photo.set_RGB(i, j, pixel_color, samples_per_pixel);
        }
        cout_mutex.lock();
        UpdateProgress(++cnt, image_height - 1);
        cout_mutex.unlock();
      }
    };
    /*
      线程任务划分：图像高度 / 线程数，计算完任务片后将线程启用并塞入队列
    */
    uint32_t block = image_height / async_num;
    uint32_t superfluous = image_height - block * async_num;
    for (uint32_t ti = 0, k = 0; ti != async_num; ++ti) {
      uint32_t jl = ti * block + k, jr = (ti + 1) * block + k;
      if (k < superfluous) {
        jr++, k++;
      }
      deque.emplace_back(std::async(std::launch::async, action, jl, jr));
    }
    // 等待各个线程都完成
    for (auto &i : deque) {
      // 逐渐输出图片
      // stbi_flip_vertically_on_write(true);
      // auto data = stbi_write_bmp(photoname.c_str(), image_width, image_height, 3,
      // photo.image.data()); if (!data) {
      //   std::cerr << "ERROR: Could not load texture image file '" << photoname << "'.\n";
      // }
      i.wait();
    }
    // 图像生成 set_RGB
    stbi_flip_vertically_on_write(true);
    auto data =
        stbi_write_bmp(photoname.c_str(), image_width, image_height, 3, photo.image.data());
    if (!data) {
      std::cerr << "ERROR: Could not load texture image file '" << photoname << "'.\n";
    }
    // 自己写的 bmp 输出
    // photo.generate(photoname);
  }
  // 普通的采样，在一个像素格内采样 samples_per_pixel 次
  auto simple_random_sampling(uint32_t i, uint32_t j) -> color {
    color res(0, 0, 0);
    for (uint32_t s = 0; s < samples_per_pixel; ++s) {
      auto u = (i + random_double()) / (image_width - 1);
      auto v = (j + random_double()) / (image_height - 1);
      ray r = cam.get_ray(u, v);
      // res += ray_color(r, world, max_depth);
      res += ray_color(r, world, light, max_depth);
    }
    return res;
  }
  auto sqrt_random_sampling(uint32_t i, uint32_t j) -> color {
    thread_local uint32_t N = std::sqrt(samples_per_pixel);
    color res(0, 0, 0);
    for (uint32_t di = 0; di < N; ++di) {
      for (uint32_t dj = 0; dj < N; ++dj) {
        auto u = (i + (di + random_double()) / N) / (image_width - 1);
        auto v = (j + (dj + random_double()) / N) / (image_height - 1);
        ray r = cam.get_ray(u, v);
        // res += ray_color(r, world, max_depth);
        res += ray_color(r, world, light, max_depth);
      }
    }
    return res;
  }
  // 发射光线返回得到颜色
  auto ray_color(const ray &r, const hittable &world, const hittable &lights, int depth)
      -> color {
    // 递归次数限制
    if (depth <= 0)
      return {0, 0, 0};
    hit_record rec;
    // 如果光线什么都没有击中，则返回背景颜色
    if (!world.hit(r, interval(0.001, infinity), rec))
      return background;

    scatter_record srec;
    color color_from_emission = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, srec))
      return color_from_emission;

    if (srec.skip_pdf) {
      return srec.attenuation * ray_color(srec.skip_pdf_ray, world, lights, depth - 1);
    }

    auto light_ptr = new hittable_pdf(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);
    ray scattered = ray(rec.p, p.generate());
    auto pdf_val = p.value(scattered.direction());

    double scattering_pdf = rec.mat_ptr->scattering_pdf(r, rec, scattered);

    color sample_color = ray_color(scattered, world, lights, depth - 1);
    color color_from_scatter = (srec.attenuation * scattering_pdf * sample_color) / pdf_val;

    return color_from_emission + color_from_scatter;
  }
};

#endif