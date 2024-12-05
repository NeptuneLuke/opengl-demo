#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib> // EXIT_FAILURE | EXIT_SUCCESS
#include <cstdint> // uint32_t
#include <string>
#include <fstream>


/* ----------------------------------------------------------------- */
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#define GLEW_VAO_NUMS 1
GLuint glew_rendering_program;
GLuint glew_vao[GLEW_VAO_NUMS]; // vertex array object
/* ----------------------------------------------------------------- */


/* ----------------------------------------------------------------- */
void init(GLFWwindow* window);
void display(GLFWwindow* window, double current_time);

GLuint glew_create_shader_program();
void print_shader_log(GLuint shader);
void print_program_log(int program);
bool check_opengl_error();

std::string read_shader_source(const char* file_path);
/* ----------------------------------------------------------------- */


void init(GLFWwindow* window) {

	glew_rendering_program = glew_create_shader_program();
	
	// Create the vertex array object for
	// the shaders
	glGenVertexArrays(GLEW_VAO_NUMS, glew_vao);
	glBindVertexArray(glew_vao[0]);
}

void display(GLFWwindow* window, double current_time) {
	
	// glClearColor(1.0, 0.0, 0.0, 1.0); // set the color to be applied when clearing background
	// glClear(GL_COLOR_BUFFER_BIT); // fill the color buffer with the color we specified above

	glUseProgram(glew_rendering_program);
	
	glPointSize(30.0f);

	// draw one point/vertex (starting from 0)
	glDrawArrays(GL_POINTS, 0, 1);
}

GLuint glew_create_shader_program() {

	GLint vert_compiled;
	GLint frag_compiled;
	GLint shaders_linked;

	std::string vert_shader_str = read_shader_source("vert_shader.glsl");
	std::string frag_shader_str = read_shader_source("frag_shader.glsl");

	const char* vert_shader_src = vert_shader_str.c_str();

	const char* frag_shader_src = frag_shader_str.c_str();

	// Returns an index for referencing it later
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vert_shader, 1, &vert_shader_src, nullptr);
	glShaderSource(frag_shader, 1, &frag_shader_src, nullptr);

	// Compiler shaders and check errors
	glCompileShader(vert_shader);
	check_opengl_error();
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_compiled);
	if (vert_compiled != 1) {
		std::cout << "Vertex compilation failed! \n";
		print_shader_log(vert_shader);
	}

	glCompileShader(frag_shader);
	check_opengl_error();
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_compiled);
	if (vert_compiled != 1) {
		std::cout << "Fragment compilation failed! \n";
		print_shader_log(frag_shader);
	}

	// Saves the indices in shader_program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program); // ensures that are GLSL compatible

	// Check linking shaders errors
	check_opengl_error();
	glGetProgramiv(shader_program, GL_LINK_STATUS, &shaders_linked);
	if (shaders_linked != 1) {
		std::cout << "Linking shaders failed! \n";
		print_program_log(shader_program);
	}

	return shader_program;
}

void print_shader_log(GLuint shader) {

	int length = 0;
	int chars_written = 0;
	char* log;
	
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	
	if (length > 0) {
		log = (char*) malloc(length);
		glGetShaderInfoLog(shader, length, &chars_written, log);
		std::cout << "Shader info log: " << log << " \n";
		free(log);
	}
	else {
		std::cout << "Shader info log empty! \n";
	}
}

void print_program_log(int program) {

	int length = 0;
	int chars_written = 0;
	char* log;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);
		glGetProgramInfoLog(program, length, &chars_written, log);
		std::cout << "Program info log: " << log << " \n";
		free(log);
	}
	else {
		std::cout << "Program info log empty! \n";
	}
}

bool check_opengl_error() {

	bool is_error = false;
	int opengl_error = glGetError();
	
	while (opengl_error != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << opengl_error << " \n";
		is_error = true;
		opengl_error = glGetError();
	}
	
	return is_error;
}

std::string read_shader_source(const char* file_path) {

	std::string line = "";
	std::string file_content;
	std::ifstream file_stream(file_path, std::ios::in);

	while (!file_stream.eof()) {
		std::getline(file_stream, line);
		file_content.append(line + "\n");
	}

	file_stream.close();

	return file_content;
}


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