#pragma once

#ifndef __MESH_H__
#define __MESH_H__

#include <vector>

#include "vectors.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef vec3 Vertex;
typedef vec2 TexCoord;
typedef vec3 Normal;

class Mesh {
public:
	GLuint VaoId = 0;

	static const GLuint VERTEX = 0;
	static const GLuint TEXCOORD = 1;
	static const GLuint NORMAL = 2;

	std::vector<Vertex> Vertices;
	std::vector<TexCoord> Texs;
	std::vector<Normal> Normals;
	bool HasTexs, HasNormals;
	Mesh(std::string& filename);
	Mesh();
	~Mesh();
	void freeMeshData();
	void processMesh();
	void draw();

	GLuint VboVertices, VboTexcoords, VboNormals;

private:
	std::vector<Vertex> VerticesData;
	std::vector<TexCoord> TexsData;
	std::vector<Normal> NormalsData;
	std::vector<unsigned int> VerticesIdx, TexsIdx, NormalsIdx;
	void loadMeshData(std::string& filename);
	void parseLine(std::stringstream& sin);
	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFaces(std::stringstream& sin);
	void createBufferObjects();
	void destroyBufferObjects();
};

#endif