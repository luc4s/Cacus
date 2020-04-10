/**
 * Provides context for rendering, surface, physical device, device extensions, ...
 */
class RenderingContext {

  RenderingContext(
    VkInstance &instance,
    VkPhysicalDevice &physicalDevice,
    VkSurfaceKHR &surface,
    int width,
    int height,
    std::vector<const char*> deviceExtensions,
    uint32_t graphicsFamilyIndex,
    uint32_t presentFamilyIndex);

  ~RenderingContext();

  /*
  void render(...);
  */

private:
  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  VkDevice device;

  VkSurfaceKHR surface;

  int width;
  int height;

  VkDescriptorSetLayout descriptorSetLayout;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  SwapChain *swapChain;
};
