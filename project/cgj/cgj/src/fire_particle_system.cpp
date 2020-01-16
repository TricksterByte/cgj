#include "fire_particle_system.h"

FireParticleSystem::FireParticleSystem(int max, bool hasTexture) : ParticleSystem(max, hasTexture) {}

FireParticleSystem::~FireParticleSystem() {}

void FireParticleSystem::toggle() {
	active = !active;
}

Particle* FireParticleSystem::createParticle() {
	float x = random(-.05f, .05f);
	float y = 0.002f;
	float z = random(-.05f, .05f);
	vec3 position = currTransform.t + vec3(x, y, z);

	float velx = 0.0f;
	float vely = random(0.0f, 0.5f);
	float velz = 0.0f;
	vec3 velocity = { velx, vely, velz };

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	vec3 color = { r, g, b };

	float ge = 0.0f;
	float ttl = random(0.1f, 1.0f);
	float size = ((float)rand()) / ((float)RAND_MAX * 18);

	return new Particle(position, velocity, color, ge, ttl, size);
}
