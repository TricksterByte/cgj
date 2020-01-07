#include "shader_manager.h"

ShaderManager::ShaderManager() {
	shaderMap = new std::map<std::string, ShaderProgram*>();
}

ShaderManager::~ShaderManager() {
	for (auto it = shaderMap->begin(); it != shaderMap->end(); ++it) {
		delete(it->second);
	}
	shaderMap->clear();
	delete shaderMap;
}

void ShaderManager::add(std::string name, ShaderProgram* shader) {
	(*shaderMap)[name] = shader;
}

void ShaderManager::remove(std::string name) {
	shaderMap->erase(name);
}

ShaderProgram* ShaderManager::get(std::string name) {
	auto found = shaderMap->find(name);

	if (found != shaderMap->end())
		return found->second;

	return nullptr;
}