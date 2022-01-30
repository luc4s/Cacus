#pragma once

#include "shared_ptr.h"

class SurfaceKHR {
 public:
 private:
  std::unique_ptr<vk::SurfaceKHR> surfaceKHR;
};