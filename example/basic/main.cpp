#include <cacus.h>

#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
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

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

static vector<char> readFile(const string &filename) {
  ifstream file(filename, ios::ate | ios::binary);

  if (!file.is_open())
    throw runtime_error("Failed to open file!");

  size_t fileSize = (size_t) file.tellg();
  vector<char> buffer(fileSize);
  
  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto cacus = reinterpret_cast<Cacus*>(glfwGetWindowUserPointer(window));
  cacus->recreateSwapChain(width, height);
}

static const string MODEL_PATH = "chalet.obj";
static const string TEXTURE_PATH = "chalet.jpg";

void loadModel(
  vector<Vertex> &vertices,
  vector<uint32_t> &indices) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str()))
    throw std::runtime_error(warn + err);

  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      Vertex vertex = {};

      vertex.pos = {
        attrib.vertices[3 * index.vertex_index + 0],
        attrib.vertices[3 * index.vertex_index + 1],
        attrib.vertices[3 * index.vertex_index + 2]
      };

      vertex.texCoord = {
        attrib.texcoords[2 * index.texcoord_index + 0],
        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
      };

      vertex.color = {1.0f, 1.0f, 1.0f};

      vertices.push_back(vertex);
      indices.push_back(indices.size());
    }
  }
}

/**
 * A basic example showing how to create and init a window.
 */
int main() {

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

  // Read shader files
  auto vertShaderCode = readFile("./vert.spv");
  auto fragShaderCode = readFile("./frag.spv");
  cacus.setup(surface, vertShaderCode, fragShaderCode);

  // Load texture
  int texWidth, texHeight, texChannels;
  stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  if (pixels) {
    cacus.loadTexture(texWidth, texHeight, texChannels, pixels);
    stbi_image_free(pixels);
  } else
    cerr << "Could not load texture :(" << endl;

  //*
  // Load mesh data
  vector<Vertex> vertices;
  vector<uint32_t> indices;
  loadModel(vertices, indices);
  std::cout << "Loaded " << vertices.size() << " vertices and " << indices.size() << " indices" << endl;
  //*/
  /*
  const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
  };

  const std::vector<uint32_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
  };
  //*/
  cacus.createMeshBuffers(vertices, indices);
  cacus.finalize();

  const auto startTime = chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    auto currentTime = chrono::high_resolution_clock::now();
    float time = chrono::duration<float, chrono::seconds::period>(currentTime - startTime).count();

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