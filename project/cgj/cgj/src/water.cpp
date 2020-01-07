#include "water.h"

Water::Water() : SceneNode() {
	waterFbos = nullptr;
}

Water::~Water() {
	if (waterFbos)
		delete(waterFbos);
}

void Water::setFbos(WaterFramebuffers* fbos) {
	waterFbos = fbos;
}

WaterFramebuffers* Water::getFbos() {
	return waterFbos;
}

void Water::draw(Camera* cam) {
	if (mesh != nullptr) {
		shaderProgram->bind();

		if (shaderProgram->isUniform("ModelMatrix"))
			glUniformMatrix4fv(shaderProgram->uniforms["ModelMatrix"].index, 1, GL_TRUE, DrawMatrix.data);

		if (shaderProgram->isUniform("NormalMatrix")) {
			mat3 NormalMatrix = transpose(inverse(MatrixFactory::createMat3FromMat4(cam->getView() * DrawMatrix)));
			glUniformMatrix3fv(shaderProgram->uniforms["NormalMatrix"].index, 1, GL_TRUE, NormalMatrix.data);
		}

		if (shaderProgram->isUniform("MoveFactor")) {
			glUniform1f(shaderProgram->uniforms["MoveFactor"].index, moveFactor);
		}

		if (shaderProgram->isUniform("WaterDuDv")) {
			glActiveTexture(GL_TEXTURE2);
			textures[0]->texture->bind();
			glUniform1i(shaderProgram->uniforms["WaterDuDv"].index, textures[0]->index);
		}
		
		if (shaderProgram->isUniform("Reflection")) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, waterFbos->reflectionId);
			glUniform1i(shaderProgram->uniforms["Reflection"].index, 0);
		}

		if (callback)
			callback->beforeDraw(this);

		mesh->draw();

		if (callback)
			callback->afterDraw(this);

		shaderProgram->unbind();
	}
}

void Water::update(float elapsed) {
	moveFactor += WAVE_SPEED * elapsed;
	if (moveFactor >= 1.0f) {
		moveFactor -= 1.0f;
	}
}