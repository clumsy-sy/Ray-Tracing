#ifndef MESHTRIANGLE_hpp
#define MESHTRIANGLE_hpp

#include "../global.hpp"
#include "hittable.hpp"
#include "hittablelist.hpp"
#include "interval.hpp"
#include "triangle.hpp"

#include "../external/OBJ_Loader.hpp"
#include "../material/lambertian.hpp"
// #include "../material/dielectric.hpp"
// #include "../material/metal.hpp"
#include <memory>
#include <utility>

class MeshTriangle : public hittable {
public:
  hittable_list triangles;
  double scale = 1.0f; // 缩放系数
  material *mat_ptr;
  aabb bbox;

public:
  MeshTriangle() = default;
  MeshTriangle(
      const std::string &filename, double s = 1.0f, material *m = new lambertian(color(0.51765, 0.43922, 1) * 0.75))
      : scale(s), mat_ptr(m) {
    objl::Loader loader;
    loader.LoadFile(filename);
    assert(loader.LoadedMeshes.size() == 1);
    auto mesh = loader.LoadedMeshes[0];
    std::cout << mesh.Vertices.size() / 3 << std::endl;
    for (uint32_t i = 0; i < mesh.Vertices.size(); i += 3) {
      std::array<vec3d, 3> face_vertices;
      std::array<pdd, 3> texture;
      for (int j = 0; j < 3; j++) {
        auto vert =
            vec3d(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z) *
            scale;
        auto tex = std::make_pair(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y);
        face_vertices[j] = vert;
        texture[j] = tex;
      }
      triangles.add(std::make_unique<triangle>(face_vertices, texture, mat_ptr));
    }
    bbox = triangles.bounding_box();
  }
  auto hit(const ray &r, interval ray_t, hit_record &rec) const -> bool override;
  [[nodiscard]] auto bounding_box() const -> aabb override;
  friend auto operator<<(std::ostream &os, const MeshTriangle &m) -> std::ostream & {
    for (auto const &t : m.triangles.objects) {
      os << t << "\n";
    }
    return os;
  }
};
auto MeshTriangle::hit(const ray &r, interval ray_t, hit_record &rec) const -> bool {
  return triangles.hit(r, ray_t, rec);
}

auto MeshTriangle::bounding_box() const -> aabb {
  return bbox;
}

#endif
