#version 450

uniform float triangle_offset;
void main() { 
	if (gl_VertexID == 0) {
		gl_Position = vec4(0.25 + triangle_offset, -0.25, 0.0, 1.0);
	}
	else if (gl_VertexID == 1) {
		gl_Position = vec4(-0.25 + triangle_offset, -0.25, 0.0, 1.0);
	}
	else {
		gl_Position = vec4(0.25 + triangle_offset, 0.25, 0.0, 1.0);
	}
}