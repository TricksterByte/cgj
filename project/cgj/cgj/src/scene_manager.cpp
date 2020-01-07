#include "scene_manager.h"

SceneManager::SceneManager() {
	sceneMap = new std::map<std::string, SceneGraph*>();
}

SceneManager::~SceneManager() {
	for (auto it = sceneMap->begin(); it != sceneMap->end(); ++it) {
		delete(it->second);
	}
	sceneMap->clear();
	delete sceneMap;
}

void SceneManager::add(std::string name, SceneGraph* scene) {
	(*sceneMap)[name] = scene;
}

void SceneManager::remove(std::string name) {
	sceneMap->erase(name);
}

SceneGraph* SceneManager::get(std::string name) {
	auto found = sceneMap->find(name);
	
	if (found != sceneMap->end())
		return found->second;

	return nullptr;
}