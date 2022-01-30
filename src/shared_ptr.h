#pragma once

#include <memory>

#define DECL_SHARED(className) using##classNamePtr = std::shared_ptr<className>;
#define DECL_WEAK_SHARED(className) \
  using##classNameWkPtr = std::weak_ptr<className>;
