#version 330 core

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec3 FragPos;
out vec3 exNormal;
out vec3 ViewPos;

in vec3 inVertex;
in vec3 inNormal;

uniform vec4 ClipPlane;

void main(void)
{
	//exNormal = NormalMatrix * inNormal;
	exNormal = transpose(inverse(mat3(ModelMatrix))) * inNormal;
	FragPos = vec3(ModelMatrix * vec4(inVertex, 1));
	ViewPos = vec3(ViewMatrix * ModelMatrix * vec4(inVertex, 1));

	gl_ClipDistance[0] = dot(ModelMatrix * vec4(inVertex, 1), ClipPlane);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inVertex, 1);
}