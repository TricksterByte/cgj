#include "mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

Mesh::Mesh(std::string& filename) {
	loadMeshData(filename);
	HasTexs = (TexsIdx.size() > 0);
	HasNormals = (NormalsIdx.size() > 0);

	processMesh();
	createBufferObjects();
	freeMeshData();
}

Mesh::Mesh() {
	HasTexs = HasNormals = false;
	VboVertices = VboTexcoords = VboNormals = -1;
}

Mesh::~Mesh() {
	destroyBufferObjects();
}

void Mesh::freeMeshData() {
	VerticesData.clear();
	TexsData.clear();
	NormalsData.clear();
	VerticesIdx.clear();
	TexsIdx.clear();
	NormalsIdx.clear();
}

void Mesh::processMesh() {
	for (unsigned int i = 0; i < VerticesIdx.size(); i++) {
		unsigned int vi = VerticesIdx[i];
		Vertex v = VerticesData[vi - 1];
		Vertices.push_back(v);

		if (HasTexs) {
			unsigned int ti = TexsIdx[i];
			TexCoord t = TexsData[ti - 1];
			Texs.push_back(t);
		}

		if (HasNormals) {
			unsigned int ni = NormalsIdx[i];
			Normal n = NormalsData[ni - 1];
			Normals.push_back(n);
		}
	}
}

void Mesh::loadMeshData(std::string& filename) {
	std::ifstream file = std::ifstream(filename);

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream sline(line);
		parseLine(sline);
	}
	file.close();
}

void Mesh::parseLine(std::stringstream& sin) {
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFaces(sin);
}

void Mesh::parseVertex(std::stringstream& sin) {
	Vertex v;
	sin >> v;
	VerticesData.push_back(v);
}

void Mesh::parseTexcoord(std::stringstream& sin) {
	TexCoord t;
	sin >> t;
	TexsData.push_back(t);
}

void Mesh::parseNormal(std::stringstream& sin) {
	Normal n;
	sin >> n;
	NormalsData.push_back(n);
}

void Mesh::parseFaces(std::stringstream& sin) {
	std::string token;

	if (NormalsData.empty() && TexsData.empty()) {
		for (int i = 0; i < 3; i++) {
			sin >> token;
			VerticesIdx.push_back(std::stoi(token));
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			std::getline(sin, token, '/');
			if (token.size() > 0) VerticesIdx.push_back(std::stoi(token));
			std::getline(sin, token, '/');
			if (token.size() > 0) TexsIdx.push_back(std::stoi(token));
			std::getline(sin, token, ' ');
			if (token.size() > 0) NormalsIdx.push_back(std::stoi(token));
		}
	}
}

void Mesh::draw() {
	glBindVertexArray(VaoId);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());
	glBindVertexArray(0);
}

void Mesh::createBufferObjects() {
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTEX);
		glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		if (HasTexs) {
			glGenBuffers(1, &VboTexcoords);
			glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
			glBufferData(GL_ARRAY_BUFFER, Texs.size() * sizeof(TexCoord), &Texs[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORD);
			glVertexAttribPointer(TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(TexCoord), 0);
		}

		if (HasNormals) {
			glGenBuffers(1, &VboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
			glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMAL);
			glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VboVertices);
		glDeleteBuffers(1, &VboTexcoords);
		glDeleteBuffers(1, &VboNormals);
	}
}

void Mesh::destroyBufferObjects() {
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTEX);
	glDisableVertexAttribArray(TEXCOORD);
	glDisableVertexAttribArray(NORMAL);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}