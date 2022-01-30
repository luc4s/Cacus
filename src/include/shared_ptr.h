#pragma once

#include <memory>

#define DECL_SHARED(x) \
  class x;             \
  using x##Ptr = std::shared_ptr<x>;
