#version 330

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 gColor[];

out vec4 fColor;

void main()
{
	fColor = vec4(gColor[0], 0.2);
	vec4 p = gl_in[0].gl_Position;

	gl_Position = ProjectionMatrix * p;
	EmitVertex();
	gl_Position = ProjectionMatrix * vec4(p.x, p.y - 0.1, p.zw);
	EmitVertex();
	EndPrimitive();
}