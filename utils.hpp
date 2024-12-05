#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

namespace myutils {

	GLuint glew_create_shader_program();
	std::string read_shader_source(const char* file_path);

	void print_shader_log(GLuint shader);
	void print_program_log(int program);
	bool check_opengl_error();
}
