#include <cacus.h>

#include <set>
#include <cstring>
#include <stdexcept>
#include <algorithm>

static const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

Cacus::Cacus(uint32_t width, uint32_t height) : Cacus(width, height, {}, 0) {}

Cacus::Cacus(uint32_t width, uint32_t height, const char **extensionNames, size_t extensionCount) :
  surface(VK_NULL_HANDLE),
  physicalDevice(VK_NULL_HANDLE),
  initialized(false),
  width(width),
  height(height)
{
  // Check if required extensions are available
  if (extensionCount > 0) {
    uint32_t vkCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vkCount, nullptr);
    std::vector<VkExtensionProperties> vkExtensions(vkCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &vkCount, vkExtensions.data());

    for (size_t i = 0; i < extensionCount; ++i) {
      bool found = false;
      for (const VkExtensionProperties &properties : vkExtensions) {
        if (strcmp(extensionNames[i], properties.extensionName) == 0) {
          found = true;
          break;
        }
      }

      if (!found) {
        throw std::runtime_error("Extension not supported.");
      }
    }
  }

  // Check validation layers
  if (enableValidationLayers && !checkValidationLayersSupport())
    throw std::runtime_error("Validation layer not available.");


  // Create vulkan instance
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Cacus Library";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Cacus Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;


  createInfo.enabledExtensionCount = extensionCount;
  if (extensionCount > 0)
    createInfo.ppEnabledExtensionNames = extensionNames;

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else
    createInfo.enabledLayerCount = 0;

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
  if (result != VK_SUCCESS)
    throw std::runtime_error("Could not create instance");
}

Cacus::~Cacus() {
  if (initialized) {
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
  }
  vkDestroyInstance(instance, nullptr);
}

void Cacus::init() {
  if (surface == VK_NULL_HANDLE)
    throw std::runtime_error("Surface has not been set");

  initialized = true;

  // Get physical device
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0)
    throw std::runtime_error("Failed to find GPUs with Vulkan support!");
  
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
  for (const auto& device : devices) {
      if (isDeviceSuitable(device)) {
          physicalDevice = device;
          break;
      }
  }

  if (physicalDevice == VK_NULL_HANDLE)
      throw std::runtime_error("Failed to find a suitable GPU!");

  // Create logical device
  QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  // Create queues
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {
    indices.graphicsFamily.value(),
    indices.presentFamily.value()
  };

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDeviceCreateInfo deviceCreateInfo = {};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

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

  // Create swap chain
  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  // Request one more image than minimum count (to no wait on driver)
  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  // Clamp to maximum allowed
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
    imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
  swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapChainCreateInfo.surface = surface;
  swapChainCreateInfo.minImageCount = imageCount;
  swapChainCreateInfo.imageFormat = surfaceFormat.format;
  swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
  swapChainCreateInfo.imageExtent = extent;
  swapChainCreateInfo.imageArrayLayers = 1;
  swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  if (indices.graphicsFamily != indices.presentFamily) {
      swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      swapChainCreateInfo.queueFamilyIndexCount = 2;
      swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
      swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      swapChainCreateInfo.queueFamilyIndexCount = 0; // Optional
      swapChainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
  }

  swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapChainCreateInfo.presentMode = presentMode;
  swapChainCreateInfo.clipped = VK_TRUE;
  swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
      throw std::runtime_error("Failed to create swap chain!");

  // Get swap chain images
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;
}

VkSurfaceFormatKHR Cacus::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
      availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  // Return first if preferred not available
  return availableFormats[0];
}

VkPresentModeKHR Cacus::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      return availablePresentMode;
  }

  // FIFO is guaranteed to be available
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Cacus::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const {
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

bool Cacus::isDeviceSuitable(const VkPhysicalDevice &device) const {
  // Device is suitable if it allows a queue family
  QueueFamilyIndices indices = findQueueFamilies(device);
  bool swapChainAdequate = false;
  const bool extensionsSupported = checkDeviceExtensionSupport(device);
  if (extensionsSupported) {
      const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }
  return indices.isComplete() &&
          swapChainAdequate &&
          extensionsSupported;
}

SwapChainSupportDetails Cacus::querySwapChainSupport(const VkPhysicalDevice &device) const {
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount) {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

  if (presentModeCount) {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

QueueFamilyIndices Cacus::findQueueFamilies(const VkPhysicalDevice &device) const {
  QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  // Get queues with graphics bit
  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        indices.graphicsFamily = i;

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport)
      indices.presentFamily = i;

    ++i;
  }

  return indices;
}

bool Cacus::checkDeviceExtensionSupport(const VkPhysicalDevice &device) const {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for (const auto& extension : availableExtensions)
    requiredExtensions.erase(extension.extensionName);

  return requiredExtensions.empty();
}

bool Cacus::checkValidationLayersSupport() const {
  // Get layers
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  // Compare against requested
  for (const char* layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound)
      return false;
  }

  return true;
}