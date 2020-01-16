#pragma once
#ifndef __GL_PROGRAM_H__
#define __GL_PROGRAM_H__

#include <vector>

#include "vectors.h"
#include "i_updatable.h"
#include "shadowmap.h"
#include "water.h"
#include "fire_particle_system.h"
#include "rain_particle_system.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class MouseState { STOPPED, STARTING, ROTATING };

class GLProgram {
private:
	const GLuint UBO_BP = 0;
	std::vector<IUpdatable*> updatables;

	void createToonShader();
	void createSilhouetteShader();
	void createShadowShader();
	void createWaterShader();
	void createSkyboxShader();
	void createRainParticleSystemShader();
	void createFireParticleSystemShader();
	SceneNode* createPoplarTree(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s);
	SceneNode* createFirTree(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s);
	SceneNode* createTent(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s);
	SceneNode* createCampfire(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s);

public:
	GLfloat Offset = 0.02f;
	GLfloat OffsetDelta = 0.001f;

	MouseState state = MouseState::STOPPED;
	float elapsed_time;
	vec2 lastPosition;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float yDistance = 0.0f;

	ShadowMap* map;
	Water* water;
	FireParticleSystem* fireParticleSystem;
	RainParticleSystem* rainParticleSystem;

	GLProgram();
	~GLProgram();

	void createMeshes();
	void createTextures();
	void createShaders();
	void createSceneGraph();
	void drawSceneGraph();
	void setViewProjectionMatrix();
	void update(float elaspsed);
};


#endif