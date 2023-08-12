#ifndef MESHTRIANGLE_hpp
#define MESHTRIANGLE_hpp

#include "../global.hpp"
#include "BVH.hpp"
#include "hittable.hpp"
#include "triangle.hpp"

#include "../external/OBJ_Loader.hpp"
#include "../material/lambertian.hpp"
// #include "../material/dielectric.hpp"
// #include "../material/metal.hpp"
#include <memory>
#include <utility>

class MeshTriangle : public hittable {
public:
  // std::vector<std::shared_ptr<triangle>> triangles;
  hittable_list triangles;
  double scale = 1.0f; // 缩放系数
  std::shared_ptr<material> mat_ptr;
  aabb box;

public:
  MeshTriangle() = default;
  MeshTriangle(const std::string &filename, double s = 1.0f,
      std::shared_ptr<material> m = std::make_shared<lambertian>(color(0.51765, 0.43922, 1) * 0.75))
      : scale(s), mat_ptr(std::move(m)) {
    objl::Loader loader;
    loader.LoadFile(filename);
    assert(loader.LoadedMeshes.size() == 1);
    auto mesh = loader.LoadedMeshes[0];
    std::cout << mesh.Vertices.size() / 3 << std::endl;

    Vec3d min_vert = Vec3d{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity()};
    Vec3d max_vert = Vec3d{-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity()};
    for (uint32_t i = 0; i < mesh.Vertices.size(); i += 3) {
      // auto *t = new triangle();
      std::array<Vec3d, 3> face_vertices;
      std::array<pdd, 3> texture;
      for (int j = 0; j < 3; j++) {
        auto vert =
            Vec3d(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z) *
            scale;
        auto tex = std::make_pair(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y);
        face_vertices[j] = vert;
        texture[j] = tex;

        min_vert =
            Vec3d(std::min(min_vert.x(), vert.x()), std::min(min_vert.y(), vert.y()), std::min(min_vert.z(), vert.z()));
        max_vert =
            Vec3d(std::max(max_vert.x(), vert.x()), std::max(max_vert.y(), vert.y()), std::max(max_vert.z(), vert.z()));
      }

      // auto albedo = color::random(0.5, 1);
      // auto fuzz = random_double(0, 0.5)();
      // auto mat_ptr = std::make_shared<metal>(albedo, fuzz);
      triangles.add(std::make_shared<triangle>(face_vertices, texture, mat_ptr));
    }
    box = aabb(min_vert, max_vert);
  }
  auto hit(const ray &r, double t_min, double t_max, hit_record &rec) const -> bool override;
  auto bounding_box(aabb &output_box) const -> bool override;
  friend auto operator<<(std::ostream &os, const MeshTriangle &m) -> std::ostream & {
    for (auto const &t : m.triangles.objects) {
      os << t << "\n";
    }
    return os;
  }
};
auto MeshTriangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const -> bool {
  auto tree = std::make_shared<bvh_node>(triangles);
  return tree->hit(r, t_min, t_max, rec);
}

auto MeshTriangle::bounding_box(aabb &output_box) const -> bool {
  output_box = box;
  return true;
}

#endif
