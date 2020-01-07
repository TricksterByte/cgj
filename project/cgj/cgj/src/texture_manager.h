#pragma once
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include "i_manager.h"
#include "texture.h"

#include <string>
#include <map>

class TextureManager : public IManager<TextureManager> {

	friend IManager<TextureManager>;
	std::map<std::string, Texture*>* texMap;
	TextureManager();
	~TextureManager();

public:
	void add(std::string name, Texture* mesh);
	void remove(std::string name);
	Texture* get(std::string name);
};

#endif