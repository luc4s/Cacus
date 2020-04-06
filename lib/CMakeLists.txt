cmake_minimum_required(VERSION 3.17)

add_subdirectory(glm)

target_link_libraries(cacus PRIVATE glm)