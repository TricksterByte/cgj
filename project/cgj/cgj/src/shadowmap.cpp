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
	if (on) {
		on = false;
		lightIntensity = 0.f;
	}
	else {
		on = true;
		lightIntensity = lastIntensity;
	}
}

void ShadowMap::update(float elapsed) {
	if (on) {
		switch (var) {
		case INC:
			lightIntensity += step * elapsed;
			if (lightIntensity > 0.40f) var = DEC;
			break;

		case DEC:
			lightIntensity -= step * elapsed;
			if (lightIntensity < 0.20f) var = INC;
			break;
		}

		lastIntensity = lightIntensity;

		lightPos = vec3(random(-1.34f, -1.26f), random(0.46f, 0.54f), random(-0.64f, -0.56f));
	}
}