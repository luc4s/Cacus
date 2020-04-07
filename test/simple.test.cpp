#include "gtest/gtest.h"

#include <cacus.h>
#include <stdexcept>

#include <GLFW/glfw3.h>

TEST(SimpleTests, InitEmptyCacus) {
  ASSERT_NO_THROW({
    Cacus cacus;
  });
}

TEST(SimpleTests, InitInvalidCacus) {
  const char *extensions[1] = {
    "\0"
  };
  ASSERT_THROW({
    Cacus cacus(extensions, 1);
  }, std::runtime_error);
}

TEST(SimpleTests, InitCacusGLFW) {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  ASSERT_NO_THROW({
    Cacus cacus(glfwExtensions, glfwExtensionCount);
  });
}