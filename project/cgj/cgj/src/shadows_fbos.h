#pragma once
#ifndef __SHADOWS_FBOS_H__
#define __SHADOWS_FBOS_H__

#include "texture.h"

class ShadowsFramebuffers {
private:
	const int SHADOW_WIDTH;
	const int SHADOW_HEIGHT;
	const int DISPLAY_WIDTH;
	const int DISPLAY_HEIGHT;

	void destroy();
	void createDepthCubeMap();
	void createFramebuffer();

public:
	ShadowsFramebuffers(const int shadowW, const int shadowH, const int displayW, const int displayH);
	~ShadowsFramebuffers();

	GLuint depthCubeMapId;
	GLuint depthFramebuffer;

	void bindFramebuffer();
	void unbindFramebuffer();
};

#endif