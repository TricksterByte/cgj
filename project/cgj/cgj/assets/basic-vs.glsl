#version 330 core

in vec4 inVertex;

uniform mat4 ModelMatrix;

uniform Camera
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * inVertex;
}