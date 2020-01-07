#pragma once
#ifndef __DYNAMIC_MESH_H__
#define __DYNAMIC_MESH_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DynamicMesh {
public:
	static const GLuint VERTEX = 0;
	static const GLuint COLOR = 3;
	static const GLuint SIZE = 4;

	int maxParticles;
};

#endif