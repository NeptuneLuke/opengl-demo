#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>

#include <iostream>
#include <string>
#include <fstream>

namespace myutils {

GLuint glew_create_shader_program(
	const char* vert,
	const char* frag);

GLuint glew_create_shader_program(
	const char* vert,
	const char* geometry,
	const char* frag);

GLuint glew_create_shader_program(
	const char* vert,
	const char* tess_control, const char* tess_eval,
	const char* frag);

GLuint glew_create_shader_program(
	const char* vert,
	const char* tess_control, const char* tess_eval,
	const char* geometry,
	const char* frag);


GLuint prepare_shader(int SHADER_TYPE, const char* shader_file);
int finalize_shader_program(GLuint shader_program);
std::string read_shader_source(const char* file_path);

void print_shader_log(GLuint shader);
void print_program_log(int program);
bool check_opengl_error();

GLuint load_texture(const char* texture_file);
}
