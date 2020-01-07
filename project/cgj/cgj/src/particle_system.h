#pragma once
#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "mesh.h"
#include "particle.h"
#include "scene_node.h"

class ParticleSystem : public SceneNode {
public:
	int maxParticles;
	float timeSinceLast = 0;

	ParticleSystem(int max);
	~ParticleSystem();

	void update(float dt);
	Particle* createParticle();
	void draw(Camera* cam);

	std::vector<Particle*> particles;

private:
	GLuint VaoId;
	std::vector<Vertex> Vertices;

	void createBufferObjects();
	void destroyBufferObjects();

	GLuint VboVertices;
};

#endif