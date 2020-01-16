#include "shadowmap.h"
#include "matrix_factory.h"
#include "math_helpers.h"

ShadowMap::ShadowMap(const int shadowW, const int shadowH, const int displayW, const int displayH, const vec3& light, const float n, const float f) : 
	SHADOW_WIDTH(shadowW), SHADOW_HEIGHT(shadowH),
	DISPLAY_WIDTH(displayW), DISPLAY_HEIGHT(displayH),
	near(n), far(f) {

	lightPos = light;

	shadowFbos = nullptr;
	calcMatrices();
}

ShadowMap::~ShadowMap() {
	if (shadowFbos)
		delete(shadowFbos);
}

void ShadowMap::setFbos(ShadowsFramebuffers* fbos) {
	shadowFbos = fbos;
}

ShadowsFramebuffers* ShadowMap::getFbos() {
	return shadowFbos;
}

void ShadowMap::calcMatrices() {
	mat4 proj = MatrixFactory::createPerspectiveProjectionMatrix(degreesToRadians(90.f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near, far);
	shadowMats[0] = proj * MatrixFactory::createViewMatrix(lightPos, lightPos + vec3(1.f, 0.f, 0.f), vec3(0.f, -1.f, 0.f));
	shadowMats[1] = proj * MatrixFactory::createViewMatrix(lightPos, lightPos + vec3(-1.f, 0.f, 0.f), vec3(0.f, -1.f, 0.f));
	shadowMats[2] = proj * MatrixFactory::createViewMatrix(lightPos, lightPos + vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 1.f));
	shadowMats[3] = proj * MatrixFactory::createViewMatrix(lightPos, lightPos + vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, -1.f));
	shadowMats[4] = proj * MatrixFactory::createViewMatrix(lightPos, lightPos + vec3(0.f, 0.f, 1.f), vec3(0.f, -1.f, 0.f));
	shadowMats[5] = proj * MatrixFactory::createViewMatrix(lightPos, lightPos + vec3(0.f, 0.f, -1.f), vec3(0.f, -1.f, 0.f));
}

void ShadowMap::lightIntensityToggle() {
	lightIntensity = (lightIntensity == 0.4f? 0.0f : 0.4f);
}