#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

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

class Cacus {
public:

  /**
   * Default constructor
   */
  Cacus(uint32_t width, uint32_t height);

  /**
   * @param extensionNames Array of required extensions
   * @param extensionCount Number of extensions
   * @throw Error if extensions are not supported
   */
  Cacus(uint32_t width, uint32_t height, const char **extensionNames, size_t extensionCount);

  /**
   * Initializes Vulkan, surface must have been set.
   */
  void init();

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

  /**
   * Temporary function. Soon to be removed.
   */
  void createGraphicsPipeline(std::vector<char> vertex, std::vector<char> fragment);

  /**
   * Temporary, will be removed in the near future.
   */
  void createFrameBuffers();

  ~Cacus();

private:
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

  VkPipelineLayout pipelineLayout;
  VkRenderPass renderPass;
  VkPipeline graphicsPipeline;

  std::vector<VkFramebuffer> swapChainFramebuffers;
};