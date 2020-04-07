#include <cacus.h>

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

using namespace std;

/**
 * A basic example showing how to create and init a window.
 */
int main() {
  glfwInit();
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

  // Get GLFW extensions
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  Cacus cacus(WIDTH, HEIGHT, glfwExtensions, glfwExtensionCount);

  VkSurfaceKHR surface;
  glfwCreateWindowSurface(cacus.getInstance(), window, nullptr, &surface);
  cacus.setSurface(surface);
  cacus.init();
 
  cout << "Ready" << endl;

  while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}