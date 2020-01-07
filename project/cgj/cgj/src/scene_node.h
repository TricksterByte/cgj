#pragma once
#ifndef __SCENE_NODE__
#define __SCENE_NODE__

#include "matrices.h"
#include "quaternion.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "i_scene_node_callback.h"

#include <vector>

typedef struct {
	vec3 t;
	qtrn r;
	vec3 s;
} Transform;

typedef vec3 Color;

class SceneNode {
public:
	ShaderProgram* shaderProgram;
	SceneNode* parent;
	Mesh* mesh;
	ISceneNodeCallback* callback;

	Color color[2];
	float steps;
	std::vector<TextureInfo*> textures;

	Transform currTransform;

	mat4 ModelMatrix;
	mat4 DrawMatrix;

	std::vector<SceneNode*> children;

	SceneNode();
	virtual ~SceneNode();

	SceneNode* createNode();
	void addNode(SceneNode* node);

	void setColor(const vec3& color1, const vec3& color2);
	void setSteps(const float steps);
	void setMesh(Mesh* m);
	void addTextureInfo(TextureInfo* tex);
	void setShaderProgram(ShaderProgram* program);
	void setCallback(ISceneNodeCallback* c);

	void setModelMatrix(const vec3& t, const qtrn& r, const vec3& s);
	void setModelMatrix(const mat4 mat);

	void move(const vec3& axis, float delta);
	void rotate(float angle, const vec3& axis);
	void scale(const vec3& s);
	virtual void draw(Camera* camera);
	void update();
	void release();
};

#endif