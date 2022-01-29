cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the project name
project(Cacus 	VERSION 1.0
				DESCRIPTION "Vulkan rendering engine"
				LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)

add_library(cacus STATIC)
target_link_libraries(cacus ${Vulkan_LIBRARIES})
target_include_directories(cacus PUBLIC include PRIVATE ${Vulkan_INCLUDE_DIRS})

set(CACUS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(lib)


# Examples
add_subdirectory(example)

# Tests
enable_testing()

add_subdirectory(test)

add_test(
    NAME unit_tests
    COMMAND unit_tests)