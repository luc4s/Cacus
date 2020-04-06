#include <cacus.h>

#include <vector>
#include <string>
#include <stdexcept>


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

/**
 * @param extensionNames Array of required extensions
 * @param extensionCount Number of extensions
 * @throw Error if extensions are not supported
 */
Cacus::Cacus(const char **extensionNames, size_t extensionCount) {
	// Check if required extensions are available
	uint32_t vkCount = 0;
	std::vector<VkExtensionProperties> vkExtensions;
	vkEnumerateInstanceExtensionProperties(nullptr, &vkCount, vkExtensions.data());

	for (const VkExtensionProperties &properties : vkExtensions) {
		for (size_t i = 0; i < extensionCount; ++i) {
			const std::string extension = extensionNames[i];
			if (extension.compare(properties.extensionName)) {
				std::string error("Extension not supported: ");
				error += extensionNames[i];
				throw std::runtime_error(error);
			}
		}
	}

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

/**
 * Cacus destructor
 */
Cacus::~Cacus() {
	vkDestroyInstance(instance, nullptr);
}

bool Cacus::checkValidationLayersSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
	    bool layerFound = false;

	    for (const auto& layerProperties : availableLayers) {
	        if (strcmp(layerName, layerProperties.layerName) == 0) {
	            layerFound = true;
	            break;
	        }
	    }

	    if (!layerFound) {
	        return false;
	    }
	}
    return false;
}