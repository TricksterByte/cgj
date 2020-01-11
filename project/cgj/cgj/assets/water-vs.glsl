#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inVertex;
in vec2 inTexcoord;
out vec4 clipSpace;
out vec2 exTexcoord;

const float tiling = 2.0;

void main(void)
{
	clipSpace = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inVertex, 1.0);
	exTexcoord = inTexcoord * tiling;

	gl_Position = clipSpace;
}