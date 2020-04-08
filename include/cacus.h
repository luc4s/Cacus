#include <vulkan/vulkan.h>
#include <optional>
#include <vector>
#include <array>

#include <glm/glm.hpp>

typedef struct QueueFamilyIndicesStruct {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
} QueueFamilyIndices;

typedef struct SwapChainSupportDetailsStruct {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
} SwapChainSupportDetails ;


typedef struct VertexStruct {
  glm::vec2 pos;
  glm::vec3 color;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription = {};

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(VertexStruct);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(VertexStruct, pos);
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VertexStruct, color);
    return attributeDescriptions;
  }
} Vertex;

typedef struct UniformBufferObjectStruct {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
} UniformBufferObject;

class Cacus {
public:

  /**
   * Constructor
   */
  Cacus(uint32_t width, uint32_t height);

  /**
   * @param extensionNames Array of required extensions
   * @param extensionCount Number of extensions
   * @throw Error if extensions are not supported
   */
  Cacus(uint32_t width, uint32_t height, const char **extensionNames, size_t extensionCount);

  /**
   * @return Vulkan instance pointer
   */
  VkInstance getInstance() {
    return instance;
  }

  /**
   * @param Set the surface to use.
   */
  void setSurface(VkSurfaceKHR newSurface) {
    surface = newSurface;
  }

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
  void setup( VkSurfaceKHR newSurface,
              std::vector<char> vertex,
              std::vector<char> fragment) {
    surface = newSurface;
    vertexShader = vertex;
    fragmentShader = fragment;
    init();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    setupDrawing();
    createSyncObjects();
  }

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
    const std::vector<uint16_t> &newIndices);

private:
  /**
   * Temporary functions that will be removed in the near future.
   */
  void init();

  void createGraphicsPipeline();

  void setupDrawing();

  void createCommandPool();

  void createSyncObjects();

  void createCommandBuffers();

  void createDescriptorSetLayout();

  void updateUniformBuffer(uint32_t currentImage);

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
  bool checkDeviceExtensionSupport(const VkPhysicalDevice &device) const;

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


  bool initialized;

  uint32_t width;
  uint32_t height;

  size_t currentFrame;

  std::vector<Vertex> vertices;
  std::vector<uint16_t> indices;
  UniformBufferObject ubo;

  std::vector<char> vertexShader;
  std::vector<char> fragmentShader;

  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  VkSurfaceKHR surface;
  VkSwapchainKHR swapChain;

  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;

  VkDescriptorSetLayout descriptorSetLayout;
  VkPipelineLayout pipelineLayout;
  VkRenderPass renderPass;
  VkPipeline graphicsPipeline;

  std::vector<VkFramebuffer> swapChainFramebuffers;

  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;

  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;

  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;
};