#version 330 core

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform Camera
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform vec3 LightDirection;

in vec3 inVertex;
in vec3 inNormal;
out float exIntensity;

void main(void)
{
//	Normal and light must be defined in the same space

/** /
//	Model/World Space (object at the origin)
	vec3 N = inNormal;
/**/
//	Eye Space
	vec3 N = normalize(NormalMatrix * inNormal); // normalize for visual clarity
//	vec3 N = vec3(ViewMatrix * ModelMatrix * vec4(inNormal, 1.0)); // equivalent?
/**/

	exIntensity = dot(-LightDirection, N);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inVertex, 1.0);
}
	