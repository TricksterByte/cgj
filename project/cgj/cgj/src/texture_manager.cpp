#include "texture_manager.h"

TextureManager::TextureManager() {
	texMap = new std::map<std::string, Texture*>();
}

TextureManager::~TextureManager() {
	for (auto it = texMap->begin(); it != texMap->end(); ++it) {
		delete(it->second);
	}
	texMap->clear();
	delete texMap;
}

void TextureManager::add(std::string name, Texture* mesh) {
	(*texMap)[name] = mesh;
}

void TextureManager::remove(std::string name) {
	texMap->erase(name);
}

Texture* TextureManager::get(std::string name) {
	auto found = texMap->find(name);

	if (found != texMap->end())
		return found->second;

	return nullptr;
}
