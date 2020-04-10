#include <vulkan/vulkan.h>
#include <optional>
#include <vector>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

typedef struct QueueFamilyIndicesStruct {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
} QueueFamilyIndices;


typedef struct VertexStruct {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(VertexStruct);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(VertexStruct, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VertexStruct, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(VertexStruct, texCoord);

    return attributeDescriptions;
  }

} Vertex;

typedef struct UniformBufferObjectStruct {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
} UniformBufferObject;

/**
 * Base class of the rendering engine, responsible for creating rendering context,
 * handling validation layers and device selection.
 */
class Cacus {
public:

  /**
   * Creates the instance without any extension enabled.
   */
  Cacus();

  /**
   * Instantiates rendering engine with given extensions.
   *
   * @param extensionNames Array of required extensions
   * @param extensionCount Number of extensions
   * @throw runtime error if extensions are not supported
   */
  Cacus(const char **extensionNames, size_t extensionCount);

  /**
   * @return Vulkan instance pointer
   */
  VkInstance getInstance() {
    return instance;
  }

  /**
   * Creates a suitable rendering context for the given surface.
   */
  RenderingContext *createRenderingContext(
    const VkSurfaceKHR &surface,
    int width, int height) const;


  void getDimensions(int &outWidth, int &outHeight) {
    outWidth = width;
    outHeight = height;
  }

  /**
   * Set transform of current shape.
   */
  void setTransform(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);

  /**
   * Destructor.
   */
  ~Cacus();

  /**
   * Performs setup of Vulkan (to remove)
   */
  void setup( std::vector<char> vertex,
              std::vector<char> fragment) {
    vertexShader = vertex;
    fragmentShader = fragment;
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
  }

  void finalize();

  /**
   * Draws on surface.
   * @return true if swap chain must be recreated.
   */
  bool draw();

  /**
   * Recreate the swap chain with new dimensions.
   */
  void recreateSwapChain(uint32_t newWidth, uint32_t newHeight);

  /**
   * Create vertex an index buffers for drawing shapes
   * @param newVertices vertices
   * @param newIndices indices
   */
  void createMeshBuffers(
    const std::vector<Vertex> &newVertices,
    const std::vector<uint32_t> &newIndices);

  void loadTexture(const int texWidth, const int texHeight, const int texChannels, const unsigned char *pixels);

private:
  /**
   * Temporary functions that will be removed in the near future.
   */

  void preFinalize();

  void createSyncObjects();

  void createCommandBuffers();

  void updateUniformBuffer(uint32_t currentImage);

  void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
  
  VkCommandBuffer beginSingleTimeCommands();

  void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

  VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

  VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  /**
   * Creates a buffer.
   */
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

  /**
   * Copy buffers.
   */
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  /**
   * Returns the type of memory depending on application and buffer requirements
   */
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

  /**
   * Cleanup the swap chain
   */
  void cleanupSwapChain();

  /**
   * @return True if validation layers are available, false otherwise.
   */
  bool checkValidationLayersSupport() const;

  /**
   * @return True if device support required extensions
   */
  bool checkDeviceExtensionSupport(
    const VkPhysicalDevice &device,
    const std::vector<const char*> &deviceExtensions) const;

  /**
   * @param device Device to check for suitability
   * @return True if device is suitable.
   */
  bool isDeviceSuitable(const VkPhysicalDevice &device) const;

  /**
   * @param device Physical device on which to query queue families
   * @return Indices of queues
   */
  QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &device) const;

  /**
   * @param device Physical device
   * @return Surface format and present modes for given device
   */
  SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &device) const;

  /**
   * @param List of available formats
   * @return Chosen format
   */
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const;

  /**
   * @param List of available present modes
   * @return Preferred present mode
   */
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const;

  /**
   * Chooses swap extent (resolution)
   * @param Possible resolutions
   * @return Preferred extent
   */
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;

  /**
   * Create a shader module from byte code.
   * @param Bytecode
   * @return Shader module
   */
  VkShaderModule createShaderModule(const std::vector<char> &code) const;


  uint32_t width;
  uint32_t height;

  size_t currentFrame;

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  UniformBufferObject ubo;

  std::vector<char> vertexShader;
  std::vector<char> fragmentShader;

  VkInstance instance;

  // VkCommandPool commandPool;
  // VkDescriptorPool descriptorPool;
/*
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<VkDescriptorSet> descriptorSets;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;

  // Uniform buffers
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;*/

  // Vertex & index buffers
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;


  // Texture mapping
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView textureImageView;
  VkSampler textureSampler;
};
