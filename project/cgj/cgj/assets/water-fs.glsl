#version 330 core

uniform sampler2D Reflection;
uniform sampler2D Refraction;
uniform sampler2D WaterDuDv;

uniform float MoveFactor;

in vec4 clipSpace;
in vec2 exTexcoord;
in vec3 toCameraVector;
out vec4 FragmentColor;

const float waveStrength = 0.01;

void main(void)
{
	vec2 normalizedDeviceSpace = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5;
	vec2 refractionTexCoords = normalizedDeviceSpace;
	vec2 reflectionTexCoords = vec2(normalizedDeviceSpace.x, -normalizedDeviceSpace.y);

	vec2 distortion1 = (texture(WaterDuDv, vec2(exTexcoord.x + MoveFactor, exTexcoord.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(WaterDuDv, vec2(-exTexcoord.x + MoveFactor, exTexcoord.y + MoveFactor)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;

	reflectionTexCoords += totalDistortion;
	refractionTexCoords += totalDistortion;

	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999);
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

	vec4 refractColor = texture(Refraction, refractionTexCoords);
	vec4 reflectColor = texture(Reflection, reflectionTexCoords);

	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));

	//FragmentColor = mix(reflectColor, refractColor, refractiveFactor);
	FragmentColor = mix(reflectColor, vec4(0, 0, 0, 1), refractiveFactor);
	//FragmentColor = reflectColor;
}