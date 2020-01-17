#version 330

in vec3 inVertex;
in vec3 inColor;
in float inSize;

out vec3 gColor;
out float gSize;

void main(void)
{
	gColor = inColor;
	gSize = inSize;
	gl_Position = vec4(inVertex, 1);
}