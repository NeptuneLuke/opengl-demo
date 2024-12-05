#include "utils.hpp"

namespace myutils {


	GLuint glew_create_shader_program() {

		GLint vert_compiled;
		GLint frag_compiled;
		GLint shaders_linked;

		std::string vert_shader_str = read_shader_source("vert_shader.glsl");
		std::string frag_shader_str = read_shader_source("frag_shader.glsl");

		// Converts to C's null terminated strings
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
		if (frag_compiled != 1) {
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

	void print_shader_log(GLuint shader) {

		int length = 0;
		int chars_written = 0;
		char* log;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			log = (char*)malloc(length);
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
}
