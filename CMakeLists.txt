cmake_minimum_required(VERSION 3.17)

# set the project name
project(Cacus 	VERSION 1.0
				DESCRIPTION "Vulkan rendering engine"
				LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cacus STATIC)

target_include_directories(cacus PUBLIC include)
set(CACUS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(lib)


# Examples
add_subdirectory(example)