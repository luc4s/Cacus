
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

#include <context.h>
#include <surface.h>

static void onResize(GLFWwindow* window, int width, int height) {
  auto surface =
      *reinterpret_cast<cacus::SurfacePtr*>(glfwGetWindowUserPointer(window));
  surface->resize(width, height);
}

int main() {
  using namespace cacus;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window =
      glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

  // Get GLFW extensions
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  /// Create context
  auto builder = std::make_shared<Context::Builder>();
  builder->setAppName("Example");
  builder->addExtensions(glfwExtensions, glfwExtensionCount);

  auto ctx = builder->build();

  // Create render surface
  SurfacePtr surface = ctx->create<Surface>(window);

  glfwSetWindowUserPointer(window, &surface);
  glfwSetFramebufferSizeCallback(window, onResize);
}