#pragma once

#include <SOIL.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "texture.h"

///////////////////////////////////////////////// TextureInfo

TextureInfo::TextureInfo(GLenum u, GLenum i, const std::string& uni, Texture* tex) :
		unit(u),
		index(i),
		uniform(uni),
		texture(tex) {}

void TextureInfo::updateShader(ShaderProgram* shader) {
	glActiveTexture(unit);
	texture->bind();
	glUniform1i(shader->uniforms[uniform].index, index);
}

///////////////////////////////////////////////// Texture

Texture::Texture() : id(-1) {}

Texture::~Texture() {}

///////////////////////////////////////////////// Texture2D

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::load(const std::string& filename) {
	int width, height, channels;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == nullptr) {
		exit(EXIT_FAILURE);
	} else {
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
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
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		if (image == nullptr) {
			exit(EXIT_FAILURE);
		} else {
		}
		glTexImage2D(CUBEMAP_TEXTURES[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

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
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == nullptr) {
		exit(EXIT_FAILURE);
	}
	else {
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
