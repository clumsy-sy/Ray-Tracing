/**
 * @file scene_parse.hpp
 * @brief 解析 JSON 文件
 *
 */
#ifndef SCENE_PARSE_HPP
#define SCENE_PARSE_HPP
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

#include "../external/cJSON.h"
#include "../camera/camera.hpp"
#include "../renderer/Renderer.hpp"
#include "scene.hpp"

class scene {
private:
  // use exsit scene
  int scene_id;
  // image parms
  std::string image_name;
  std::uint32_t image_width;
  std::uint32_t image_height;
  double aspect_ratio;
  // camera parms
  point3 lookfrom;
  vec3d lookat;
  vec3d vup;
  double vfov;
  double aperture;
  // render parms
  hittable_list *world;
  hittable_list *light;
  color background;
  std::uint32_t pps;
  std::uint32_t threads;
  std::unique_ptr<Renderer<camera>> renderer;
  // json parse
  cJSON *root;

private:
  auto parse_scene_id() -> void;
  auto parse_image_name() -> void;
  auto parse_image_size() -> void;
  auto parse_lookfrom() -> void;
  auto parse_lookat() -> void;
  auto parse_vup() -> void;
  auto parse_vfov() -> void;
  auto parse_aperture() -> void;
  auto parse_object() -> void;
  auto parse_background() -> void;
  auto parse_pps() -> void;
  auto parse_threads() -> void;

public:
  scene() {
    world = new hittable_list();
    light = new hittable_list();
  }
  auto scene_parse(const std::string &json_path) -> bool;
  auto generate() -> void;
  auto set_log() -> void;
};

auto scene::parse_scene_id() -> void {
  auto item = cJSON_GetObjectItem(root, "scene_id");
  if (item != nullptr) {
    scene_id = item->valueint;
#ifdef LOG
    std::cout << "[scene id]:" << scene_id << "\n";
#endif
  } else {
    scene_id = -1;
  }
}
auto scene::parse_image_name() -> void {
  auto item = cJSON_GetObjectItem(root, "image_name");
  if (item != nullptr) {
    image_name = item->valuestring;
#ifdef LOG
    std::cout << "[img name]: " << image_name << "\n";
#endif
  } else {
    image_name = "img.bmp";
  }
}
auto scene::parse_image_size() -> void {
  auto width = cJSON_GetObjectItem(root, "width");
  auto height = cJSON_GetObjectItem(root, "height");
  auto ratio = cJSON_GetObjectItem(root, "ratio");
  if (width != nullptr && height != nullptr && ratio != nullptr) {
    image_width = width->valueint;
    image_height = height->valueint;
    aspect_ratio = (double)image_width / image_height;
  } else if (width != nullptr && height != nullptr) {
    image_width = width->valueint;
    image_height = height->valueint;
    aspect_ratio = (double)image_width / image_height;
  } else if (width != nullptr && ratio != nullptr) {
    image_width = width->valueint;
    aspect_ratio = ratio->valuedouble;
    image_height = static_cast<uint32_t>(image_width / aspect_ratio);
  } else if (height != nullptr && ratio != nullptr) {
    image_height = height->valueint;
    aspect_ratio = ratio->valuedouble;
    image_width = static_cast<uint32_t>(image_height * aspect_ratio);
  } else {
    image_width = 600;
    image_height = 800;
    aspect_ratio = (double)image_width / image_height;
  }
}

