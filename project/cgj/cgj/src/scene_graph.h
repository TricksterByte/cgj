#pragma once
#ifndef __SCENE_GRAPH__
#define __SCENE_GRAPH__

#include "scene_node.h"
#include "camera.h"

class SceneGraph {
public:
	SceneGraph();
	~SceneGraph();

	SceneNode* root;
	SceneNode* getRoot();
	SceneNode* createNode();

	Camera* camera;
	Camera* getCamera();
	void setCamera(Camera* camera);
	
	void draw();;
};

#endif