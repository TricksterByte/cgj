#version 330 core

in vec3 inVertex;

uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ModelMatrix * vec4(inVertex, 1.0);
}