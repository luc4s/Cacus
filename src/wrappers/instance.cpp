#include "instance.h"

#include "macros.h"

NAMESPACE_CACUS

InstanceBuilder::InstanceBuilder() {
  appInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
  appInfo.setPEngineName("Cacus");
  appInfo.setEngineVersion(VK_MAKE_VERSION(1, 0, 0));
  appInfo.setApiVersion(VK_API_VERSION_1_0);

  createInfo.setPApplicationInfo(&appInfo);
}

void InstanceBuilder::setAppName(const std::string& name) {
  appInfo.setPApplicationName(name.c_str());
}

std::unique_ptr<Instance> InstanceBuilder::build() const {
  auto vkInstance = vk::createInstance(createInfo);
  return std::make_unique<Instance>(std::move(vkInstance));
}

Instance::Instance(vk::Instance&& instance) : instance(std::move(instance)) {}

Instance::~Instance() { instance.destroy(); }

NAMESPACE_CACUS_END
