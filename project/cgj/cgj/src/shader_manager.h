#pragma once
#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "i_manager.h"
#include "shader.h"

#include <string>
#include <map>

class ShaderManager : public IManager<ShaderManager> {

	friend IManager<ShaderManager>;
	std::map<std::string, ShaderProgram*>* shaderMap;
	ShaderManager();
	~ShaderManager();

public:
	void add(std::string name, ShaderProgram* shader);
	void remove(std::string name);
	ShaderProgram* get(std::string name);
};

#endif