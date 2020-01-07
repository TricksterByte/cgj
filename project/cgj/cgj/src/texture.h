#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "shader.h"
#include "sampler.h"

class Texture;
class Texture1D;
class Texture2D;
class Texture3D;
class TextureCubeMap;
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
	GLenum unit;                // GL_TEXTURE1
	GLuint index;               // sampler index in shader
	std::string uniform;        // uniform name in shader
	Texture* texture = nullptr; // Texture (engine object)
	Sampler* sampler = nullptr; // Sampler (engine object)

	TextureInfo(GLenum textureUnit, GLuint index, const std::string& uniform, Texture* t, Sampler* s);
	void updateShader(ShaderProgram* shader);
};

///////////////////////////////////////////////// Texture1D

class Texture1D : Texture {
public:
	void bind();
	void unbind();
	void createRandomNoise(const unsigned short length);
	void createPerlinNoise(const unsigned short length, const double scale, const double alpha, const double beta, const int harmonics);
};

///////////////////////////////////////////////// Texture2D

class Texture2D : Texture {
public:
	void bind();
	void unbind();
	void load(const std::string& filename);
	void loadMipMap(int levels, const std::string& prefix, const std::string& suffix, const std::string& filename);
	void createRandomNoise(const unsigned short side);
	void createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double alpha, const double beta, const int harmonics);
};

/////////////////////////////////////////////////

class Texture3D : Texture {
public:
	void bind();
	void unbind();
	void createRandomNoise(const unsigned short side);
};

///////////////////////////////////////////////// TextureCubeMap

class TextureCubeMap : Texture {
public:
	void bind();
	void unbind();
	void loadCubeMap(const std::string& prefix, const std::string& suffix);
	void saveCubeMap(int index, int mip);
	void renderCube();
};

#endif