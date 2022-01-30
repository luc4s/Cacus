#pragma once

#include <vector>

namespace data {

DECL_SHARED(StaticMesh)

class StaticMesh : public Mesh {
 public:
  class Builder {
   public:
    void addVertex(float x, float y, float z) {}

    StaticMeshPtr Create() {}

   private:
  };

 protected:
  explicit StaticMesh(std::vector<float>&& vertices) : vertices(vertices) {}

 private:
  std::vector<float> vertices;
};

}  // namespace data
