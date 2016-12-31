#version 330
uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;
in vec4 positionV2;
void main () {	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix *positionV2;
}