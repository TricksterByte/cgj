#pragma once

#include <SOIL.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "texture.h"
#include "sampler.h"

///////////////////////////////////////////////// TextureInfo

TextureInfo::TextureInfo(GLenum u, GLenum i, const std::string& uni, Texture* tex, Sampler* splr) :
		unit(u),
		index(i),
		uniform(uni),
		texture(tex),
		sampler(splr) {}

void TextureInfo::updateShader(ShaderProgram* shader) {
	glActiveTexture(unit);
	texture->bind();
	glUniform1i(shader->uniforms[uniform].index, index);
	if (sampler) sampler->bind(index);
}

///////////////////////////////////////////////// Texture

Texture::Texture() : id(-1) {}

Texture::~Texture() {}

///////////////////////////////////////////////// Texture1D

void Texture1D::bind() {
	glBindTexture(GL_TEXTURE_1D, id);
}

void Texture1D::unbind() {
	glBindTexture(GL_TEXTURE_1D, 0);
}

void Texture1D::createRandomNoise(const unsigned short length) {
	unsigned char* data = new unsigned char[length];
	
	for (int i = 0; i < length; ++i) {
		double noise = rand() / (double)RAND_MAX;
		data[i] = static_cast<unsigned char>(255 * noise);
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_1D, id);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, length, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	delete[] data;
}

/*
void Texture1D::createPerlinNoise(const unsigned short length, const double scale, const double alpha, const double beta, const int harmonics) {
	unsigned char* data = new unsigned char[length];
	int idx = 0;
	double step = 1.0 / length;

	for (double x = 0.0; x < 1.0; x += step) {
		double noise = PerlinNoise::instance()->noise1D(scale * x, alpha, beta, harmonics);
		data[id++] = static_cast<unsigned char>(128 * noise + 128);
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_1D, id);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, length, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	delete[] data;
}
*/

///////////////////////////////////////////////// Texture2D

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::load(const std::string& filename) {
	int width, height, channels;
	std::cout << "Loading image file " << filename << "... ";
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == nullptr) {
		std::cout << "error." << std::endl;
		exit(EXIT_FAILURE);
	} else {
		std::cout << "ok." << std::endl;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

/** /
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
/** /
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
/**/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
/** /
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
/** /
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
/**/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
/**/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
}

void Texture2D::loadMipMap(int levels, const std::string& prefix, const std::string& suffix, const std::string& filename) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	for (int i = 0; i < levels; ++i) {
		std::stringstream ss;
		ss << prefix << i << suffix;
		std::string filename = ss.str();

		int width, height;
		std::cout << "Loading mipmap file " << filename << "... ";
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		if (image == nullptr) {
			std::cout << "error." << std::endl;
			exit(EXIT_FAILURE);
		} else {
			std::cout << "ok." << std::endl;
		}

		glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::createRandomNoise(const unsigned short side) {
	unsigned int size = side * side;
	unsigned char* data = new unsigned char[size];
	int idx = 0;

	for (int ix = 0; ix < side; ++ix) {
		for (int iy = 0; iy < side; ++iy) {
			double noise = rand() / (double)RAND_MAX;
			data[idx++] = static_cast<unsigned char>(255 * noise);
		}
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, side, side, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	delete[] data;
}

/*
void Texture2D::createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double alpha, const double beta, const int harmoncis) {
	srand((unsigned int)time(0));
	const unsigned int size = side * side;
	float* data = new float[size];
	int idx = 0;
	double step = 1.0 / (double)side;

	for (double x = 0.0; x < 1.0; x += step) {
		for (double y = 0.0; y < 1.0; y += step) {
			double noise = PerlinNoise::instance()->noise2D(scalex * x, scaley * y, alpha, harmonics);
			data[idx++] = static_cast<float>(noise);
		}
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_1D, 0, GL_R16F, side, side, 0, GL_RED, GL_FLOAT, data);

	delete[] data;
}
*/

///////////////////////////////////////////////// Texture3D

void Texture3D::bind() {
	glBindTexture(GL_TEXTURE_3D, id);
}

void Texture3D::unbind() {
	glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::createRandomNoise(const unsigned short side) {
	unsigned int size = side * side * side;
	unsigned char* data = new unsigned char[size];
	int idx = 0;

	for (int ix = 0; ix < side; ++ix) {
		for (int iy = 0; iy < side; ++iy) {
			for (int iz = 0; iz < side; ++iz) {
				double noise = rand() / (double)RAND_MAX;
				data[idx++] = static_cast<unsigned char>(255 * noise);
			}
		}
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, side, side, side, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	delete[] data;
}

///////////////////////////////////////////////// TextureCubeMap

void TextureCubeMap::bind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void TextureCubeMap::unbind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

const std::string CUBEMAP_SUFFIXES[6] = {
	"right", "left",
	"top", "bottom",
	"front", "back"
};

const GLenum CUBEMAP_TEXTURES[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

void TextureCubeMap::loadCubeMap(const std::string& prefix, const std::string& suffix) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i) {
		std::stringstream ss;
		ss << prefix << CUBEMAP_SUFFIXES[i] << suffix;
		std::string filename = ss.str();

		int width, height;
		std::cout << "Loading cubemap file " << filename << "... ";
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		if (image == nullptr) {
			std::cout << "error." << std::endl;
			exit(EXIT_FAILURE);
		} else {
			std::cout << "ok." << std::endl;
		}
		glTexImage2D(CUBEMAP_TEXTURES[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

/*
void TextureCubeMap::saveCubeMap(int index, int mip) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
}


void TextureCubeMap::renderCube() {
	bind();
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  1.0f,  -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f,  -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f,  -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f,  1.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f, 1.0f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		}
	}
}
*/

///////////////////////////////////////////////// ParticleTexture

ParticleTexture::ParticleTexture(int r, bool add) : rows(r), additive(add) {}

void ParticleTexture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

void ParticleTexture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ParticleTexture::load(const std::string& filename) {
	int width, height, channels;
	std::cout << "Loading image file " << filename << "... ";
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == nullptr) {
		std::cout << "error." << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "ok." << std::endl;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
}

int ParticleTexture::getRows() {
	return rows;
}

bool ParticleTexture::getAdditive() {
	return additive;
}
