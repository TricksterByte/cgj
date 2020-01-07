#version 330 core

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inVertex;
in vec3 inNormal;
out vec3 exNormal;

uniform vec4 ClipPlane;

void main(void)
{
//	exNormal = inNormal;
	exNormal = NormalMatrix * inNormal;

	vec4 mcVertex = ModelMatrix * vec4(inVertex, 1.0);
	gl_ClipDistance[0] = dot(mcVertex, ClipPlane);

	gl_Position = ProjectionMatrix * ViewMatrix * mcVertex;
}