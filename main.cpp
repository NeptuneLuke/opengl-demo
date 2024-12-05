#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib> // EXIT_FAILURE | EXIT_SUCCESS
#include <cstdint> // uint32_t


/* ----------------------------------------------------------------- */
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#define GLEW_VAO_NUMS 1
GLuint glew_rendering_program;
GLuint glew_vao[GLEW_VAO_NUMS]; // vertex array object


GLuint glew_create_shader_program() {
	
	const char* vert_shader_src =
		"#version 450 \n"
		"void main() \n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

	const char* frag_shader_src =
		"version 450 \n"
		"out vec4 color; \n"
		"void main() \n"
		"{ color = vec4(0.0, 0.0, 1.0, 1.0); }";

	// Returns an index for referencing it later
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert_shader, 1, &vert_shader_src, nullptr);
	glShaderSource(frag_shader, 1, &frag_shader_src, nullptr);

	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	// Saves the indices in shader_program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program); // ensures that are GLSL compatible

	return shader_program;
}


void init(GLFWwindow* window) {

	glew_rendering_program = glew_create_shader_program();
	
	// Create the vertex array object for
	// the shaders
	glGenVertexArrays(GLEW_VAO_NUMS, glew_vao);
	glBindVertexArray(glew_vao[0]);
}

void display(GLFWwindow* window, double current_time) {
	
	glClearColor(1.0, 0.0, 0.0, 1.0); // set the color to be applied when clearing background
	glClear(GL_COLOR_BUFFER_BIT); // fill the color buffer with the color we specified above

	glUseProgram(glew_rendering_program);
	
	// draw one point/vertex (starting from 0)
	// the default size of a point in OpenGL is one pixel
	glDrawArrays(GL_POINTS, 0, 1);
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