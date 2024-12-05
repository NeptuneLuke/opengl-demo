#version 450

in vec4 varying_colors;
out vec4 color;

// Here the following uniform are not used,
// but it's good practice to write them.
uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;

void main() { 
	//color = vec4(1.0, 0.0, 0.0, 1.0); // hardcoded red color for every cube's pixel
	color = varying_colors;
}

/*
 * in -> the elements will receives values from the buffer
 * out -> the elements will send values to the next stage in the pipeline
*/