#version 450

layout (location=0) in vec3 position;

// now model and view matrices are separated
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform float time_factor; // for animation and placement of ojbects

out vec4 varying_colors; // color gradient based on the pixels location

mat4 rotate_x(float rad);
mat4 rotate_y(float rad);
mat4 rotate_z(float rad);
mat4 translate(float x, float y, float z);


/*
 * in -> the elements will receives values from the buffer
 * out -> the elements will send values to the next stage in the pipeline
 * remember that gl_Position is an OpenGL built-in "out vec4" variable!
*/
void main() { 

	// different for each object instance
	float i = gl_InstanceID + time_factor;
	
	// these are the x, y, and z components for the translation, done below
	float a = sin(2.0 * i) * 10.0;
	float b = sin(3.0 * i) * 10.0;
	float c = sin(4.0 * i) * 10.0;

	// build rotation and translation matrices to be applied
	// to this current cube's model matrix
	// changing the values 3 * i changes the speed of rotation of the objects
	mat4 local_rotation_x = rotate_x(3 * i);
	mat4 local_rotation_y = rotate_y(3 * i);
	mat4 local_rotation_z = rotate_z(3 * i);
	mat4 local_translation = translate(a, b, c);
	
	// build the model matrix and then the model-view matrix
	mat4 new_model_matrix = 
		model_matrix * local_translation 
		* local_rotation_x * local_rotation_y * local_rotation_z;
	
	mat4 modelview_matrix = view_matrix * new_model_matrix;
	
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

mat4 rotate_x(float rad) {
	
	mat4 rot_mat = 
		mat4(1.0, 0.0, 0.0, 0.0,
			 0.0, cos(rad), -sin(rad), 0.0,
			 0.0, sin(rad), cos(rad), 0.0,
			 0.0, 0.0, 0.0, 1.0 );
	
	return rot_mat;
}

mat4 rotate_y(float rad) {
	
	mat4 rot_mat = 
		mat4(cos(rad), 0.0, sin(rad), 0.0,
			  0.0, 1.0, 0.0, 0.0,
			  -sin(rad), 0.0, cos(rad), 0.0,
			  0.0, 0.0, 0.0, 1.0 );
	
	return rot_mat;
}

mat4 rotate_z(float rad) {
	
	mat4 rot_mat = 
		mat4(cos(rad), -sin(rad), 0.0, 0.0,
			  sin(rad), cos(rad), 0.0, 0.0,
			  0.0, 0.0, 1.0, 0.0,
			  0.0, 0.0, 0.0, 1.0 );
	
	return rot_mat;
}

mat4 translate(float x, float y, float z) {

	mat4 translate_mat = 
		mat4 (1.0, 0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0, 0.0,
			  0.0, 0.0, 1.0, 0.0,
			  x, y, z, 1.0);
			  
	return translate_mat;
}