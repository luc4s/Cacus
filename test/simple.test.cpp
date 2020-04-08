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