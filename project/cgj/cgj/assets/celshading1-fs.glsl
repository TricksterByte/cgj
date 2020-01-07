#version 330 core

in float exIntensity;
out vec4 outFragColor;

const float Step1 = 0.25f;
const float Step2 = 0.50f;
const float Step3 = 0.75f;

const vec3 ColorStep0 = vec3(0.2, 0.1, 0.1);
const vec3 ColorStep1 = vec3(0.4, 0.2, 0.2);
const vec3 ColorStep2 = vec3(0.6, 0.3, 0.3);
const vec3 ColorStep3 = vec3(1.0, 0.5, 0.5);

vec3 computeColorDiffuse(const in float intensity)
{
	return vec3(max(intensity, 0.0));
}

vec3 computeColorToon(const in float intensity)
{
	if (intensity > Step3)
		return ColorStep3;
	else if (intensity > Step2)
		return ColorStep2;
	else if (intensity > Step1)
		return ColorStep1;
	else
		return ColorStep0;
}

void main(void)
{
	vec3 color;
//	color = vec3(1.0);
//	color = computeColorDiffuse(exIntensity);
	color = computeColorToon(exIntensity);
	outFragColor = vec4(color, 1.0);
}