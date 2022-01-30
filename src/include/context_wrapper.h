#pragma once

#include "macros.h"
#include "shared_ptr.h"

NAMESPACE_CACUS

DECL_SHARED(Context)

class ContextWrapper {
 public:
  ContextWrapper() {}

  ContextWrapper(ContextPtr ctx) : context(ctx) {}

  template <class T, class... Args>
  std::shared_ptr<T> create(Args&&... args) {
    return std::make_shared<T>(Ctx(), args...);
  }

 protected:
  virtual ContextPtr Ctx() { return context; }

 private:
  ContextPtr context;
};

NAMESPACE_CACUS_END