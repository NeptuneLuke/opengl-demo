#include "my_utils.hpp"

#include <GL/glew.h> // not actually required, because of utils.hpp
#include <GLFW/glfw3.h> // not actually required, because of utils.hpp
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>

#include <iostream> // not actually required, because of utils.hpp
#include <cstdlib> // EXIT_FAILURE | EXIT_SUCCESS
#include <cstdint> // uint32_t



/* ----------------------------------------------------------------- */
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#define GLEW_VAO_NUMS 1 // Vertex Array Object size, at least one
#define GLEW_VBO_NUMS 2 // Vertex Buffer Object size, one for every single object to render

GLuint glew_rendering_program;
GLuint glew_vao[GLEW_VAO_NUMS]; // Vertex Array Object
GLuint glew_vbo[GLEW_VBO_NUMS]; // Vertex Buffer Object

float camera_x, camera_y, camera_z;
float cube_pos_x, cube_pos_y, cube_pos_z;
float pyramid_pos_x, pyramid_pos_y, pyramid_pos_z;

// Allocates variables used in display(), so that they won't need
// to be allocated during rendering.
GLuint projection_loc, modelview_loc;
glm::mat4 view_mat, model_mat, perspective_mat, modelview_mat;
int width, height;
float aspect_ratio;

GLuint pyramid_texture;
/* ----------------------------------------------------------------- */


/* ----------------------------------------------------------------- */
void init(GLFWwindow* window);
void display(GLFWwindow* window, double delta_time);

// Sets up the vertices of the objects.
void setup_vertices();

// Rebuilds the perspective matrix according to the new window sizes.
void window_reshape_perspective_matrix_CALLBACK(GLFWwindow* window, int new_width, int new_height);
/* ----------------------------------------------------------------- */


// Sets up the vertices of the objects.
void setup_vertices() {

	// We have 6 cube faces, so 12 triangles.
	// Each triangle is 3 vertices, so 3 x 12 = 36 vertices.
	// Each vertex has 3 values (X, Y, Z), so 3 x 36 = 108 vertices positions.
	// Obviously the triangles have vertices in common, but we are sending
	// each triangle's vertices down the pipeline separately, so we 
	// specify them separately.
	// In the cube, (0, 0, 0) is the center, and the corners range from
	// -1.0 to 1.0
	float cube_vertices_pos[108] = {
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

	// We have one square base and 4 faces.
	// So 1 square = 2 triangles. Plus the 4 triangles of the faces.
	// A total of 6 triangles, so 3 x 6 = 18 vertices.
	// Each vertex has 3 values (X, Y, Z), so 3 x 18 = 54 vertices positions.
	float pyramid_vertices_pos[54] = {
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
	};

	// Setup the pyramid's texture coordinates.
	// Obviously this will require to have its VBO.
	float pyramid_texture_coords[36] = {
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // top and right faces
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // back and left faces
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }; // base triangles


	// Setup VAO and VBO.
	glGenVertexArrays(GLEW_VAO_NUMS, glew_vao); // at least 1 VAO
	glBindVertexArray(glew_vao[0]);
	glGenBuffers(GLEW_VBO_NUMS, glew_vbo); // at least 2 VBOs

	/*
	// Load the cube vertices into the 0th VBO buffer.
	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_pos), cube_vertices_pos, GL_STATIC_DRAW);
	*/
	

	// Load the pyramid vertices into the 0th VBO buffer.
	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices_pos), pyramid_vertices_pos, GL_STATIC_DRAW);

	// Load the pyramid texture coordinates into the 1th VBO buffer.
	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_texture_coords), pyramid_texture_coords, GL_STATIC_DRAW);
}
/* ----------------------------------------------------------------- */


