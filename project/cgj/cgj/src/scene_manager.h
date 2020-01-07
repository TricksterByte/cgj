#pragma once
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "i_manager.h"
#include "scene_graph.h"

#include <map>

class SceneManager : public IManager<SceneManager> {

	friend IManager<SceneManager>;
	std::map<std::string, SceneGraph*>* sceneMap;
	SceneManager();
	~SceneManager();

public:
	void add(std::string name, SceneGraph* scene);
	void remove(std::string name);
	SceneGraph* get(std::string name);
};

#endif