#pragma once
#ifndef __RAIN_PARTICLE_SYSTEM_H__
#define __RAIN_PARTICLE_SYSTEM_H__

#include "particle_system.h"

class RainParticleSystem : public ParticleSystem {
	bool active = false;
public:
	RainParticleSystem(int max, bool hasTex);
	~RainParticleSystem();
	void toggle();
private:
	virtual Particle* createParticle();
};

#endif
