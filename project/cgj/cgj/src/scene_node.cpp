#include "camera.h"
#include "scene_node.h"
#include "matrix_factory.h"

SceneNode::SceneNode() {
	ModelMatrix = MatrixFactory::createIdentityMat4();
	DrawMatrix = MatrixFactory::createIdentityMat4();
}

SceneNode::~SceneNode() {
	release();

	if (callback)
		delete(callback);

	for (auto& t : textures) {
		delete(t);
	}
	textures.clear();
}

SceneNode* SceneNode::createNode() {
	SceneNode* childNode = new SceneNode();

	childNode->setShaderProgram(shaderProgram);
	childNode->parent = this;
	childNode->DrawMatrix = DrawMatrix * childNode->ModelMatrix;

	children.push_back(childNode);

	return children.back();
}

void SceneNode::addNode(SceneNode* node) {
	node->parent = this;
	node->DrawMatrix = DrawMatrix * node->ModelMatrix;
	
	children.push_back(node);
}

void SceneNode::setColor(const vec3& c1, const vec3& c2) {
	color[0] = c1;
	color[1] = c2;
}

void SceneNode::setSteps(const float s) {
	steps = s;
}

void SceneNode::setMesh(Mesh* m) {
	mesh = m;
}

void SceneNode::addTextureInfo(TextureInfo* tex) {
	textures.push_back(tex);
}

void SceneNode::setShaderProgram(ShaderProgram* program) {
	shaderProgram = program;
}

void SceneNode::setCallback(ISceneNodeCallback* c) {
	callback = c;
}

void SceneNode::setModelMatrix(const vec3& t, const qtrn& r, const vec3& s) {
	ModelMatrix = MatrixFactory::createTranslationMat4(t) * MatrixFactory::createMat4FromQtrn(r) * MatrixFactory::createScaleMat4(s);
	
	currTransform = { t, r, s };

	if (parent == nullptr) {
		DrawMatrix = ModelMatrix;
	}
	else {
		DrawMatrix = parent->DrawMatrix * ModelMatrix;
	}

	update();
}

void SceneNode::setModelMatrix(mat4 mat) {
	ModelMatrix = mat;

	if (parent == nullptr) {
		DrawMatrix = ModelMatrix;
	} else {
		DrawMatrix = parent->DrawMatrix * ModelMatrix;
	}

	update();
}

void SceneNode::move(const vec3& axis, float delta) {
	vec3 t = axis * delta;

	ModelMatrix = MatrixFactory::createTranslationMat4(t) * ModelMatrix;

	if (parent == nullptr) {
		DrawMatrix = ModelMatrix;
	} else {
		DrawMatrix = parent->DrawMatrix * ModelMatrix;
	}

	update();
}

void SceneNode::rotate(float angle, const vec3& axis) {
	qtrn tmp(angle, axis);
	qtrn r = normalize(tmp * currTransform.r);

	ModelMatrix = MatrixFactory::createMat4FromQtrn(r) * ModelMatrix;

	if (parent == nullptr) {
		DrawMatrix = ModelMatrix;
	}
	else {
		DrawMatrix = parent->DrawMatrix * ModelMatrix;
	}

	update();
}

void SceneNode::scale(const vec3& s) {
	ModelMatrix = MatrixFactory::createScaleMat4(s) * ModelMatrix;

	if (parent == nullptr) {
		DrawMatrix = ModelMatrix;
	}
	else {
		DrawMatrix = parent->DrawMatrix * ModelMatrix;
	}

	update();
}

void SceneNode::draw(Camera* camera) {
	if (mesh != nullptr) {
		shaderProgram->bind();

		if (shaderProgram->isUniform("ModelMatrix"))
			glUniformMatrix4fv(shaderProgram->uniforms["ModelMatrix"].index, 1, GL_TRUE, DrawMatrix.data);

		if (shaderProgram->isUniform("NormalMatrix")) {
			mat3 NormalMatrix = transpose(inverse(MatrixFactory::createMat3FromMat4(camera->getView() * DrawMatrix)));
			glUniformMatrix3fv(shaderProgram->uniforms["NormalMatrix"].index, 1, GL_TRUE, NormalMatrix.data);
		}

		if (shaderProgram->isUniform("Color1") && shaderProgram->isUniform("Color2")) {
			glUniform3fv(shaderProgram->uniforms["Color1"].index, 1, color[0].data());
			glUniform3fv(shaderProgram->uniforms["Color2"].index, 1, color[1].data());
		}

		if (shaderProgram->isUniform("Steps")) {
			glUniform1f(shaderProgram->uniforms["Steps"].index, steps);
		}

		if (!textures.empty()) {
			for (auto& i : textures) {
				i->updateShader(shaderProgram);
			}
		}
		
		if (callback)
			callback->beforeDraw(this);

		mesh->draw();

		if (callback)
			callback->afterDraw(this);

		shaderProgram->unbind();
	}

	for (SceneNode* child : children) child->draw(camera);
}

void SceneNode::update() {
	for (SceneNode* child : children) {
		child->DrawMatrix = DrawMatrix * child->ModelMatrix;
		child->update();
	}
}

void SceneNode::release() {
	while (children.size() != 0) {
		delete(children.back());
		children.pop_back();
	}
}