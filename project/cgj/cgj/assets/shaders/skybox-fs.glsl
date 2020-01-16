#version 330 core

uniform samplerCube TexCubeMap;

in vec3 exVertex;
out vec4 FragmentColor;

void main(void)
{
	FragmentColor = texture(TexCubeMap, exVertex);
}