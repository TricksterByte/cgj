#include "camera.h"
#include "scene_graph.h"

SceneGraph::SceneGraph() {
	root = new SceneNode();
	camera = nullptr;
}

SceneGraph::~SceneGraph() {
	delete(camera);
	delete(root);
}

SceneNode* SceneGraph::getRoot() {
	return root;
}

SceneNode* SceneGraph::createNode() {
	SceneNode* node = root->createNode();
	return node;
}

Camera* SceneGraph::getCamera() {
	return camera;
}

void SceneGraph::setCamera(Camera* cam) {
	camera = cam;
}

void SceneGraph::draw() {
	if (root->shaderProgram->isUniformBlock("Camera"))
		camera->bind();

	root->draw(camera);
}