#include "surface.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "context.h"
#include "instance.h"
#include "vulkan_api.h"

NAMESPACE_CACUS

Surface::Surface(ContextPtr ctx, GLFWwindow* window) : ContextWrapper(ctx) {
  VkSurfaceKHR vkSurface;
  vk::Instance instance = *(Ctx()->inst());
  glfwCreateWindowSurface(instance, window, nullptr, &vkSurface);
}

void Surface::resize(size_t width, size_t height) {
  // TODO
}

NAMESPACE_CACUS_END
