#pragma once

#include "../macros.h"

NAMESPACE_CACUS

class Material {
 public:
  Material(float r, float g, float b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
  }

 private:
  float color[3];
};

NAMESPACE_CACUS_END
