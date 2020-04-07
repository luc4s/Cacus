#include "gtest/gtest.h"

#include <cacus.h>
#include <stdexcept>

#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

TEST(SimpleTests, EmptyCacus) {
  ASSERT_NO_THROW({
    Cacus cacus(WIDTH, HEIGHT);
  });
}

TEST(SimpleTests, InitInvalidCacus) {
  const char *extensions[1] = {
    ""
  };
  ASSERT_THROW({
    Cacus cacus(WIDTH, HEIGHT, extensions, 1);
  }, std::runtime_error);
}

TEST(SimpleTests, InitCacusGLFW) {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  ASSERT_NO_THROW({
    Cacus cacus(WIDTH, HEIGHT, glfwExtensions, glfwExtensionCount);
  });
}

TEST(SimpleTests, InitCacusWithoutSurface) {
  ASSERT_THROW({
    Cacus cacus(WIDTH, HEIGHT);
    cacus.init();
  }, std::runtime_error);
}