cmake_minimum_required(VERSION 3.17)
add_executable(basic main.cpp)

find_package(glfw3 REQUIRED)

target_link_libraries(basic cacus glfw)
target_include_directories(basic PUBLIC ${CACUS_HEADERS})