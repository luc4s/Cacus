#pragma once

#include <vulkan_api.h>

#include "macros.h"
#include "shared_ptr.h"

DECL_VK_CLASS(Instance)

NAMESPACE_CACUS

DECL_SHARED(Instance)

class InstanceBuilder {
 public:
  InstanceBuilder();

  void setAppName(const std::string &name);

  std::unique_ptr<Instance> build() const;

 private:
  vk::InstanceCreateInfo createInfo;
  vk::ApplicationInfo appInfo;
};

class Instance {
 public:
  Instance(vk::Instance &&);
  virtual ~Instance();

  operator vk::Instance() { return instance; }

 private:
  vk::Instance instance;
};

NAMESPACE_CACUS_END
