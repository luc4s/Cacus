#pragma once

#include "context_wrapper.h"
#include "shared_ptr.h"

class GLFWwindow;

NAMESPACE_CACUS

DECL_SHARED(SurfaceKHR)
DECL_SHARED(Surface)

class Surface : public ContextWrapper {
 public:
  Surface(ContextPtr ctx, GLFWwindow *window);

  void resize(size_t width, size_t height);

 private:
  SurfaceKHRPtr surface;
};

NAMESPACE_CACUS_END
