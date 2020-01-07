#version 330

layout(points) in;

layout(triangle_strip, max_vertices = 6) out;
out vec4 fColor;

uniform vec3 Color;
uniform float Size;

void main()
{
	fColor = vec4(Color, 1);

	gl_Position = gl_in[0].gl_Position + vec4(-Size, -Size, 0.0, 0.0);
	EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(Size, -Size, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(Size, Size, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position + vec4(-Size, -Size, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(Size, Size, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-Size, Size, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}