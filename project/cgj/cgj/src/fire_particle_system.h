#pragma once
#ifndef __FIRE_PARTICLE_SYSTEM_H__
#define __FIRE_PARTICLE_SYSTEM_H__

#include "particle_system.h"

class FireParticleSystem : public ParticleSystem {
	bool active = true;
public:
	FireParticleSystem(int max, bool hasTex);
	~FireParticleSystem();
	void toggle();
private:
	virtual Particle* createParticle();
};

#endif
