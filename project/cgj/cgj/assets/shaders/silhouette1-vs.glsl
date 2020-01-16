#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inVertex;
in vec3 inNormal;

uniform float Offset;
uniform vec4 ClipPlane;

void main(void)
{
	mat4 mv  = ViewMatrix * ModelMatrix;
	mat4 mvp = ProjectionMatrix * mv;

	vec3 eyePos = (mv * vec4(inVertex, 1)).xyz;
	float dist = length(eyePos);

	vec3 v = inVertex + normalize(inNormal) * Offset * (25 / dist);
	vec4 mcVertex = ModelMatrix * vec4(v, 1.0);
	gl_ClipDistance[0] = dot(mcVertex, ClipPlane);

	gl_Position = mvp * vec4(v, 1.0);
}