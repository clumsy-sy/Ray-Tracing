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
#include <set>
#include <unordered_map>
#include <utility>

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
  std::uint32_t max_depth;
  std::uint32_t pps;
  std::uint32_t threads;
  std::unique_ptr<Renderer<camera>> renderer;
  // json parse
  cJSON *root;
  std::set<std::string> json_set;
  std::unordered_map<std::string, texture *> tex_map;
  std::unordered_map<std::string, material *> mat_map;

private:
  // basic
  auto parse_vec3d(cJSON *) -> std::pair<std::string, vec3d>;
  // parse
  auto parse_inherit(cJSON *sub_root) -> void;
  auto parse_scene_id(cJSON *sub_root) -> void;
  auto parse_image_name(cJSON *sub_root) -> void;
  auto parse_image_size(cJSON *sub_root) -> void;
  auto parse_lookfrom(cJSON *sub_root) -> void;
  auto parse_lookat(cJSON *sub_root) -> void;
  auto parse_vup(cJSON *sub_root) -> void;
  auto parse_vfov(cJSON *sub_root) -> void;
  auto parse_aperture(cJSON *sub_root) -> void;
  auto parse_texture(cJSON *sub_root) -> void;
  auto parse_material(cJSON *sub_root) -> void;
  auto parse_object_once(cJSON *item) -> std::unique_ptr<hittable>;
  auto parse_object(cJSON *sub_root) -> void;
  auto parse_background(cJSON *sub_root) -> void;
  auto parse_max_depth(cJSON *sub_root) -> void;
  auto parse_pps(cJSON *sub_root) -> void;
  auto parse_threads(cJSON *sub_root) -> void;

public:
  scene() {
    scene_id = -1;
    image_name = "img.bmp";
    image_width = 600;
    image_height = 600;
    aspect_ratio = 1.0;
    lookfrom = point3(0.0, 0.0, 0.0);
    lookat = point3(0.0, 0.0, 0.0);
    vup = point3(0.0, 0.0, 0.0);
    vfov = 40.0;
    aperture = 0.1;
    background = color(0.0, 0.0, 0.0);
    max_depth = 5;
    pps = 64;
    threads = 64;
    world = new hittable_list();
    light = new hittable_list();
  }

  auto scene_parse(const std::string &json_path) -> bool;
  auto scene_parse_sub(const std::string &json_path) -> bool;
  auto generate() -> void;
  auto generate_single_thread() -> void;
};

auto scene::parse_vec3d(cJSON *vec) -> std::pair<std::string, vec3d> {
  vec3d now;
  if (vec == nullptr) {
    return std::make_pair("not exist!", now);
  }
  if (cJSON_IsArray(vec)) {
    int arraySize = cJSON_GetArraySize(vec);
    if (arraySize != 3) {
      return std::make_pair("array size != 3 ", now);
    }
    for (int i = 0; i < arraySize; i++) {
      cJSON *element = cJSON_GetArrayItem(vec, i);
      now[i] = element->valuedouble;
    }
    return std::make_pair("", now);
  } else {
    return std::make_pair("not a array!", now);
  }
}

auto scene::parse_inherit(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "inherit");
  if (item != nullptr) {
    if (cJSON_IsArray(item)) {
      int arraySize = cJSON_GetArraySize(item);
      for (int i = 0; i < arraySize; i++) {
        cJSON *element = cJSON_GetArrayItem(item, i);
        if (json_set.find(element->valuestring) == json_set.end()) {
          json_set.insert(element->valuestring);
          scene_parse_sub(element->valuestring);
        }
      }
    } else {
      if (json_set.find(item->valuestring) == json_set.end()) {
        json_set.insert(item->valuestring);
        scene_parse_sub(item->valuestring);
      }
    }
  }
}

auto scene::parse_scene_id(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "scene_id");
  if (item != nullptr) {
    scene_id = item->valueint;
#ifdef LOG
    std::cout << "[scene id]:" << scene_id << "\n";
#endif
  }
}
auto scene::parse_image_name(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "image_name");
  if (item != nullptr) {
    image_name = item->valuestring;
  }
}
auto scene::parse_image_size(cJSON *sub_root) -> void {
  auto width = cJSON_GetObjectItem(sub_root, "width");
  auto height = cJSON_GetObjectItem(sub_root, "height");
  auto ratio = cJSON_GetObjectItem(sub_root, "ratio");
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
  }
}

