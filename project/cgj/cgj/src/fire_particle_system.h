#pragma once
#ifndef __FIRE_PARTICLE_SYSTEM_H__
#define __FIRE_PARTICLE_SYSTEM_H__

#include "particle_system.h"

class FireParticleSystem : public ParticleSystem {
public:
	FireParticleSystem(int max, bool hasTex);
	~FireParticleSystem();
private:
	virtual Particle* createParticle();
};

#endif
