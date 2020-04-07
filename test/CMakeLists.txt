set(BUILD_GMOCK 0)
set(INSTALL_GTEST 0)

add_subdirectory(googletest)

add_executable(
    unit_tests
    simple.test.cpp)

target_link_libraries(
    unit_tests
    cacus
    glfw3
    gtest_main)