auto scene::parse_lookfrom(cJSON *sub_root) -> void {
  auto lookfrom_raw = cJSON_GetObjectItem(sub_root, "lookfrom");
  if (lookfrom_raw) {
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
auto scene::parse_lookat(cJSON *sub_root) -> void {
  auto lookat_raw = cJSON_GetObjectItem(sub_root, "lookat");
  if (lookat_raw) {
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
auto scene::parse_vup(cJSON *sub_root) -> void {
  auto vup_raw = cJSON_GetObjectItem(sub_root, "vup");
  if (vup_raw) {
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
auto scene::parse_vfov(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "vfov");
  if (item != nullptr) {
    vfov = item->valuedouble;
  }
}
auto scene::parse_aperture(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "aperture");
  if (item != nullptr) {
    aperture = item->valuedouble;
  }
}

auto scene::parse_texture(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "texture");
  cJSON *child;
  cJSON_ArrayForEach(child, item) {
    auto type = cJSON_GetObjectItem(child, "type");
    if (type != nullptr) {
      std::string type_str = type->valuestring;
      auto find_it = texture_map.find(type_str);
      if (find_it != texture_map.end()) {
        std::string tex_name = child->string;
        if (find_it->second == Color) {
          auto color = cJSON_GetObjectItem(child, "color");
          auto result = parse_vec3d(color);
          if (result.first != "") {
            std::cerr << find_it->first << " has error members! " << result.first << "\n";
          }
          texture *t = new solid_color(result.second);
          tex_map.insert({tex_name, t});
        } else if (find_it->second == Checker) {
          auto tex1_raw = cJSON_GetObjectItem(child, "tex1");
          auto tex2_raw = cJSON_GetObjectItem(child, "tex2");
          if (tex1_raw && tex2_raw) {
            std::string tex1_name = tex1_raw->valuestring;
            std::string tex2_name = tex2_raw->valuestring;
            auto find_tex1 = tex_map.find(tex1_name);
            auto find_tex2 = tex_map.find(tex2_name);
            if (find_tex1 != tex_map.end() && find_tex2 != tex_map.end()) {
              texture *t = new checker_texture(find_tex1->second, find_tex2->second);
              tex_map.insert({tex_name, t});
            } else {
              std::cerr << find_it->first << "can not find tex1 and tex2\n";
            }
          } else {
            std::cerr << find_it->first << "can not get tex1 and tex2\n";
          }
        } else if (find_it->second == Image) {
          auto filename_raw = cJSON_GetObjectItem(child, "filename");
          if (filename_raw) {
            auto file = filename_raw->valuestring;
            texture *t = new image_texture(file);
            tex_map.insert({tex_name, t});
          } else {
            std::cerr << find_it->first << "can not get filename\n";
          }
        } else if (find_it->second == Noise) {
          auto scale_raw = cJSON_GetObjectItem(child, "scale");
          if (scale_raw) {
            double scale = scale_raw->valuedouble;
            texture *t = new noise_texture(scale);
            tex_map.insert({tex_name, t});
          } else {
            std::cerr << find_it->first << "can not get scale\n";
          }
        }
      }
    }
  }
}

auto scene::parse_material(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "material");
  cJSON *child = nullptr;
  cJSON_ArrayForEach(child, item) {
    auto type = cJSON_GetObjectItem(child, "type");
    if (type != nullptr) {
      std::string type_str = type->valuestring;
      auto find_it = material_map.find(type_str);
      if (find_it != material_map.end()) {
        std::string mat_name = child->string;
        switch (find_it->second) {
        case Lambertian: {
          auto tex_raw = cJSON_GetObjectItem(child, "tex");
          if (tex_raw) {
            std::string tex = tex_raw->valuestring;
            auto find_tex = tex_map.find(tex);
            if (find_tex != tex_map.end()) {
              material *mat = new lambertian(find_tex->second);
              mat_map.insert({mat_name, mat});
            } else {
              std::cerr << find_it->first << "can not find tex\n";
            }
          } else {
            std::cerr << find_it->first << "can not get tex\n";
          }

        } break;
        case Dielectric: {
          auto ir_raw = cJSON_GetObjectItem(child, "ir");
          if (ir_raw) {
            double ir = ir_raw->valuedouble;
            material *mat = new dielectric(ir);
            mat_map.insert({mat_name, mat});
          } else {
            std::cerr << find_it->first << "can not get ir\n";
          }
        } break;
        case Diffuse_light: {
          auto tex_raw = cJSON_GetObjectItem(child, "tex");
          if (tex_raw) {
            std::string tex = tex_raw->valuestring;
            auto find_tex = tex_map.find(tex);
            if (find_tex != tex_map.end()) {
              material *mat = new diffuse_light(find_tex->second);
              mat_map.insert({mat_name, mat});
            } else {
              std::cerr << find_it->first << "can not find tex\n";
            }
          } else {
            std::cerr << find_it->first << "can not get tex\n";
          }
        } break;
        case Isotropic: {
          auto tex_raw = cJSON_GetObjectItem(child, "tex");
          if (tex_raw) {
            std::string tex = tex_raw->valuestring;
            auto find_tex = tex_map.find(tex);
            if (find_tex != tex_map.end()) {
              material *mat = new isotropic(find_tex->second);
              mat_map.insert({mat_name, mat});
            } else {
              std::cerr << find_it->first << "can not find tex\n";
            }
          } else {
            std::cerr << find_it->first << "can not get tex\n";
          }
        } break;
        case Metal: {
          auto color = cJSON_GetObjectItem(child, "color");
          auto fuzz_raw = cJSON_GetObjectItem(child, "fuzz");
          auto result = parse_vec3d(color);
          if (result.first != "") {
            std::cerr << find_it->first << " has error members! " << result.first << "\n";
          }
          if (fuzz_raw) {
            double fuzz = fuzz_raw->valuedouble;
            material *mat = new metal(result.second, fuzz);
            mat_map.insert({mat_name, mat});
          } else {
            std::cerr << find_it->first << "can not get fuzz\n";
          }
        } break;
        }
      }
    }
  }
}

auto scene::parse_object_once(cJSON *child) -> std::unique_ptr<hittable> {
  std::unique_ptr<hittable> hit;
  auto type = cJSON_GetObjectItem(child, "type");
  if (type != nullptr) {
    std::string type_str = type->valuestring;
    auto find_it = obj_map.find(type_str);
    if (find_it != obj_map.end()) {
      switch (find_it->second) {
      case Sph: {
        auto center_raw = cJSON_GetObjectItem(child, "center");
        auto radius_raw = cJSON_GetObjectItem(child, "radius");
        auto material_raw = cJSON_GetObjectItem(child, "material");
        if (material_raw == nullptr) {
          std::cerr << find_it->first << " lack 'material' :  \n";
          exit(-1);
        }
        auto result = parse_vec3d(center_raw);
        if (result.first != "") {
          std::cerr << find_it->first << " has error members! " << result.first << "\n";
        } else if (radius_raw && material_raw) {
          auto find_mat = mat_map.find(material_raw->valuestring);

          if (find_mat != mat_map.end()) {
            hit = std::make_unique<sphere>(
                result.second, radius_raw->valuedouble, find_mat->second);
          } else {
            std::cerr << find_it->first << " can not find mat\n";
          }
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      case Tri: {
        auto Q_raw = cJSON_GetObjectItem(child, "Q");
        auto u_raw = cJSON_GetObjectItem(child, "u");
        auto v_raw = cJSON_GetObjectItem(child, "v");
        auto result0 = parse_vec3d(Q_raw);
        auto result1 = parse_vec3d(u_raw);
        auto result2 = parse_vec3d(v_raw);
        if (result0.first == "" && result1.first == "" && result2.first == "") {
          auto material_raw = cJSON_GetObjectItem(child, "material");
          if (material_raw == nullptr) {
            std::cerr << find_it->first << " lack 'material' :  \n";
            exit(-1);
          }
          auto find_mat = mat_map.find(material_raw->valuestring);

          if (find_mat != mat_map.end()) {
            hit = std::make_unique<triangle>(
                result0.second, result1.second, result2.second, find_mat->second);
          } else {
            std::cerr << find_it->first << " can not find mat\n";
          }
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      case Quad: {
        auto v0_raw = cJSON_GetObjectItem(child, "Q");
        auto v1_raw = cJSON_GetObjectItem(child, "u");
        auto v2_raw = cJSON_GetObjectItem(child, "v");
        auto result0 = parse_vec3d(v0_raw);
        auto result1 = parse_vec3d(v1_raw);
        auto result2 = parse_vec3d(v2_raw);
        if (result0.first == "" && result1.first == "" && result2.first == "") {
          auto material_raw = cJSON_GetObjectItem(child, "material");
          if (material_raw == nullptr) {
            std::cerr << find_it->first << " lack 'material' :  \n";
            exit(-1);
          }
          auto find_mat = mat_map.find(material_raw->valuestring);

          if (find_mat != mat_map.end()) {
            hit = std::make_unique<quad>(
                result0.second, result1.second, result2.second, find_mat->second);
          } else {
            std::cerr << find_it->first << " can not find mat\n";
          }
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }

      } break;
      case List: {
        auto list_raw = cJSON_GetObjectItem(child, "list");
        int arraysize = cJSON_GetArraySize(list_raw);
        for (int i = 0; i < arraysize; i++) {
          cJSON *obj = cJSON_GetArrayItem(list_raw, i);
          parse_object_once(obj);
        }
      } break;
      case BVH: {
        auto list_raw = cJSON_GetObjectItem(child, "list");
        hittable_list box;
        int arraysize = cJSON_GetArraySize(list_raw);
        for (int i = 0; i < arraysize; i++) {
          cJSON *obj = cJSON_GetArrayItem(list_raw, i);
          box.add(parse_object_once(obj));
        }
        hit = std::make_unique<bvh_node>(box);
      } break;
      case Mesh: {
        auto file_raw = cJSON_GetObjectItem(child, "filename");
        auto scale_raw = cJSON_GetObjectItem(child, "scale");
        if (file_raw && scale_raw) {
          std::string file = file_raw->valuestring;
          double scale = scale_raw->valuedouble;
          auto material_raw = cJSON_GetObjectItem(child, "material");
          if (material_raw == nullptr) {
            std::cerr << find_it->first << " lack 'material' :  \n";
            exit(-1);
          }
          auto find_mat = mat_map.find(material_raw->valuestring);

          if (find_mat != mat_map.end()) {
            hit = std::make_unique<bvh_node>(
                std::make_unique<MeshTriangle>(file, scale, find_mat->second)->triangles);
          } else {
            std::cerr << find_it->first << " can not find mat\n";
          }
        }

      } break;
      case Box: {
        auto p_min = cJSON_GetObjectItem(child, "p_min");
        auto p_max = cJSON_GetObjectItem(child, "p_max");
        auto result0 = parse_vec3d(p_min);
        auto result1 = parse_vec3d(p_max);
        if (result0.first == "" && result1.first == "") {
          auto material_raw = cJSON_GetObjectItem(child, "material");
          if (material_raw == nullptr) {
            std::cerr << find_it->first << " lack 'material' :  \n";
            exit(-1);
          }
          auto find_mat = mat_map.find(material_raw->valuestring);
          if (find_mat != mat_map.end()) {
            hit = std::make_unique<box>(result0.second, result1.second, find_mat->second);
          } else {
            std::cerr << find_it->first << " can not find mat\n";
          }
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      case Trans: {
        auto offset_raw = cJSON_GetObjectItem(child, "offset");
        auto result = parse_vec3d(offset_raw);
        auto obj = cJSON_GetObjectItem(child, "obj");
        if (result.first == "" && obj) {
          hit = std::make_unique<translate>(parse_object_once(obj), result.second);
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      case Scale: {
        auto offset_raw = cJSON_GetObjectItem(child, "offset");
        auto result = parse_vec3d(offset_raw);
        auto obj = cJSON_GetObjectItem(child, "obj");
        if (result.first == "" && obj) {
          hit = std::make_unique<scale>(parse_object_once(obj), result.second);
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      case R_x: {
        auto angle_raw = cJSON_GetObjectItem(child, "angle");
        auto obj = cJSON_GetObjectItem(child, "obj");
        if (angle_raw && obj) {

          hit = std::make_unique<rotate_x>(parse_object_once(obj), angle_raw->valuedouble);
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      case R_y: {
        auto angle_raw = cJSON_GetObjectItem(child, "angle");
        auto obj = cJSON_GetObjectItem(child, "obj");
        if (angle_raw && obj) {

          hit = std::make_unique<rotate_y>(parse_object_once(obj), angle_raw->valuedouble);
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      case R_z: {
        auto angle_raw = cJSON_GetObjectItem(child, "angle");
        auto obj = cJSON_GetObjectItem(child, "obj");
        if (angle_raw && obj) {

          hit = std::make_unique<rotate_z>(parse_object_once(obj), angle_raw->valuedouble);
        } else {
          std::cerr << find_it->first << " lack of information\n";
        }
      } break;
      }
    }
  }
  return hit;
}

auto scene::parse_object(cJSON *sub_root) -> void {
  parse_texture(sub_root);
  parse_material(sub_root);
#ifdef DEBUG
  std::cout << "tex ---------------------\n";
  for (const auto &i : tex_map) {
    std::cout << i.first << " " << i.second << "\n";
  }
  std::cout << "mat ---------------------\n";
  for (const auto &i : mat_map) {
    std::cout << i.first << " " << i.second << "\n";
  }
#endif

  auto item = cJSON_GetObjectItem(sub_root, "objects");
  cJSON *child = nullptr;
  cJSON_ArrayForEach(child, item) {
    bool is_light = false;
    auto type = cJSON_GetObjectItem(child, "type");
    if (type != nullptr) {
      std::string type_str = type->valuestring;
      auto find_it = obj_map.find(type_str);
      if (find_it != obj_map.end()) {
        std::string obj_name = child->string;
        auto is_light_raw = cJSON_GetObjectItem(child, "is_light");
        if (cJSON_IsBool(is_light_raw) && cJSON_IsTrue(is_light_raw)) {
          is_light = true;
        }
        switch (find_it->second) {
        case Sph: {
          auto center_raw = cJSON_GetObjectItem(child, "center");
          auto radius_raw = cJSON_GetObjectItem(child, "radius");
          auto material_raw = cJSON_GetObjectItem(child, "material");
          if (material_raw == nullptr) {
            std::cerr << find_it->first << " lack 'material' :  \n";
            exit(-1);
          }
          auto result = parse_vec3d(center_raw);
          if (result.first != "") {
            std::cerr << find_it->first << " has error members! " << result.first << "\n";
          } else if (radius_raw && material_raw) {
            auto find_mat = mat_map.find(material_raw->valuestring);
            if (is_light) {
              light->add(
                  std::make_unique<sphere>(result.second, radius_raw->valuedouble, nullptr));
            }
            if (find_mat != mat_map.end()) {
              world->add(std::make_unique<sphere>(
                  result.second, radius_raw->valuedouble, find_mat->second));
            } else if (!is_light) {
              std::cerr << find_it->first << " can not find mat\n";
            }
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        case Tri: {
          auto Q_raw = cJSON_GetObjectItem(child, "Q");
          auto u_raw = cJSON_GetObjectItem(child, "u");
          auto v_raw = cJSON_GetObjectItem(child, "v");
          auto result0 = parse_vec3d(Q_raw);
          auto result1 = parse_vec3d(u_raw);
          auto result2 = parse_vec3d(v_raw);
          if (result0.first == "" && result1.first == "" && result2.first == "") {
            auto material_raw = cJSON_GetObjectItem(child, "material");
            if (material_raw == nullptr) {
              std::cerr << find_it->first << " lack 'material' :  \n";
              exit(-1);
            }
            auto find_mat = mat_map.find(material_raw->valuestring);
            if (is_light) {
              light->add(std::make_unique<triangle>(
                  result0.second, result1.second, result2.second, nullptr));
            }
            if (find_mat != mat_map.end()) {
              world->add(std::make_unique<triangle>(
                  result0.second, result1.second, result2.second, find_mat->second));
            } else if (!is_light) {
              std::cerr << find_it->first << " can not find mat\n";
            }
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        case Quad: {
          auto v0_raw = cJSON_GetObjectItem(child, "Q");
          auto v1_raw = cJSON_GetObjectItem(child, "u");
          auto v2_raw = cJSON_GetObjectItem(child, "v");
          auto result0 = parse_vec3d(v0_raw);
          auto result1 = parse_vec3d(v1_raw);
          auto result2 = parse_vec3d(v2_raw);
          if (result0.first == "" && result1.first == "" && result2.first == "") {
            auto material_raw = cJSON_GetObjectItem(child, "material");
            if (material_raw == nullptr) {
              std::cerr << find_it->first << " lack 'material' :  \n";
              exit(-1);
            }
            auto find_mat = mat_map.find(material_raw->valuestring);
            if (is_light) {
              light->add(std::make_unique<quad>(
                  result0.second, result1.second, result2.second, nullptr));
            }
            if (find_mat != mat_map.end()) {
              world->add(std::make_unique<quad>(
                  result0.second, result1.second, result2.second, find_mat->second));
            } else if (!is_light) {
              std::cerr << find_it->first << " can not find mat\n";
            }
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }

        } break;
        case List: {
          auto list_raw = cJSON_GetObjectItem(child, "list");
          int arraysize = cJSON_GetArraySize(list_raw);
          for (int i = 0; i < arraysize; i++) {
            cJSON *obj = cJSON_GetArrayItem(list_raw, i);
            world->add(parse_object_once(obj));
          }
        } break;
        case BVH: {
          auto list_raw = cJSON_GetObjectItem(child, "list");
          hittable_list box;
          int arraysize = cJSON_GetArraySize(list_raw);
          for (int i = 0; i < arraysize; i++) {
            cJSON *obj = cJSON_GetArrayItem(list_raw, i);
            box.add(parse_object_once(obj));
          }
          world->add(std::make_unique<bvh_node>(box));
        } break;
        case Mesh: {
          auto file_raw = cJSON_GetObjectItem(child, "filename");
          auto scale_raw = cJSON_GetObjectItem(child, "scale");
          if (file_raw && scale_raw) {
            std::string file = file_raw->valuestring;
            double scale = scale_raw->valuedouble;
            auto material_raw = cJSON_GetObjectItem(child, "material");
            if (material_raw == nullptr) {
              std::cerr << find_it->first << " lack 'material' :  \n";
              exit(-1);
            }
            auto find_mat = mat_map.find(material_raw->valuestring);
            if (is_light) {
              light->add(std::make_unique<bvh_node>(
                  std::make_unique<MeshTriangle>(file, scale, nullptr)->triangles));
            }
            if (find_mat != mat_map.end()) {
              world->add(std::make_unique<bvh_node>(
                  std::make_unique<MeshTriangle>(file, scale, find_mat->second)->triangles));
            } else if (!is_light) {
              std::cerr << find_it->first << " can not find mat\n";
            }
          }

        } break;
        case Box: {
          auto p_min = cJSON_GetObjectItem(child, "p_min");
          auto p_max = cJSON_GetObjectItem(child, "p_max");
          auto result0 = parse_vec3d(p_min);
          auto result1 = parse_vec3d(p_max);
          if (result0.first == "" && result1.first == "") {
            auto material_raw = cJSON_GetObjectItem(child, "material");
            if (material_raw == nullptr) {
              std::cerr << find_it->first << " lack 'material' :  \n";
              exit(-1);
            }
            auto find_mat = mat_map.find(material_raw->valuestring);
            if (is_light) {
              light->add(std::make_unique<box>(result0.second, result1.second, nullptr));
            }
            if (find_mat != mat_map.end()) {
              world->add(
                  std::make_unique<box>(result0.second, result1.second, find_mat->second));
            } else if (!is_light) {
              std::cerr << find_it->first << " can not find mat\n";
            }
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        case Trans: {
          auto offset_raw = cJSON_GetObjectItem(child, "offset");
          auto result = parse_vec3d(offset_raw);
          auto obj = cJSON_GetObjectItem(child, "obj");
          if (result.first == "" && obj) {
            world->add(std::make_unique<translate>(parse_object_once(obj), result.second));
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        case Scale: {
          auto offset_raw = cJSON_GetObjectItem(child, "offset");
          auto result = parse_vec3d(offset_raw);
          auto obj = cJSON_GetObjectItem(child, "obj");
          if (result.first == "" && obj) {
            world->add(std::make_unique<scale>(parse_object_once(obj), result.second));
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        case R_x: {
          auto angle_raw = cJSON_GetObjectItem(child, "angle");
          auto obj = cJSON_GetObjectItem(child, "obj");
          if (angle_raw && obj) {
            world->add(
                std::make_unique<rotate_x>(parse_object_once(obj), angle_raw->valuedouble));
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        case R_y: {
          auto angle_raw = cJSON_GetObjectItem(child, "angle");
          auto obj = cJSON_GetObjectItem(child, "obj");
          if (angle_raw && obj) {
            world->add(
                std::make_unique<rotate_y>(parse_object_once(obj), angle_raw->valuedouble));
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        case R_z: {
          auto angle_raw = cJSON_GetObjectItem(child, "angle");
          auto obj = cJSON_GetObjectItem(child, "obj");
          if (angle_raw && obj) {
            world->add(
                std::make_unique<rotate_z>(parse_object_once(obj), angle_raw->valuedouble));
          } else {
            std::cerr << find_it->first << " lack of information\n";
          }
        } break;
        }
      }
    }
  }
}

auto scene::parse_background(cJSON *sub_root) -> void {
  auto background_raw = cJSON_GetObjectItem(sub_root, "background");
  if (background_raw) {
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
auto scene::parse_max_depth(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "max_depth");
  if (item != nullptr) {
    max_depth = item->valueint;
  }
}
auto scene::parse_pps(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "pps");
  if (item != nullptr) {
    pps = item->valueint;
  }
}
auto scene::parse_threads(cJSON *sub_root) -> void {
  auto item = cJSON_GetObjectItem(sub_root, "threads");
  if (item != nullptr) {
    threads = item->valueint;
  }
}
auto scene::scene_parse_sub(const std::string &json_path) -> bool {
  // 打开 JSON 文件
  std::ifstream file(json_path);
  if (!file) {
    std::cout << "Error opening JSON file." << std::endl;
    return false;
  }

  // 读取文件内容到字符串
  std::string jsonString(
      (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  // 关闭文件
  file.close();

  // 解析 JSON 数据
  auto sub = cJSON_Parse(jsonString.c_str());
  if (sub == nullptr) {
    std::cerr << " json file can not parse!\n";
  }
  parse_inherit(sub);
  parse_scene_id(sub);
  parse_image_name(sub);
  parse_vup(sub);
  parse_vfov(sub);
  parse_aperture(sub);
  parse_background(sub);
  parse_max_depth(sub);
  parse_pps(sub);
  parse_threads(sub);

  if (scene_id == -1) {
    parse_image_size(sub);
    parse_lookfrom(sub);
    parse_lookat(sub);
    parse_object(sub);
  } else {
    choose_scene(scene_id, *world, *light, aspect_ratio, image_width, vfov, lookfrom, lookat,
        background);
  }
  // 释放内存
  cJSON_Delete(sub);
  return true;
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
  if (root == nullptr) {
    std::cerr << " json file can not parse!\n";
  }
  json_set.insert(json_path);
  parse_inherit(root);
  parse_scene_id(root);
  parse_image_name(root);
  parse_vup(root);
  parse_vfov(root);
  parse_aperture(root);
  parse_background(root);
  parse_max_depth(root);
  parse_pps(root);
  parse_threads(root);

  if (scene_id == -1) {
    parse_image_size(root);
    parse_lookfrom(root);
    parse_lookat(root);
    parse_object(root);
  } else {
    choose_scene(scene_id, *world, *light, aspect_ratio, image_width, vfov, lookfrom, lookat,
        background);
  }
#ifdef LOG
  std::cout << "[img name]: " << image_name << "\n";
#endif
#ifdef DEBUG
  std::cout << *world << "\n";
  std::cout << *light << "\n";
#endif

  camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture);
  renderer = std::make_unique<Renderer<camera>>(*world, *light, aspect_ratio, image_width);
  renderer->set_camera(cam);
  renderer->set_photo_name(image_name);
  renderer->set_samples_per_pixel(pps);
  renderer->set_max_depth(max_depth);
  renderer->set_background(background);
  renderer->set_async_num(threads);

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
auto scene::generate_single_thread() -> void {
#ifdef LOG
  std::cout << *renderer;
#endif
  renderer->render_single();
}

#endif