void init(GLFWwindow* window) {

	// Read shader code and build the rendering program.
	glew_rendering_program = myutils::glew_create_shader_program("vert_shader.glsl", "frag_shader.glsl");

	// Setup camera position.
	camera_x = 0.0f;
	camera_y = 0.0f;
	camera_z = 12.0f;

	// Setup cube position.
	cube_pos_x = 0.0f;
	cube_pos_y = -2.5f; // shift down Y to reveal perspective
	cube_pos_z = 0.0f;

	// Setup pyramid position.
	pyramid_pos_x = 3.0f;
	pyramid_pos_y = -2.5f; // shift down Y to reveal perspective
	pyramid_pos_z = 2.0f;

	// Create objects vertices and load them.
	setup_vertices();

	// Pre-computing the perspective matrix.
	glfwGetFramebufferSize(window, &width, &height);
	aspect_ratio = (float)width / (float)height;
	perspective_mat = glm::perspective(
		1.0472f, // 1.0472 radians = 60 degrees
		aspect_ratio,
		0.1f, // 0.1f is the near clipping plane
		1000.0f); // 1000.0f is the far clipping plane

	pyramid_texture = myutils::load_texture("brick1.jpg");
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

	// Enable back-face culling
	glEnable(GL_CULL_FACE);

	// Adjust OpenGL settings to enable HSR with the specific
	// depth testing to use.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Get the uniform variables for the model-view and projection matrices
	// from the GLSL shaders files.
	projection_loc = glGetUniformLocation(glew_rendering_program, "projection_matrix");
	modelview_loc = glGetUniformLocation(glew_rendering_program, "modelview_matrix");

	// We are no longer building the perspective matrix
	// for every frame, because we don't need it, except for
	// when resizing the window.
	// The perspective matrix build was moved to init().

	// View matrix is always the same regardless of the object
	// but we still need it to build it for every frame 
	// (in the future it will become useful).
	view_mat = glm::translate(
		glm::mat4(1.0f) /* identity matrix*/,
		glm::vec3(-camera_x, -camera_y, -camera_z));


	/*
	// Draw the cube.
	
	// Set winding order for back-face culling.
	// The cube vertices have clockwise winding order.
	glFrontFace(GL_CW);

	// Model matrix and consequently modelview matrix change
	// as the object to draw changes.
	model_mat = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(cube_pos_x, cube_pos_y, cube_pos_z));

	modelview_mat = model_mat * view_mat;

	glUniformMatrix4fv(modelview_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat));
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(perspective_mat));

	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Draw triangles made of 36 vertices (the cube we created) starting from vertex 0.
	glDrawArrays(GL_TRIANGLES, 0, 36);
	*/


	// Draw the pyramid.

	model_mat = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(pyramid_pos_x, pyramid_pos_y, pyramid_pos_z));

	modelview_mat = model_mat * view_mat;

	glUniformMatrix4fv(modelview_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat));
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(perspective_mat));

	// Associate VBO with the corresponding vertex attribute in the vertex shader.
	// Enables the buffer containing the cube vertices and attaches it to the
	// 0th vertex attribute to prepare for sending the vertices to the shader.
	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Draw the texture on the pyramid.
	glBindBuffer(GL_ARRAY_BUFFER, glew_vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pyramid_texture);

	// Set winding order for back-face culling.
	// The pyramid vertices have counter-clockwise winding order.
	glFrontFace(GL_CCW);

	// Draw triangles made of 18 vertices (the pyramid we created) starting from vertex 0.
	glDrawArrays(GL_TRIANGLES, 0, 18);

}

// Rebuilds the perspective matrix according to the new window sizes.
void window_reshape_perspective_matrix_CALLBACK(GLFWwindow* window, int new_width, int new_height) {

	aspect_ratio = (float)new_width / (float)new_height;
	glViewport(0, 0, new_width, new_height);
	perspective_mat = glm::perspective(
		1.0472f, // 1.0472 radians = 60 degrees
		aspect_ratio,
		0.1f, // 0.1f is the near clipping plane
		1000.0f); // 1000.0f is the far clipping plane
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

	// Link the callback to the function that
	// rebuilds the perspective matrix according
	// to the new window sizes.
	glfwSetWindowSizeCallback(window, window_reshape_perspective_matrix_CALLBACK);

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