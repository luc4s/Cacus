#include "cacus.h"

#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

Cacus::Cacus() {

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

	std::cout << "Bonjour tout le monde -> " << test[0] << std::endl;
}