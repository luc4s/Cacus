#pragma once

#include <string>
#include <vector>

#include "context_wrapper.h"
#include "macros.h"
#include "shared_ptr.h"

NAMESPACE_CACUS

DECL_SHARED(Context)
DECL_SHARED(Instance)
DECL_SHARED(InstanceBuilder)
DECL_SHARED(Surface)

class Context : public ContextWrapper,
                public std::enable_shared_from_this<Context> {
 public:
  class Builder {
   public:
    Builder();

    void setUseValidationLayers(bool enabled);

    void addExtensions(const char **ext, size_t count);

    void setAppName(const std::string &name);

    ContextPtr build() const;

   private:
    std::shared_ptr<InstanceBuilder> builder;
    std::vector<std::string> extensions;
    void (*validationCallback)(uint8_t, const std::string &) = nullptr;
  };

  virtual ~Context() {}

  InstancePtr inst() { return instance; }

 protected:
  ContextPtr Ctx() override { return shared_from_this(); }

 private:
  InstancePtr instance;
};

NAMESPACE_CACUS_END
