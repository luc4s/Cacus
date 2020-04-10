namespace utils {
  typedef struct SwapChainSupportDetailsStruct {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
  } SwapChainSupportDetails;

  SwapChainSupportDetails querySwapChainSupport(
    const VkPhysicalDevice &physicalDevice,
    const VkSurfaceKHR &surface);

  VkImageView createImageView(
    VkDevice &device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspectFlags);

  void createImage(
    VkDevice &device,
    uint32_t width,
    uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkImage& image,
    VkDeviceMemory& imageMemory);
}
