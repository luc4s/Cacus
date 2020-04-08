#include <cacus.h>

#include <iostream>

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

/**
 * A basic example showing how to create and init a window.
 */
int main() {
  // Read shader files
  auto vertShaderCode = readFile("./vert.spv");
  auto fragShaderCode = readFile("./frag.spv");

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

  cacus.createGraphicsPipeline(vertShaderCode, fragShaderCode);
  cacus.createFrameBuffers();
 
  cout << "Ready" << endl;

  while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}