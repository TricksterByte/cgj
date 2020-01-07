#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inVertex;
out vec4 clipSpace;
out vec2 exTexcoord;

const float tiling = 0.5;

void main(void)
{
	clipSpace = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inVertex, 1.0);
	exTexcoord = vec2(clipSpace.x / 2.0 + 0.5, clipSpace.y / 2.0 + 0.5) * tiling;

	gl_Position = clipSpace;
}