auto scene::parse_lookfrom() -> void {
  auto lookfrom_raw = cJSON_GetObjectItem(root, "lookfrom");
  if (lookfrom_raw == nullptr) {
    lookfrom = vec3d(0, 0, 0);
  } else {
    if (cJSON_IsArray(lookfrom_raw)) {
      int arraySize = cJSON_GetArraySize(lookfrom_raw);
      if (arraySize != 3)
        std::cerr << " ERROR lookfrom size!\n";
      for (int i = 0; i < arraySize; i++) {
        cJSON *element = cJSON_GetArrayItem(lookfrom_raw, i);
        lookfrom[i] = element->valuedouble;
      }
    } else {
      std::cerr << "lookfrom is not array";
    }
  }
}
auto scene::parse_lookat() -> void {
  auto lookat_raw = cJSON_GetObjectItem(root, "lookat");
  if (lookat_raw == nullptr) {
    lookat = vec3d(0, 0, 0);
  } else {
    if (cJSON_IsArray(lookat_raw)) {
      int arraySize = cJSON_GetArraySize(lookat_raw);
      if (arraySize != 3)
        std::cerr << " ERROR lookat size!\n";
      for (int i = 0; i < arraySize; i++) {
        cJSON *element = cJSON_GetArrayItem(lookat_raw, i);
        lookat[i] = element->valuedouble;
      }
    } else {
      std::cerr << "lookat is not array";
    }
  }
}
auto scene::parse_vup() -> void {
  auto vup_raw = cJSON_GetObjectItem(root, "vup");
  if (vup_raw == nullptr) {
    vup = vec3d(0, 1, 0);
  } else {
    if (cJSON_IsArray(vup_raw)) {
      int arraySize = cJSON_GetArraySize(vup_raw);
      if (arraySize != 3)
        std::cerr << " ERROR vup size!\n";
      for (int i = 0; i < arraySize; i++) {
        cJSON *element = cJSON_GetArrayItem(vup_raw, i);
        vup[i] = element->valuedouble;
      }
    } else {
      std::cerr << "vup is not array";
    }
  }
}
auto scene::parse_vfov() -> void {
  auto item = cJSON_GetObjectItem(root, "vfov");
  if (item != nullptr) {
    vfov = item->valuedouble;
  } else {
    vfov = 40.0;
  }
}
auto scene::parse_aperture() -> void {
  auto item = cJSON_GetObjectItem(root, "aperture");
  if (item != nullptr) {
    aperture = item->valuedouble;
  } else {
    aperture = 0.1;
  }
}
auto scene::parse_object() -> void {}
auto scene::parse_background() -> void {
  auto background_raw = cJSON_GetObjectItem(root, "background");
  if (background_raw == nullptr) {
    background = vec3d(0, 0, 0);
  } else {
    if (cJSON_IsArray(background_raw)) {
      int arraySize = cJSON_GetArraySize(background_raw);
      if (arraySize != 3)
        std::cerr << " ERROR background size!\n";
      for (int i = 0; i < arraySize; i++) {
        cJSON *element = cJSON_GetArrayItem(background_raw, i);
        background[i] = element->valuedouble;
      }
    } else {
      std::cerr << "background is not array";
    }
  }
}
auto scene::parse_pps() -> void {
  auto item = cJSON_GetObjectItem(root, "pps");
  if (item != nullptr) {
    pps = item->valueint;
  } else {
    pps = 400;
  }
}
auto scene::parse_threads() -> void {
  auto item = cJSON_GetObjectItem(root, "threads");
  if (item != nullptr) {
    threads = item->valueint;
  } else {
    threads = 16 * 16;
  }
}
auto scene::scene_parse(const std::string &json_path) -> bool {
  // 打开 JSON 文件
  std::ifstream file(json_path);
  if (!file) {
    std::cerr << "Error opening JSON file." << std::endl;
    return false;
  }

  // 读取文件内容到字符串
  std::string jsonString(
      (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  // 关闭文件
  file.close();

  // 解析 JSON 数据
  root = cJSON_Parse(jsonString.c_str());
  parse_scene_id();
  parse_image_name();
  if (scene_id == -1) {
    parse_image_size();
    parse_lookfrom();
    parse_lookat();
    parse_vup();
    parse_vfov();
    parse_aperture();
    parse_object();
    parse_background();
    parse_pps();
    parse_threads();
  } else {
    choose_scene(scene_id, *world, *light, aspect_ratio, image_width, vfov, lookfrom, lookat,
        background);
  }
#ifdef DEBUG
  std::cout << *world << "\n";
  std::cout << *light << "\n";
#endif
  vec3d vup(0, 1, 0);
  auto aperture = 0.1;

  camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture);
  renderer = std::make_unique<Renderer<camera>>(*world, *light, aspect_ratio, image_width);
  renderer->set_camera(cam);
  renderer->set_photo_name(image_name);
  renderer->set_samples_per_pixel(400);
  renderer->set_max_depth(10);
  renderer->set_background(background);
  // renderer->set_async_num(1);
  renderer->set_async_num(16 * 16);
  // 释放内存
  cJSON_Delete(root);

  return true;
}
auto scene::generate() -> void {
#ifdef LOG
  std::cout << *renderer;
#endif
  renderer->render();
  // renderer->render_single();
}

#endif