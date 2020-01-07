#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vectors.h"
#include "matrices.h"
#include "matrix_factory.h"
#include "quaternion.h"

class Camera {
public:
	const float sensitivity = 0.5f;

	qtrn current;
	mat4 rotation;
	mat4 translation;
	mat4 projection;

public:
	GLuint vbo;

	Camera(const GLuint ubo);
	~Camera();

	void bind();

	//void move(Movement mov, double time);
	//void lookAt(float offsetX, float offsetY);

	void changeRotation(float offsetX, float offsetY, float offsetZ);
	const mat4 getRotation() const;
	void changeTranslation(vec3 diff);
	const mat4 getTranslation() const;

	void setProjection(const mat4& proj);
	const mat4 getProjection() const;
	const mat4 getView() const;
	const float getSensitivity() const;
};

#endif