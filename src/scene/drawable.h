#pragma once

#include "node.h"

namespace scene {

DECL_SHARED(DrawableMesh)

class DrawableMesh : public Node {
 public:
  DrawableMesh(Mesh::Ptr mesh, Material::Ptr material) {}

 private:
  Mesh::Ptr mesh;
  Material::Ptr material;
};

}  // namespace scene
