add_executable(basic.deprecated main.cpp)

find_package(glfw3 REQUIRED)

target_link_libraries(basic.deprecated cacus glfw)

# Custom target to compile shaders (assumed glslc is available)
add_custom_target(shaders
  COMMAND glslc shader.vert -o vert.spv
  COMMAND glslc shader.frag -o frag.spv
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
  TARGET shaders
  POST_BUILD
  COMMAND mv *.spv ${CMAKE_CURRENT_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})