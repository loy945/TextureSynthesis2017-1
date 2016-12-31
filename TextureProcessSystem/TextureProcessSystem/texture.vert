#version 330
uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;
in vec4 position;
in vec2 texCoord1;
in vec2 texCoord2;
in vec2 texCoord3;
in vec2 texCoord4;
in vec2 texCoord5;
in vec2 texCoord6;
in vec2 texCoord7;
in vec2 texCoord8;
in vec2 texCoord9;
in vec2 texCoord10;
in vec2 texCoord11;
in vec2 texCoord12;
in vec4 texID_1;
in vec4 texID_2;
in vec4 normals;

out vec2 texCoordV1;
out vec2 texCoordV2;
out vec2 texCoordV3;
out vec2 texCoordV4;
out vec2 texCoordV5;
out vec2 texCoordV6;
out vec2 texCoordV7;
out vec2 texCoordV8;
out vec2 texCoordV9;
out vec2 texCoordV10;
out vec2 texCoordV11;
out vec2 texCoordV12;

out vec4 texID_1V;
out vec4 texID_2V;
out vec4 normalsV;
out vec4 positionV;
void main () {	
	for(int i=0;i<4;i++)
	{
		texID_1V[i]=texID_1[i];
		texID_2V[i]=texID_2[i];
		normalsV[i]=normals[i];
		positionV[i]=position[i];
	}
	texCoordV1 = texCoord1;
	texCoordV2 = texCoord2;
	texCoordV3 = texCoord3;
	texCoordV4 = texCoord4;
	texCoordV5 = texCoord5;
	texCoordV6 = texCoord6;
	texCoordV7 = texCoord7;
	texCoordV8 = texCoord8;
	texCoordV9 = texCoord9;
	texCoordV10 = texCoord10;
	texCoordV11 = texCoord11;
	texCoordV12 = texCoord12;
	

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix *position;	
}