#include "rain_particle_system.h"

RainParticleSystem::RainParticleSystem(int max, bool hasTexture) : ParticleSystem(max, hasTexture) {
	active = false;
}

RainParticleSystem::~RainParticleSystem() {}

Particle* RainParticleSystem::createParticle() {
	float x = random(-8.f, 8.f);
	float y = 0.0f;
	float z = random(-8.f, 8.f);
	vec3 position = currTransform.t + vec3(x, y, z);

	float velx = 0.0f;
	float vely = 0.0f;
	float velz = 0.0f;
	vec3 velocity = { velx, vely, velz };

	float r = 0.5f;
	float g = 0.5f;
	float b = 1.0f;
	vec3 color = { r, g, b };

	float ge = 10.0f;
	float ttl = random(0.27f, 0.32f);
	float size = random(0.3f, 1.8f);

	return new Particle(position, velocity, color, ge, ttl, size);
}
