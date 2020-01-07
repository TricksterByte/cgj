#include <iostream>

#include "sampler.h"

///////////////////////////////////////////////// Sampler

Sampler::Sampler() {
	glGenSamplers(1, &samplerId);
	glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Sampler::~Sampler() {}

void Sampler::bind(GLuint unit) {
	glBindSampler(unit, samplerId);
}

void Sampler::unbind(GLuint unit) {
	glBindSampler(unit, 0);
}

///////////////////////////////////////////////// NearestSampler

void NearestSampler::create() {
	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

///////////////////////////////////////////////// LinearSampler

void LinearSampler::create() {
	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

///////////////////////////////////////////////// NearestMipmapNearestSampler

void NearestMipmapNearestSampler::create() {
	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
}

///////////////////////////////////////////////// NearestMipmapLinearSampler

void NearestMipmapLinearSampler::create() {
	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
}

///////////////////////////////////////////////// LinearAnisotropicSampler

void LinearAnisotropicSampler::create() {
	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
}

///////////////////////////////////////////////// MaxAnisotropicSampler

void MaxAnisotropicSampler::create() {
	GLfloat max = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL_NEAREST);
	std::cout << "Sampler set to maximum anisotropu (" << max << ")." << std::endl;
}