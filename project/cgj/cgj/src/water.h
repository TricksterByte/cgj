#pragma once
#ifndef __WATER_H__
#define __WATER_H__

#include "scene_node.h"
#include "water_fbos.h"

class Water : public SceneNode {
private:
	WaterFramebuffers* waterFbos;
	float moveFactor = 0.f;

	const float WAVE_SPEED = 0.025f;

public:
	Water();
	~Water();

	void setFbos(WaterFramebuffers* fbos);
	WaterFramebuffers* getFbos();
	void draw(Camera* cam);
	void update(float elapsed);
};

#endif