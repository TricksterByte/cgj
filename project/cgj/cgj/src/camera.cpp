#include "camera.h"
#include "vectors.h"
#include "math_helpers.h"
#include "matrix_factory.h"

Camera::Camera(const GLuint ubo) {
	current = qtrn();
	projection = MatrixFactory::createIdentityMat4();
	rotation = MatrixFactory::createIdentityMat4();
	translation = MatrixFactory::createTranslationMat4(vec3(0, 0, -5));

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_UNIFORM_BUFFER, vbo);
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, ubo, vbo);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Camera::~Camera() {
	glDeleteBuffers(1, &vbo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::bind() {
	float view[16];
	convertToColumnMajor(getView(), view);
	float proj[16];
	convertToColumnMajor(getProjection(), proj);

	glBindBuffer(GL_UNIFORM_BUFFER, vbo);
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 16, view);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 16, sizeof(float) * 16, proj);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::changeRotation(float offsetX, float offsetY, float offsetZ) {
	qtrn qtrnX = qtrn(offsetX, AXIS_Y);
	qtrn qtrnY = qtrn(offsetY, AXIS_X);
	qtrn qtrnZ = qtrn(offsetZ, AXIS_Z);
	current = qtrnZ * qtrnY * qtrnX * current;
	rotation = MatrixFactory::createMat4FromQtrn(current);
}

const mat4 Camera::getRotation() const {
	return rotation;
}

void Camera::changeTranslation(vec3 diff) {
	translation = MatrixFactory::createTranslationMat4(diff) * translation;
}

const mat4 Camera::getTranslation() const {
	return translation;
}

void Camera::setProjection(const mat4& proj) {
	projection = proj;
}

const mat4 Camera::getView() const {
	return translation * rotation;
}

const mat4 Camera::getProjection() const {
	return projection;
}

const float Camera::getSensitivity() const {
	return sensitivity;
}