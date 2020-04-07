#include <cacus.h>

#include <vector>
#include <cstring>
#include <stdexcept>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

Cacus::Cacus() : Cacus(NULL, 0) {}

Cacus::Cacus(const char **extensionNames, size_t extensionCount) {
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
  createInfo.ppEnabledExtensionNames = extensionNames;

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
}

Cacus::~Cacus() {
  vkDestroyInstance(instance, nullptr);
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