#version 330

uniform sampler2D ParticleTexture;

in vec4 fColor;
in vec2 fTexCoords1;
in vec2 fTexCoords2;
in float fBlend;

out vec4 outFragColor;

void main(void)
{
	vec4 color1 = texture(ParticleTexture, fTexCoords1);
	vec4 color2 = texture(ParticleTexture, fTexCoords2);
	outFragColor = mix(color1, color2, fBlend);
}