#version 450

uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;

layout (location=0) in vec3 position;

void main() { 
	gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
}
// The multiplication applies the matrix transforms to the vertex, converting it to
// camera space. Those values are put in the built-in OpenGL output variable gl_Position
// and the proceed through the pipeline and are interpolated by the rasterizer.
// The interpolated pixel locations (fragments), are then sent to the fragment shader
// which sets the color of an outputted pixel. The fragment shader processes the pixels
// one by one (with a separate invocation for each pixel).


/*
 * in -> the elements will receives values from the buffer
 * out -> the elements will send values to the next stage in the pipeline
 * remember that gl_Position is an OpenGL built-in "out vec4" variable!
*/