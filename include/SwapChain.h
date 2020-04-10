#include <utils.h>

#include <vector>

/** 
 * Wrapper for swap chain management.
 */
class SwapChain {
  SwapChain(
    uint32_t graphicsQueueIndex,
    uint32_t presentQueueIndex,
    utils::SwapChainSupportDetails swapChainSupport,
    VkSurfaceFormatKHR surfaceFormat,
    VkPresentModeKHR presentMode,
    VkExtent2D extent);

  ~SwapChain();

private:
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  VkCommandPool commandPool;
  VkDescriptorPool descriptorPool;

  std::vector<RenderTarget*> renderTargets;

  VkPipelineLayout pipelineLayout;
  VkRenderPass renderPass;
  VkPipeline graphicsPipeline;

  // Depth buffering
  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;
  VkFormat depthFormat;
};
