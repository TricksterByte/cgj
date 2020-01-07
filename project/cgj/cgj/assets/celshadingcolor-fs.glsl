#version 330 core

in vec3 exNormal;
out vec4 outFragColor;

uniform vec3 Color1;
uniform vec3 Color2;
uniform float Steps;

uniform vec3 LightDirection;

vec3 computeColorToon(const in float intensity)
{
	float factor = floor(intensity * Steps) * (1 / (Steps - 1));

	return vec3(Color1.x + factor * (Color1.x - Color2.x), Color1.y + factor * (Color1.y - Color2.y), Color1.z + factor * (Color1.z - Color2.z));
}

void main(void)
{
	vec3 L = normalize(LightDirection);
	vec3 N = normalize(exNormal);

	float intensity = max(dot(-L, N), 0.0); 

	outFragColor = vec4(computeColorToon(intensity), 1.0);
}