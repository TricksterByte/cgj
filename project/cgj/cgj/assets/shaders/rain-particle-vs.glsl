#version 330

uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 ModelMatrix;

in vec3 inVertex;
in vec3 inColor;
in float inSize;

out vec3 gColor;

void main(void)
{
	gColor = inColor;
	
	mat4 ModelView = ViewMatrix * ModelMatrix;
	gl_Position = ModelView * vec4(inVertex, 1);
}