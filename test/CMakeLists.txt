set(BUILD_GMOCK 0)
set(INSTALL_GTEST 0)

add_subdirectory(googletest)

find_package(glfw3 REQUIRED)

add_executable(
    unit_tests
    simple.test.cpp)

target_link_libraries(
    unit_tests
    cacus
    glfw
    gtest_main)
