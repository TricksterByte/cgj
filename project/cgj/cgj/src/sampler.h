#pragma once
#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "shader.h"

class Sampler;
class NearestSampler;
class LinearSampler;
class NearestMipmapNearestSampler;
class NearestMipmapLinearSampler;
class LinearMipmapNearestSampler;
class LinearAnisotropicSampler;
class MaxAnisotropicSampler;

///////////////////////////////////////////////// Sampler

class Sampler {
protected:
	GLuint samplerId;

public:
	Sampler();
	~Sampler();
	void bind(GLuint unit);
	void unbind(GLuint unit);
	virtual void create() = 0;
};

///////////////////////////////////////////////// NearestSampler

class NearestSampler : Sampler {
public:
	void create();
};

///////////////////////////////////////////////// LinearSampler

class LinearSampler : Sampler {
public:
	void create();
};

///////////////////////////////////////////////// NearestMipmapNearestSampler

class NearestMipmapNearestSampler : Sampler {
public:
	void create();
};

///////////////////////////////////////////////// NearestMipmapLinearSampler

class NearestMipmapLinearSampler : Sampler {
public:
	void create();
};

///////////////////////////////////////////////// LinearMipmapNearestSampler

class LinearMipmapNearestSampler : Sampler {
public:
	void create();
};

///////////////////////////////////////////////// LinearAnisotropicSampler

class LinearAnisotropicSampler : Sampler {
public:
	void create();
};

///////////////////////////////////////////////// MaxAnisotropicSampler

class MaxAnisotropicSampler : Sampler {
public:
	void create();
};


#endif