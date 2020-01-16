#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "shader.h"

class Texture;
class Texture2D;
class TextureCubeMap;
class ParticleTexture;
struct TextureInfo;

///////////////////////////////////////////////// Texture

class Texture {
protected:
	GLuint id;

public:
	Texture();
	~Texture();
	virtual void bind() = 0;
	virtual void unbind() = 0;
};

///////////////////////////////////////////////// TextureInfo

struct TextureInfo {
	GLenum unit;               
	GLuint index;              
	std::string uniform;       
	Texture* texture = nullptr;

	TextureInfo(GLenum textureUnit, GLuint index, const std::string& uniform, Texture* t);
	void updateShader(ShaderProgram* shader);
};

///////////////////////////////////////////////// Texture2D

class Texture2D : Texture {
public:
	void bind();
	void unbind();
	void load(const std::string& filename);
};

///////////////////////////////////////////////// TextureCubeMap

class TextureCubeMap : Texture {
public:
	void bind();
	void unbind();
	void loadCubeMap(const std::string& prefix, const std::string& suffix);
};

///////////////////////////////////////////////// ParticleTexture

class ParticleTexture : Texture {
	int rows = 1;
	bool additive;
public:
	ParticleTexture(int r, bool add);
	void bind();
	void unbind();
	void load(const std::string& filename);
	int getRows();
	bool getAdditive();
};

#endif
