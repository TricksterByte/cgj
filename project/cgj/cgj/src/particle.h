#pragma once
#ifndef __PARTICCLE_H__
#define __PARTICLE_H__

#include "vectors.h"

class Particle {
public:
	vec3 position;
	vec3 velocity;
	vec3 color;

	float size;

	bool gravity;
	float ttl;
	float elapsed = 0;

	Particle(const vec3& p, const vec3& v, const vec3& c, float sz, bool g, float time);
	bool update(float dt);
};

#endif