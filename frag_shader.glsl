#version 450

out vec4 color;

// Here the following uniform are not used,
// but it's good practice to write them.
uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;

void main() { 
	color = vec4(1.0, 0.0, 0.0, 1.0); // hardcoded red color for every cube pixel
}