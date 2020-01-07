#pragma once
#ifndef __WATER_FBO_H__
#define __WATER_FBO_H__

#include "texture.h"

class WaterFramebuffers {
private:
	const int REFRACTION_WIDTH;
	const int REFRACTION_HEIGHT;
	const int REFLECION_WIDTH;
	const int REFLECTION_HEIGHT;
	const int DISPLAY_WIDTH;
	const int DISPLAY_HEIGHT;

	void destroy();
	void createColorTextureAttachment(GLuint* id, const int width, const int height);
	void createDepthTextureAttachment(GLuint* id, const int width, const int height);
	void createRenderbufferObject(const int width, const int height);
	void createFramebuffer(GLuint* framebuffer);
	void bindFramebuffer(GLuint framebuffer, const int width, const int height);

public:
	WaterFramebuffers(const int refractionW, const int refractionH, const int reflectionW, const int reflectionH, const int displayW, const int displayH);
	~WaterFramebuffers();

	GLuint refractionId, refractionDepthId, reflectionId;
	GLuint refractionFramebuffer, reflectionFramebuffer, rboDepth;

	void bindRefractionFramebuffer();
	void bindReflectionFramebuffer();
	void unbindFramebuffer();

	void initializeRefractionFramebuffer();
	void initializeReflectionFramebuffer();
};

#endif