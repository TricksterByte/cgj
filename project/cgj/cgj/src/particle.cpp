#include "particle.h"
#include "math_helpers.h"

Particle::Particle(const vec3& p, const vec3& v, const vec3& c, float sz, bool g, float time) :
	position(p),
	velocity(v),
	color(c),
	size(sz),
	gravity(g),
	ttl(time) {}

bool Particle::update(float dt) {
	if (gravity) 
		velocity.y += -15 * dt;
	position += velocity * dt;

	elapsed += dt;
	return elapsed < ttl;
}