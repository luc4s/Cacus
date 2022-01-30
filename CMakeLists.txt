cmake_minimum_required(VERSION 3.18)

# set the project name
project(Cacus 	VERSION 1.0
				DESCRIPTION "Vulkan rendering engine"
				LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)

add_library(cacus STATIC)

add_subdirectory(src)
add_subdirectory(lib)

target_link_libraries(cacus PRIVATE ${Vulkan_LIBRARIES} glfw PUBLIC glm)
target_include_directories(cacus PRIVATE ${Vulkan_INCLUDE_DIRS})



# Examples
add_subdirectory(example)

# Tests
enable_testing()
add_subdirectory(test)

add_test(
    NAME unit_tests
    COMMAND unit_tests)