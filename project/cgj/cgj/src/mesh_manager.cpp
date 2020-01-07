#include "mesh_manager.h"

MeshManager::MeshManager() {
	meshMap = new std::map<std::string, Mesh*>();
}

MeshManager::~MeshManager() {
	for (auto it = meshMap->begin(); it != meshMap->end(); ++it) {
		delete(it->second);
	}
	meshMap->clear();
	delete meshMap;
}

void MeshManager::add(std::string name, Mesh* mesh) {
	(*meshMap)[name] = mesh;
}

void MeshManager::remove(std::string name) {
	meshMap->erase(name);
}

Mesh* MeshManager::get(std::string name) {
	auto found = meshMap->find(name);

	if (found != meshMap->end())
		return found->second;

	return nullptr;
}
