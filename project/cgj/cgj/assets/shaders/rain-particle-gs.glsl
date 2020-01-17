#version 330

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 ModelMatrix;

in vec3 gColor[];
in float gSize[];

out vec4 fColor;

void main()
{
	fColor = vec4(gColor[0], 0.1);
	float Size = gSize[0];
	vec4 p = gl_in[0].gl_Position;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * p;
	EmitVertex();
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(p.x, p.y - Size, p.zw);
	EmitVertex();
	EndPrimitive();
}