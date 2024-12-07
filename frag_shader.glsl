#version 450

in vec2 texture_pixels;
out vec4 color;

// Here the following uniform are not used,
// but it's good practice to write them.
uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

layout(binding=0) uniform sampler2D texture_sampler;

void main() { 
	color = texture(texture_sampler, texture_pixels);
}

/*
 * in -> the elements will receives values from the buffer
 * out -> the elements will send values to the next stage in the pipeline
*/