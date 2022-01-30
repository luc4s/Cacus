add_executable(basic main.cpp)

find_package(glfw3 REQUIRED)

target_link_libraries(basic PRIVATE cacus glfw)