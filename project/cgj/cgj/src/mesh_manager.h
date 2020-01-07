#pragma once
#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__

#include "i_manager.h"
#include "mesh.h"

#include <string>
#include <map>

class MeshManager : public IManager<MeshManager> {

	friend IManager<MeshManager>;
	std::map<std::string, Mesh*>* meshMap;
	MeshManager();
	~MeshManager();

public:
	void add(std::string name, Mesh* mesh);
	void remove(std::string name);
	Mesh* get(std::string name);
};

#endif