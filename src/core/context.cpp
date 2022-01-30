#include "context.h"

#include "instance.h"
#include "vulkan_api.h"

NAMESPACE_CACUS

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData) {
  // std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

Context::Builder::Builder() { builder = std::make_shared<InstanceBuilder>(); }

void Context::Builder::setUseValidationLayers(bool enabled) {
  // TODO
}

void Context::Builder::addExtensions(const char** ext, size_t count) {}

void Context::Builder::setAppName(const std::string& name) {}

ContextPtr Context::Builder::build() const { return ContextPtr(); }

NAMESPACE_CACUS_END
