#include <cacus.h>

#include <iostream>
#include <GLFW/glfw3.h>


using namespace std;

/**
 * A basic example showing how to create and init a window.
 */
int main() {
    glfwInit();

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	Cacus cacus(glfwExtensions, glfwExtensionCount);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

 
	cout << "Hello World!" << endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}