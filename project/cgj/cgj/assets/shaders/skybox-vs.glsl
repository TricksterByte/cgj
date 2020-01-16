#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inVertex;
out vec3 exVertex;

void main(void)
{
	vec4 mcVertex = vec4(inVertex, 1.0);
	vec4 ccVertex = ProjectionMatrix * ViewMatrix * ModelMatrix * mcVertex;

	exVertex = inVertex; // MODEL SPACE
//	exVertex = (ViewMatrix * ModelMatrix * mcVertex).xyz; // EYE SPACE
	gl_Position = ccVertex.xyww;
}