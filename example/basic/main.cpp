#include <cacus.h>

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

  // Load texture
  int texWidth, texHeight, texChannels;
  stbi_uc* pixels = stbi_load("texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  if (pixels) {
    cacus.loadTexture(texWidth, texHeight, texChannels, pixels);
    stbi_image_free(pixels);
  } else
    std::cerr << "Could not load texture :(" << std::endl;

  const auto startTime = std::chrono::high_resolution_clock::now();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    // Compute transforms
    int width, height;
    cacus.getDimensions(width, height);

    glm::mat4 proj = glm::perspective(
      glm::radians(45.0f), width / (float) height, 0.1f, 10.0f);
    proj[1][1] *= -1;

    const glm::mat4 model = glm::rotate(
      glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    const glm::mat4 view = glm::lookAt(
      glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    cacus.setTransform(model, view, proj);
  
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