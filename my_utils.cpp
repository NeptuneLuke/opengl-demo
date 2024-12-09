#include "my_utils.hpp"

namespace myutils {


// Vertex and Fragment shaders program.
// Returns the ID reference of the program.
GLuint glew_create_shader_program(const char* vert, const char* frag) {

	// Builds shaders
	GLuint vert_shader = prepare_shader(GL_VERTEX_SHADER, vert);
	GLuint frag_shader = prepare_shader(GL_FRAGMENT_SHADER, frag);

	// Saves the indices in shader_program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	
	// Links program
	finalize_shader_program(shader_program);

	return shader_program;
}

// Vertex, Geometry, Fragment shaders program
GLuint glew_create_shader_program(const char* vert, const char* geometry, const char* frag) {

	// Builds shaders
	GLuint vert_shader = prepare_shader(GL_VERTEX_SHADER, vert);
	GLuint geometry_shader = prepare_shader(GL_GEOMETRY_SHADER, geometry);
	GLuint frag_shader = prepare_shader(GL_FRAGMENT_SHADER, frag);
	
	// Saves the indices in shader_program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, geometry_shader);
	glAttachShader(shader_program, frag_shader);
	
	// Links program
	finalize_shader_program(shader_program);

	return shader_program;
}

// Vertex, Tessellation control, Tessellation evalutaion, Fragment shaders program
GLuint glew_create_shader_program(
	const char* vert,
	const char* tess_control, const char* tess_eval,
	const char* frag) {

	// Builds shaders
	GLuint vert_shader = prepare_shader(GL_VERTEX_SHADER, vert);
	GLuint tess_control_shader = prepare_shader(GL_TESS_CONTROL_SHADER, tess_control);
	GLuint tess_eval_shader = prepare_shader(GL_TESS_EVALUATION_SHADER, tess_eval);
	GLuint frag_shader = prepare_shader(GL_FRAGMENT_SHADER, frag);


	// Saves the indices in shader_program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, tess_control_shader);
	glAttachShader(shader_program, tess_eval_shader);
	glAttachShader(shader_program, frag_shader);

	// Links program
	finalize_shader_program(shader_program);

	return shader_program;
}

// Vertex, Tessellation control, Tessellation evalutaion, Geometry, Fragment shaders program
GLuint glew_create_shader_program(
	const char* vert,
	const char* tess_control, const char* tess_eval,
	const char* geometry,
	const char* frag) {

	// Builds shaders
	GLuint vert_shader = prepare_shader(GL_VERTEX_SHADER, vert);
	GLuint tess_control_shader = prepare_shader(GL_TESS_CONTROL_SHADER, tess_control);
	GLuint tess_eval_shader = prepare_shader(GL_TESS_EVALUATION_SHADER, tess_eval);
	GLuint geometry_shader = prepare_shader(GL_GEOMETRY_SHADER, geometry);
	GLuint frag_shader = prepare_shader(GL_FRAGMENT_SHADER, frag);


	// Saves the indices in shader_program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, tess_control_shader);
	glAttachShader(shader_program, tess_eval_shader);
	glAttachShader(shader_program, geometry_shader);
	glAttachShader(shader_program, frag_shader);

	// Links program
	finalize_shader_program(shader_program);

	return shader_program;
}

// Compiles shader and returns a ID reference of it.
GLuint prepare_shader(int SHADER_TYPE, const char* shader_file) {
	
	GLint shader_compiled;
	std::string shader_str = read_shader_source(shader_file);
	const char* shader_src = shader_str.c_str(); // converts to C's null-terminated strings

	// Returns and index for referencing
	GLuint shader_ref = glCreateShader(SHADER_TYPE);

	// Compile shader
	glShaderSource(shader_ref, 1, &shader_src, nullptr);
	glCompileShader(shader_ref);

	// Check erros
	check_opengl_error();
	glGetShaderiv(shader_ref, GL_COMPILE_STATUS, &shader_compiled);
	if (shader_compiled != 1){
		
		if (SHADER_TYPE == 35633) std::cout << "Vertex ";
		if (SHADER_TYPE == 36488) std::cout << "Tess Control ";
		if (SHADER_TYPE == 36487) std::cout << "Tess Eval ";
		if (SHADER_TYPE == 36313) std::cout << "Geometry ";
		if (SHADER_TYPE == 35632) std::cout << "Fragment ";
		std::cout << "shader compilation error! \n";
		
		print_shader_log(shader_ref);
	}

	return shader_ref;
}

// Links the shaders with the program and returns a ID reference of it.
int finalize_shader_program(GLuint shader_program) {

	GLint shaders_linked;

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

// Loads texture file and returns a ID reference of it.
GLuint load_texture(const char* texture_file) {

	GLuint texture_id;

	texture_id = SOIL_load_OGL_texture(
		texture_file,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	// Sets the mipmap level to trilinear filtering.
	// This makes up for distortion artifcats caused by the fact
	// that texture image's resolutions rarely matches that of the region 
	// in the scene being texture.
	glBindTexture(GL_TEXTURE_2D, texture_id); // makes the texture passed active
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Sets the anisotropic filtering, which is different from standard
	// mipmapping because it also supports rectangular resolutions,
	// whereas standard mipmapping only supports squared resolutions (256x256 ecc...).
	// It is used to view at various angles while retaining as much detail in the texture as possible.
	// The anisotropic filtering is not a required part of OpenGL,
	// but an extension, so first we must check if our graphhics card
	// supports it.
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		GLfloat anisotropic_setting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic_setting); // maximum degree of sampling supported
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic_setting); // being applied to the active texture object
	}

	if (texture_id == 0) {
		std::cout << "Could not find texture file " << texture_file << "! \n";
	}

	return texture_id;
}

}
