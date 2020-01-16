#include "particle.h"

Particle::Particle(const vec3& pos, const vec3& vel, const vec3& col, float ge, float life, float sz) :
	position(pos),
	velocity(vel),
	color(col),
	gravityEffect(ge),
	lifeSpan(life),
	size(sz) {}

bool Particle::update(float dt, int textureRows) {
	velocity.y += -15 * gravityEffect * dt;
	position += velocity * dt;

	if (textureRows > 0) {
		updateTexCoordInfo(textureRows);
	}

	elapsedTime += dt;
	return elapsedTime < lifeSpan;
}

vec3 Particle::getPosition() {
	return position;
}

vec3 Particle::getColor() {
	return color;
}

float Particle::getSize() {
	return size;
}

vec2 Particle::getTexOffset1() {
	return texOffset1;
}

vec2 Particle::getTexOffset2() {
	return texOffset2;
}

float Particle::getBlend() {
	return blend;
}

void Particle::updateTexCoordInfo(int textureRows) {
	float lifeFactor = elapsedTime / lifeSpan;
	int stageCount = textureRows * textureRows;
	float atlasProgression = lifeFactor * stageCount;

	float index1f;
	blend = modf(atlasProgression, &index1f);
	float index2f = (index1f < stageCount - 1 ? index1f + 1 : index1f);
	setTextureOffset(texOffset1, (int)index1f, textureRows);
	setTextureOffset(texOffset2, (int)index2f, textureRows);
}

void Particle::setTextureOffset(vec2& offset, int index, int rows) {
	int column = index % rows;
	int row = index / rows;
	offset.x = (float)column / rows;
	offset.y = (float)row / rows;
}
