#pragma once
#ifndef __SHADOWMAP_H__
#define __SHADOWMAP_H__

#include "shadows_fbos.h"
#include "vectors.h"
#include "matrices.h"
#include "i_updatable.h"

enum Variance { INC, DEC };

class ShadowMap : public IUpdatable {
private:
	ShadowsFramebuffers* shadowFbos;

public:
	ShadowMap(const int shadowW, const int shadowH, const int displayW, const int displayH, const vec3& light, const float n, const float f);
	~ShadowMap();

	const int SHADOW_WIDTH;
	const int SHADOW_HEIGHT;
	const int DISPLAY_WIDTH;
	const int DISPLAY_HEIGHT;

	const float near;
	const float far;

	float lightIntensity = 0.15f;
	float lastIntensity = 0.15f;
	float step = 0.05f;
	bool on = true;
	Variance var = INC;

	vec3 lightPos;
	mat4 shadowMats[6];
	
	void setFbos(ShadowsFramebuffers* fbos);
	ShadowsFramebuffers* getFbos();
	void calcMatrices();
	void lightIntensityToggle();

	void update(float elapsed);
};

#endif