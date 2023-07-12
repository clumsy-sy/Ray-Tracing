#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include "../external/rtw_stb_image.hpp"
#include "texture.hpp"

class image_texture : public texture {
private:
  unsigned char *data;
  int width, height;
  int bytes_per_scanline;

public:
  const static int bytes_per_pixel = 3;

  image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

  image_texture(const char *filename) {
    auto components_per_pixel = bytes_per_pixel;
    // 通过库来读取贴图的信息
    data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
    // std::cout << "width : " << width << " height: " << height << std::endl;
    if (!data) {
      std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
      width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
  }

  ~image_texture() {
    delete data;
  }

  [[nodiscard]] auto value(double u, double v, const Vec3d &) const -> color override {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (data == nullptr)
      return {0, 1, 1};

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width)
      i = width - 1;
    if (j >= height)
      j = height - 1;

    const auto color_scale = 1.0 / 255.0;
    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
  }
};

#endif