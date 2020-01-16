#version 330

uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 ModelMatrix;

in vec3 inVertex;
in vec3 inColor;
in float inSize;
in vec4 inTexOffsets;
in float inBlend;

out vec3 gColor;
out float gSize;
out vec4 gTexOffsets;
out float gBlend;

void main(void)
{
	gColor = inColor;
	gSize = inSize;
	gTexOffsets = inTexOffsets;
	gBlend = inBlend;
	gl_Position = ViewMatrix * ModelMatrix * vec4(inVertex, 1);
}