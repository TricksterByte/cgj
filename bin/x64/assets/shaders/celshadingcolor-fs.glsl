#version 330 core

in vec3 exNormal;
in vec3 FragPos;
in vec3 ViewPos;
out vec4 outFragColor;

uniform vec3 Color1;
uniform vec3 Color2;
uniform float Steps;
uniform float LightIntensity;

uniform samplerCube DepthCube;
uniform vec3 LightPosition;
uniform float FarPlane;

vec3 ComputeColorToon(const in float intensity)
{
	float factor = floor(intensity * Steps) * (1 / (Steps - 1));

	return vec3(Color1.x + factor * (Color1.x - Color2.x), Color1.y + factor * (Color1.y - Color2.y), Color1.z + factor * (Color1.z - Color2.z));
}

float ShadowCalculation(vec3 fragPos)
{
//	vec3 fragToLight = fragPos - LightPosition;
//	float closestDepth = texture(DepthCube, fragToLight).r;
//	closestDepth *= FarPlane;
//	float currentDepth = length(fragToLight);
//	float bias = 0.05;
//	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
//	return shadow;

//	vec3 sampleOffsetDirections[20] = vec3[]
//	(
//	   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
//	   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
//	   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
//	   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
//	   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
//	);   
//
//	vec3 fragToLight = fragPos - LightPosition;
//	float currentDepth = length(fragToLight);
//	float bias = 0.05;
//
//	float shadow = 0.0;
//	int samples  = 20;
//	float viewDistance = length(ViewPos);
//	float diskRadius = 0.05;
//	for(int i = 0; i < samples; ++i)
//	{
//		float closestDepth = texture(DepthCube, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
//		closestDepth *= FarPlane;
//		if(currentDepth - bias > closestDepth)
//			shadow += 1.0;
//	}
//	shadow /= float(samples); 

	vec3 fragToLight = fragPos - LightPosition;
	float currentDepth = length(fragToLight);
	
	float shadow  = 0.0;
	float bias    = 0.05; 
	float samples = 4.0;
	float offset  = 0.1;
	for(float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for(float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for(float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float closestDepth = texture(DepthCube, fragToLight + vec3(x, y, z)).r; 
				closestDepth *= FarPlane;
				if(currentDepth - bias > closestDepth)
					shadow += 1.0;
			}
		}
	}
	shadow /= (samples * samples * samples);

	return shadow;
}

void main(void)
{
	float shadow = ShadowCalculation(FragPos);

	vec3 N = normalize(exNormal);
	vec3 L = normalize(LightPosition - FragPos);
	float intensity = max(dot(L, N), 0.0);

	vec3 color = ComputeColorToon(intensity);
	vec3 ambient = 0.8 * color;
	vec3 lightColor = vec3(LightIntensity);
	vec3 diffuse = intensity * lightColor * 1.5;

	outFragColor = vec4((ambient + (1 - shadow) * diffuse) * color, 1);
}