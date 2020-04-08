#include <cacus.h>

#include <iostream>

#include <glm/glm.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <vector>

#define WIDTH 800
#define HEIGHT 600

using namespace std;

static std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open())
    throw std::runtime_error("Failed to open file!");

  size_t fileSize = (size_t) file.tellg();
  std::vector<char> buffer(fileSize);
  
  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto cacus = reinterpret_cast<Cacus*>(glfwGetWindowUserPointer(window));
  cacus->recreateSwapChain(width, height);
}

/**
 * A basic example showing how to create and init a window.
 */
int main() {
  // Read shader files
  auto vertShaderCode = readFile("./vert.spv");
  auto fragShaderCode = readFile("./frag.spv");

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

  // Get GLFW extensions
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  Cacus cacus(WIDTH, HEIGHT, glfwExtensions, glfwExtensionCount);

  glfwSetWindowUserPointer(window, &cacus);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

  VkSurfaceKHR surface;
  glfwCreateWindowSurface(cacus.getInstance(), window, nullptr, &surface);

  const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
  };

  const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
  };

  cacus.setup(surface, vertShaderCode, fragShaderCode);
  cacus.createMeshBuffers(vertices, indices);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Recreate swap chain in case of window resize.
    if (cacus.draw()) {
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      cacus.recreateSwapChain(width, height);
    }
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}