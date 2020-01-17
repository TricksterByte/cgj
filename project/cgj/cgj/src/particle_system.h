#pragma once
#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "math_helpers.h"
#include "particle.h"
#include "scene_node.h"
#include "i_updatable.h"

typedef vec3 Vertex;
typedef vec3 Color;
typedef float Size;
typedef vec4 TexOffset;
typedef float Blend;

class ParticleSystem : public SceneNode, public IUpdatable {
protected:
	int maxParticles;
	float timeSinceLast = 0;
	bool hasTexture;
	bool active = true;

	std::vector<Particle*> particles;

	GLuint VaoId;
	GLuint VboVertices;
	GLuint VboColors;
	GLuint VboSizes;
	GLuint VboTexOffsets;
	GLuint VboBlends;
	std::vector<Vertex> Vertices;
	std::vector<Color> Colors;
	std::vector<Size> Sizes;
	std::vector<TexOffset> TexOffsets;
	std::vector<Blend> Blends;
public:
	static const GLuint VERTEX = 0;
	static const GLuint COLOR = 1;
	static const GLuint SIZE = 2;
	static const GLuint TEXOFFSET = 3;
	static const GLuint BLEND = 4;

	ParticleSystem(int max, bool hasTex);
	~ParticleSystem();

	virtual void update(float dt);
	virtual void draw(Camera* cam);
	virtual void toggle();
private:
	void createBufferObjects();
	void destroyBufferObjects();
	virtual Particle* createParticle();
};

#endif
