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
out vec3 toCameraVector; 

const float tiling = 2.0;

void main(void)
{
	vec3 camPos = vec3(ViewMatrix * ModelMatrix * vec4(inVertex, 1));
	vec4 worldPos = ModelMatrix * vec4(inVertex, 1.0);

	clipSpace = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inVertex, 1.0);
	exTexcoord = inTexcoord * tiling;

	toCameraVector = camPos - worldPos.xyz;

	gl_Position = clipSpace;
}