#include "shadows_fbos.h"
#include "matrix_factory.h"
#include "math_helpers.h"

ShadowsFramebuffers::ShadowsFramebuffers(const int shadowW, const int shadowH, const int displayW, const int displayH) : 
	SHADOW_WIDTH(shadowW), SHADOW_HEIGHT(shadowH),
	DISPLAY_WIDTH(displayW), DISPLAY_HEIGHT(displayH) {
	
	depthCubeMapId = -1;
	depthFramebuffer = -1;

	createDepthCubeMap();
	createFramebuffer();
}

ShadowsFramebuffers::~ShadowsFramebuffers() {}

void ShadowsFramebuffers::destroy() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &depthCubeMapId);
	glDeleteFramebuffers(1, &depthFramebuffer);
}

void ShadowsFramebuffers::createDepthCubeMap() {
	glGenTextures(1, &depthCubeMapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMapId);

	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void ShadowsFramebuffers::createFramebuffer() {
	glGenFramebuffers(1, &depthFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMapId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowsFramebuffers::bindFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer); 
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
}

void ShadowsFramebuffers::unbindFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}