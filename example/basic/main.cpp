#include <iostream>

#include <GLFW/glfw3.h>

#include <cacus.h>

using namespace std;

int main() {
	Cacus cacus;
	cout << "Hello World!" << endl;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}