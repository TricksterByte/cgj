#version 330 core

in vec4 FragPos;

uniform vec3 LightPosition;
uniform float FarPlane;

void main()
{
	float lightDistance = length(FragPos.xyz - LightPosition);

	lightDistance = lightDistance / FarPlane;

	gl_FragDepth = lightDistance;
}