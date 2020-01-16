#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform float NumberOfRows;

in vec3 gColor[];
in float gSize[];
in vec4 gTexOffsets[];
in float gBlend[];

out vec4 fColor;
out vec2 fTexCoords1;
out vec2 fTexCoords2;
out float fBlend;

void main()
{
	fColor = vec4(gColor[0], 1);
	fBlend = gBlend[0];
	vec4 p = gl_in[0].gl_Position;
	float Size = gSize[0];
	
	vec2 va = p.xy + vec2(-Size, -Size);
	vec2 vb = p.xy + vec2(Size, -Size);
	vec2 vc = p.xy + vec2(Size, Size);
	vec2 vd = p.xy + vec2(-Size, Size);

	float step = 1 / NumberOfRows;
	vec2 stepx = vec2(step, 0);
	vec2 stepy = vec2(0, step);

	vec2 td1 = gTexOffsets[0].xy;
	vec2 td2 = gTexOffsets[0].zw;

	vec2 ta1 = td1 + stepy;
	vec2 ta2 = td2 + stepy;

	vec2 tb1 = ta1 + stepx; 
	vec2 tb2 = ta2 + stepx;

	vec2 tc1 = tb1 - stepy;
	vec2 tc2 = tb2 - stepy;

	gl_Position = ProjectionMatrix * vec4(va, p.zw);
	fTexCoords1 = ta1;
	fTexCoords2 = ta2;
	EmitVertex();
	gl_Position = ProjectionMatrix * vec4(vb, p.zw);
	fTexCoords1 = tb1;
	fTexCoords2 = tb2;
	EmitVertex();
	gl_Position = ProjectionMatrix * vec4(vc, p.zw);
	fTexCoords1 = tc1;
	fTexCoords2 = tc2;
	EmitVertex();
	EndPrimitive();

	gl_Position = ProjectionMatrix * vec4(va, p.zw);
	fTexCoords1 = ta1;
	fTexCoords2 = ta2;
	EmitVertex();
	gl_Position = ProjectionMatrix * vec4(vc, p.zw);
	fTexCoords1 = tc1;
	fTexCoords2 = tc2;
	EmitVertex();
	gl_Position = ProjectionMatrix * vec4(vd, p.zw);
	fTexCoords1 = td1;
	fTexCoords2 = td2;
	EmitVertex();
	EndPrimitive();
}