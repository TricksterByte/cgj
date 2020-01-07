#include "water_fbos.h"

WaterFramebuffers::WaterFramebuffers(const int refractionW, const int refractionH, const int reflectionW, const int reflectionH, const int displayW, const int displayH) :
	REFRACTION_WIDTH(refractionW), REFRACTION_HEIGHT(refractionH),
	REFLECION_WIDTH(reflectionW), REFLECTION_HEIGHT(reflectionH),
	DISPLAY_WIDTH(displayW), DISPLAY_HEIGHT(displayH) {

	refractionId = refractionDepthId = reflectionId = -1;
	refractionFramebuffer = reflectionFramebuffer = rboDepth = -1;

	initializeRefractionFramebuffer();
	initializeReflectionFramebuffer();
}

WaterFramebuffers::~WaterFramebuffers() {
	destroy();
}

void WaterFramebuffers::destroy() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &refractionId);
	glDeleteTextures(1, &refractionDepthId);
	glDeleteTextures(1, &reflectionId);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &rboDepth);
	glDeleteFramebuffers(1, &refractionFramebuffer);
	glDeleteFramebuffers(1, &reflectionFramebuffer);
}

void WaterFramebuffers::createColorTextureAttachment(GLuint* id, const int width, const int height) {
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *id, 0);
}

void WaterFramebuffers::createDepthTextureAttachment(GLuint* id, const int width, const int height) {
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (unsigned char*)nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *id, 0);
}

void WaterFramebuffers::createRenderbufferObject(const int width, const int height) {
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
}

void WaterFramebuffers::createFramebuffer(GLuint* framebuffer) {
	glGenFramebuffers(1, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void WaterFramebuffers::bindFramebuffer(GLuint framebuffer, const int width, const int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
}

void WaterFramebuffers::bindRefractionFramebuffer() {
	bindFramebuffer(refractionFramebuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFramebuffers::bindReflectionFramebuffer() {
	bindFramebuffer(reflectionFramebuffer, REFLECION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFramebuffers::unbindFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void WaterFramebuffers::initializeRefractionFramebuffer() {
	createFramebuffer(&refractionFramebuffer);
	createColorTextureAttachment(&refractionId, REFRACTION_WIDTH, REFRACTION_HEIGHT);
	createDepthTextureAttachment(&refractionDepthId, REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindFramebuffer();
}

void WaterFramebuffers::initializeReflectionFramebuffer() {
	createFramebuffer(&reflectionFramebuffer);
	createColorTextureAttachment(&reflectionId, REFLECION_WIDTH, REFLECTION_HEIGHT);
	unbindFramebuffer();
}