#define LOG
// #define DEBUG

#include "scene/scene_parse.hpp"

auto main(int argc, const char **argv) -> int {
  scene Scene;

  // 命令行读入参数为生成的图片命名
  std::string json_file = "examples/main.json";
  if (argc >= 2) {
    json_file = std::string(argv[1]);
    std::cout << "Reading : " << json_file << "\n";
  }
  if (!Scene.scene_parse(json_file)) {
    std::cerr << "can not parse json_file " << json_file << "\n";
  }
  Scene.generate();

  return 0;
}
