#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib> // EXIT_FAILURE | EXIT_SUCCESS
#include <cstdint> // uint32_t


/* ----------------------------------------------------------------- */
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


void init(GLFWwindow* window) {

}

void display(GLFWwindow* window, double currentTime) {
	
	glClearColor(1.0, 0.0, 0.0, 1.0); // set the color to be applied when clearing background
	glClear(GL_COLOR_BUFFER_BIT); // fill the color buffer with the color we specified above
}
/* ----------------------------------------------------------------- */


int main() {

	// Init the GLFW library
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Specify the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create window and OpenGL context with the previously
	// specified hints (OpenGL v4.3)
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL demo", nullptr, nullptr);
	glfwMakeContextCurrent(window); // explicitly set the window as the OpenGL context

	// Init the GLEW library
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1); // enables VSync (1)

	init(window);

	// Check for events until the window is closed
	while (!glfwWindowShouldClose(window)) {
		
		// including the current time ensures that our animations
		// run at the same speed regardless of the computer being used
		display(window, glfwGetTime());
		glfwSwapBuffers(window); // enables VSync (2) and paints the screen
		glfwPollEvents(); // check for events
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}