#version 450

layout (location=0) in vec3 position;

uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;

out vec4 varying_colors; // color gradient based on the pixels location
// when talking about programmer-defined interpolated vertex attributes
// the naming convetion of the variables include the word "varying"

void main() { 
	gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);
	varying_colors = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
	// multiplying the location by 0.5 and adding 0.5 converts the range of values
	// from [-1,1] to[0,1]
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