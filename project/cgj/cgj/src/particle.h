#pragma once
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vectors.h"

class Particle {
	vec3 position;
	vec3 velocity;
	vec3 color;
	float gravityEffect;
	float lifeSpan;
	float size;
	float elapsedTime = 0;

	vec2 texOffset1 = vec2();
	vec2 texOffset2 = vec2();
	float blend = 0;

public:
	Particle(const vec3& pos, const vec3& vel, const vec3& col, float ge, float life, float sz);
	bool update(float dt, int textureRows);
	vec3 getPosition();
	vec3 getColor();
	float getSize();
	vec2 getTexOffset1();
	vec2 getTexOffset2();
	float getBlend();

private:
	void updateTexCoordInfo(int textureRows);
	void setTextureOffset(vec2& offset, int index, int rows);
};

#endif
