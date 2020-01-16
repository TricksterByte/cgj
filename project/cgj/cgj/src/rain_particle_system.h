#pragma once
#ifndef __RAIN_PARTICLE_SYSTEM_H__
#define __RAIN_PARTICLE_SYSTEM_H__

#include "particle_system.h"

class RainParticleSystem : public ParticleSystem {
public:
	RainParticleSystem(int max, bool hasTex);
	~RainParticleSystem();
private:
	virtual Particle* createParticle();
};

#endif
