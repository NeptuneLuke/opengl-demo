#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>

#include <iostream>
#include <cstdlib> // EXIT_FAILURE | EXIT_SUCCESS
#include <cstdint> // uint32_t

#include "utils.hpp"


/* ----------------------------------------------------------------- */
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#define GLEW_VAO_NUMS 1 // Vertex Array Object size
#define GLEW_VBO_NUMS 2 // Vertex Buffer Object size = one for every single object to render

GLuint glew_rendering_program;
GLuint glew_vao[GLEW_VAO_NUMS]; // Vertex Array Object
GLuint glew_vbo[GLEW_VBO_NUMS]; // Vertex Buffer Object

float camera_x, camera_y, camera_z;
float cube_pos_x, cube_pos_y, cube_pos_z;

// Allocates variables used in display(), so that they won't need
// to be allocated during rendering.
GLuint modelview_loc, projection_loc;
int width, height;
float aspect_ratio;
glm::mat4 perspective_mat, view_mat, model_mat, modelview_mat;
/* ----------------------------------------------------------------- */


/* ----------------------------------------------------------------- */
void init(GLFWwindow* window);
void display(GLFWwindow* window, double delta_time);

void setup_vertices();
/* ----------------------------------------------------------------- */


// Sets up the vertices of the 2x2x2 cube.
void setup_vertices() {
	
	// We have 6 cube faces, so 12 triangles.
	// Each triangle is 3 vertices, so 3 x 12 = 36 vertices.
	// Each vertex has 3 values (X, Y, Z), so 3 x 36 = 108 vertices positions.
	// Obviously the triangles have vertices in common, but we are sending
	// each triangle's vertices down the pipeline separately, so we 
	// specify them separately.
	// In the cube, (0, 0, 0) is the center, and the corners range from
	// -1.0 to 1.0
	float vertices_pos[108] = {
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};
	
	// Setup VAO and VBO.
	glGenVertexArrays(GLEW_VAO_NUMS, glew_vao);
	glBindVertexArray(glew_vao[0]);
	glGenBuffers(GLEW_VBO_NUMS, glew_vbo);

	// Load the cube vertices into the 0th VBO buffer.
	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pos), vertices_pos, GL_STATIC_DRAW);
}
/* ----------------------------------------------------------------- */


void init(GLFWwindow* window) {

	// Read shader code and build the rendering program.
	glew_rendering_program = myutils::glew_create_shader_program();
	
	// Setup camera position.
	camera_x = 0.0f;
	camera_y = 0.0f;
	camera_z = 8.0f;

	// Setup cube position.
	cube_pos_x = 0.0f;
	cube_pos_y = -2.5f; // shift down Y to reveal perspective
	cube_pos_z = 0.0f;

	// Create cube vertices and load them.
	setup_vertices();
}

void display(GLFWwindow* window, double delta_time) {
	
	// Clear the depth buffer each frame 
	// (it isn't necessary now, but will become fundamental
	// in future animated scenes, to ensure that depth comparisons
	// aren't affected by old depth data).
	// This is called Hidden Surface Removal (HSR).
	glClear(GL_DEPTH_BUFFER_BIT);

	glClearColor(1.0, 1.0, 1.0, 1.0); // set the color to be applied when clearing background
	glClear(GL_COLOR_BUFFER_BIT); // fill the color buffer with the color we specified above

	// Try removing the glClear(GL_COLOR_BUFFER_BIT) call
	// above and see the results!

	// Enable shaders, installing the GLSL code
	// on the GPU. It doesn't run the shader program,
	// it just enables subsequent OpenGL calls to determine
	// the shader's vertex attribute and uniform locations.
	glUseProgram(glew_rendering_program);
	
	// Get the uniform variables for the model-view and projection matrices
	// from the GLSL shaders files.
	modelview_loc = glGetUniformLocation(glew_rendering_program, "modelview_matrix");
	projection_loc = glGetUniformLocation(glew_rendering_program, "projection_matrix");

	// Build perspective matrix.
	// Note that the perspective matrix could technically be rebuilt
	// only when the window is resized, and not at every frame (as we are doing now).
	// However for semplicitly, for now we will leave it like that.
	glfwGetFramebufferSize(window, &width, &height);
	aspect_ratio = (float)width / (float)height;
	perspective_mat = glm::perspective(1.0472f, aspect_ratio, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// Build view matrix, model matrix, model-view matrix.
	view_mat = glm::translate(
					glm::mat4(1.0f) /* identity matrix*/,
					glm::vec3(-camera_x, -camera_y, -camera_z));

	// We can add animation to the cube by building the model matrix
	// using a varying translation and rotation based on the time.
	// Translation on the left.
	glm::mat4 translation_mat = 
		glm::translate(
			glm::mat4(1.0f),
			glm::vec3(sin(0.35f * delta_time) * 2.0f,
				      cos(0.52f * delta_time) * 2.0f,
				      sin(0.7f * delta_time) * 2.0f));

	// Rotation on the right.
	// The 1.75 constant adjusts the rotation speed.
	// Y axis rotation.
	glm::mat4 rotation_mat = 
		glm::rotate(
			glm::mat4(1.0f),
			1.75f * (float)delta_time,
			glm::vec3(0.0f, 1.0f, 0.0f));
	
	// X axis rotation.
	rotation_mat =
		glm::rotate(rotation_mat,
					1.75f * (float)delta_time,
					glm::vec3(1.0f, 0.0f, 0.0f));
	
	// Z axis rotation.
	rotation_mat =
		glm::rotate(rotation_mat,
			1.75f * (float)delta_time,
			glm::vec3(0.0f, 0.0f, 1.0f));

	// The order of multiplication is significant!
	// Try inverting the order and see the results!
	// The computation of a vertex is right to left,
	// meaning that the rotation is done first, and then the translation.
	model_mat = translation_mat * rotation_mat;
	/* model_mat = glm::translate(
					glm::mat4(1.0f),
					glm::vec3(cube_pos_x, cube_pos_y, cube_pos_z));
	*/


	modelview_mat = view_mat * model_mat;

	// Copy perspective and model-view matrices to corresponding uniform variables
	// in the GLSL shaders files.
	glUniformMatrix4fv(modelview_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat));
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(perspective_mat));

	// Associate VBO with the corresponding vertex attribute in the vertex shader.
	// Enables the buffer containing the cube vertices and attaches it to the
	// 0th vertex attribute to prepare for sending the vertices to the shader.
	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Adjust OpenGL settings to enable HSR with the specific
	// depth testing to use.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Draw triangles made of 36 vertices (the cube we created) starting from vertex 0.
	glDrawArrays(GL_TRIANGLES, 0, 36);
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