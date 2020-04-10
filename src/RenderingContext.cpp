#include "RenderingContext.h"

/**
 * Helper function to create VkDeviceQueueCreateInfo
 */
static VkDeviceQueueCreateInfo getQueueCreateInfos(uint32_t queueFamily, float priority) {
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = queueFamily;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &priority;
}

RenderingContext::RenderingContext(
    VkInstance &instance,
    VkPhysicalDevice &physicalDevice,
    VkSurfaceKHR &surface,
    int width, int height,
    std::vector<const char *> deviceExtensions,
    uint32_t graphicsFamilyIndex,
    uint32_t presentFamilyIndex) :
    instance(instance),
    physicalDevice(physicalDevice),
    surface(surface),
    width(width),
    height(height)
{
  // Create queues
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  queueCreateInfos.push_back(getQueueCreateInfos(graphicsFamily, 1.0f));
  if (graphicsFamilyIndex != presentQueueIndex)
    queueCreateInfos.push_back(getQueueCreateInfos(presentFamilyIndex, 1.0f));

  VkPhysicalDeviceFeatures deviceFeatures = {};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo deviceCreateInfo = {};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
  deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

  deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
  deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

  if (enableValidationLayers) {
    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
  } else
    deviceCreateInfo.enabledLayerCount = 0;

  if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
    throw std::runtime_error("Failed to create logical device!");

  // Retrieve queue handles
  vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
  vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

  // Retrieve depth format
  depthFormat = findSupportedFormat(
    { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
  );

  // Command pool
  VkCommandPoolCreateInfo poolInfo = {};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = graphicsFamilyIndex

  if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    throw std::runtime_error("failed to create command pool!");

  // Create swap chain
  utils::SwapChainSupportDetails swapChainSupport = utils::querySwapChainSupport(physicalDevice, surface);
  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  swapChain = new SwapChain(
    graphicsFamilyIndex,
    presentFamilyIndex,
    swapChainSupport,
    surfaceFormat,
    presentMode,
    extent);
}

RenderingContext::~RenderingContext() {
  delete swapChain;
  delete commandBuffer;

  vkDestroyCommandPool(device, commandPool, nullptr);

  vkDestroyDevice(device, nullptr);
  vkDestroySurfaceKHR(instance, surface, nullptr);
}

VkSurfaceFormatKHR RenderingContext::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
      availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  // Return first if preferred not available
  return availableFormats[0];
}

VkPresentModeKHR RenderingContext::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      return availablePresentMode;
  }

  // FIFO is guaranteed to be available
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RenderingContext::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const {
  if (capabilities.currentExtent.width != UINT32_MAX)
    return capabilities.currentExtent;
  else {
    VkExtent2D actualExtent = { width, height };

    actualExtent.width = std::max(
      capabilities.minImageExtent.width, std::min(
        capabilities.maxImageExtent.width, actualExtent.width));

    actualExtent.height = std::max(
      capabilities.minImageExtent.height, std::min(
        capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
}

VkFormat RenderingContext::findSupportedFormat(
  const std::vector<VkFormat>& candidates,
  VkImageTiling tiling,
  VkFormatFeatureFlags features)
{
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
      return format;
    else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
      return format;
  }
  throw new std::runtime_error("Could not find a supported format");
}