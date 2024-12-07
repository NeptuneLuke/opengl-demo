#version 450

layout (location=0) in vec3 position;
layout (location=1) in vec2 texture_coords;

uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;

out vec2 texture_pixels;

layout (binding=0) uniform sampler2D texture_sampler;

/*
 * in -> the elements will receives values from the buffer
 * out -> the elements will send values to the next stage in the pipeline
 * remember that gl_Position is an OpenGL built-in "out vec4" variable!
*/
void main() { 
	gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
	texture_pixels = texture_coords;
}
