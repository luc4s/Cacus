add_executable(basic main.cpp)

find_package(glfw3 REQUIRED)

target_link_libraries(basic cacus glfw)
target_include_directories(basic PUBLIC ${CACUS_HEADERS})

add_custom_target(shaders
  COMMAND glslc shader.vert -o vert.spv
  COMMAND glslc shader.frag -o frag.spv
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})