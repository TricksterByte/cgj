#include <iostream>
#include <cmath>
#include <map>
#include <iomanip>

#include "vectors.h"
#include "matrices.h"
#include "matrix_factory.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "math_helpers.h"
#include "quaternion.h"
#include "scene_node.h"
#include "scene_graph.h"
#include "i_scene_node_callback.h"

#include "fire_particle_system.h"
#include "rain_particle_system.h"
#include "water_fbos.h"
#include "water.h"

#include "mesh_manager.h"
#include "texture_manager.h"
#include "shader_manager.h"
#include "scene_manager.h"
#include "keybuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

enum class MouseState { STOPPED, STARTING, ROTATING };

const GLuint UBO_BP = 0;
GLfloat Offset = 0.02f;
GLfloat OffsetDelta = 0.001f;

MouseState state = MouseState::STOPPED;
float elapsed_time;
vec2 lastPosition;
float offsetX = 0.0f;
float offsetY = 0.0f;
float yDistance = 0.0f;

void createMeshes() {
	std::string s("../../../project/cgj/cgj/assets/cube.obj");
	Mesh* cube = new Mesh(s);
	MeshManager::getInstance()->add("cube", cube);

	s = "../../../project/cgj/cgj/assets/poplar-treetop1.obj";
	Mesh* top1 = new Mesh(s);
	MeshManager::getInstance()->add("poplar-treetop1", top1);

	s = "../../../project/cgj/cgj/assets/poplar-treetop2.obj";
	Mesh* top2 = new Mesh(s);
	MeshManager::getInstance()->add("poplar-treetop2", top2);

	s = "../../../project/cgj/cgj/assets/poplar-trunk.obj";
	Mesh* trunk1 = new Mesh(s);
	MeshManager::getInstance()->add("poplar-trunk", trunk1);

	s = "../../../project/cgj/cgj/assets/fir-treetop.obj";
	Mesh* top3 = new Mesh(s);
	MeshManager::getInstance()->add("fir-treetop", top3);

	s = "../../../project/cgj/cgj/assets/fir-trunk.obj";
	Mesh* trunk2 = new Mesh(s);
	MeshManager::getInstance()->add("fir-trunk", trunk2);

	s = "../../../project/cgj/cgj/assets/field.obj";
	Mesh* ground = new Mesh(s);
	MeshManager::getInstance()->add("ground", ground);

	s = "../../../project/cgj/cgj/assets/water.obj";
	Mesh* water = new Mesh(s);
	MeshManager::getInstance()->add("water", water);

	s = "../../../project/cgj/cgj/assets/terrain.obj";
	Mesh* terrain = new Mesh(s);
	MeshManager::getInstance()->add("terrain", terrain);

	s = "../../../project/cgj/cgj/assets/lake.obj";
	Mesh* lake = new Mesh(s);
	MeshManager::getInstance()->add("lake", lake);

	s = "../../../project/cgj/cgj/assets/tent.obj";
	Mesh* tent = new Mesh(s);
	MeshManager::getInstance()->add("tent", tent);
}

void createTextures() {
	TextureCubeMap* cubemap = new TextureCubeMap();
	cubemap->loadCubeMap("../../../project/cgj/cgj/assets/nightsky_", ".jpg");
	TextureManager::getInstance()->add("skybox", (Texture*)cubemap);

	Texture2D* dudv = new Texture2D();
	dudv->load("../../../project/cgj/cgj/assets/waterDUDV.png");
	TextureManager::getInstance()->add("waterDuDv", (Texture*)dudv);

	ParticleTexture* fireParticle = new ParticleTexture(4, true);
	fireParticle->load("../../../project/cgj/cgj/assets/fire.png");
	TextureManager::getInstance()->add("fireParticle", (Texture*) fireParticle);
}

void createToonShader() {
	ShaderProgram* program = new ShaderProgram();

	program->addShader(GL_VERTEX_SHADER, "../../../project/cgj/cgj/assets/celshading2-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "../../../project/cgj/cgj/assets/celshadingcolor-fs.glsl");

	program->addAttribute("inVertex", Mesh::VERTEX);
	program->addAttribute("inNormal", Mesh::NORMAL);
	program->addUniform("ModelMatrix");
	program->addUniformBlock("Camera", UBO_BP);
	program->addUniform("Color1");
	program->addUniform("Color2");
	program->addUniform("Steps");
	program->addUniform("FarPlane");
	program->addUniform("DepthCube");
	program->addUniform("LightPosition");
	program->addUniform("LightIntensity");
	program->addUniform("ClipPlane");
	program->create();

	ShaderManager::getInstance()->add("scenegraph", program);
}

void createSilhouetteShader() {
	ShaderProgram* program = new ShaderProgram();

	program->addShader(GL_VERTEX_SHADER, "../../../project/cgj/cgj/assets/silhouette1-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "../../../project/cgj/cgj/assets/silhouette1-fs.glsl");
	program->addAttribute("inVertex", Mesh::VERTEX);
	program->addAttribute("inNormal", Mesh::NORMAL);
	program->addUniform("ModelMatrix");
	program->addUniformBlock("Camera", UBO_BP);
	program->addUniform("Offset");
	program->addUniform("ClipPlane");
	program->create();

	program->bind();
	glUniform1f(program->uniforms["Offset"].index, Offset);
	program->unbind();

	ShaderManager::getInstance()->add("silhouette", program);
}

void createSkyboxShader() {
	ShaderProgram* program = new ShaderProgram();

	program->addShader(GL_VERTEX_SHADER, "../../../project/cgj/cgj/assets/skybox-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "../../../project/cgj/cgj/assets/skybox-fs.glsl");
	program->addAttribute("inVertex", Mesh::VERTEX);
	program->addUniform("ModelMatrix");
	program->addUniformBlock("Camera", UBO_BP);
	program->addUniform("TexCubeMap");
	program->create();

	ShaderManager::getInstance()->add("skybox", program);
}

class BackMode : public ISceneNodeCallback {
public:
	void beforeDraw(SceneNode* node) {
		glCullFace(GL_FRONT);
	}

	void afterDraw(SceneNode* node) {
		glCullFace(GL_BACK);
	}
};

void createWaterShader() {
	ShaderProgram* program = new ShaderProgram();

	program->addShader(GL_VERTEX_SHADER, "../../../project/cgj/cgj/assets/water-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "../../../project/cgj/cgj/assets/water-fs.glsl");
	program->addAttribute("inVertex", Mesh::VERTEX);
	program->addAttribute("inTexcoord", Mesh::TEXCOORD);
	program->addUniform("ModelMatrix");
	program->addUniformBlock("Camera", UBO_BP);
	program->addUniform("Reflection");
	program->addUniform("WaterDuDv");
	program->addUniform("MoveFactor");
	program->create();

	ShaderManager::getInstance()->add("water", program);
}

void createShadowShader() {
	ShaderProgram* program = new ShaderProgram();

	program->addShader(GL_VERTEX_SHADER, "../../../project/cgj/cgj/assets/shadowmap-vs.glsl");
	program->addShader(GL_GEOMETRY_SHADER, "../../../project/cgj/cgj/assets/shadowmap-gs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "../../../project/cgj/cgj/assets/shadowmap-fs.glsl");

	program->addAttribute("inVertex", Mesh::VERTEX);
	program->addUniform("ModelMatrix");
	program->addUniform("LightPosition");
	program->addUniform("FarPlane");
	program->addUniform("ShadowMatrices[0]");
	program->addUniform("ShadowMatrices[1]");
	program->addUniform("ShadowMatrices[2]");
	program->addUniform("ShadowMatrices[3]");
	program->addUniform("ShadowMatrices[4]");
	program->addUniform("ShadowMatrices[5]");
	program->create();

	ShaderManager::getInstance()->add("shadowmap", program);
}

void createFireParticleSystemShader() {
	ShaderProgram* program = new ShaderProgram();

	program->addShader(GL_VERTEX_SHADER, "../../../project/cgj/cgj/assets/fire-particle-vs.glsl");
	program->addShader(GL_GEOMETRY_SHADER, "../../../project/cgj/cgj/assets/fire-particle-gs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "../../../project/cgj/cgj/assets/fire-particle-fs.glsl");
	program->addUniformBlock("Camera", UBO_BP);
	program->addUniform("ModelMatrix");
	program->addUniform("NumberOfRows");
	program->addUniform("ParticleTexture");
	program->addAttribute("inVertex", FireParticleSystem::VERTEX);
	program->addAttribute("inColor", FireParticleSystem::COLOR);
	program->addAttribute("inSize", FireParticleSystem::SIZE);
	program->addAttribute("inTexOffsets", FireParticleSystem::TEXOFFSET);
	program->addAttribute("inBlend", FireParticleSystem::BLEND);

	program->create();

	ShaderManager::getInstance()->add("fireParticles", program);
}

void createRainParticleSystemShader() {
	ShaderProgram* program = new ShaderProgram();

	program->addShader(GL_VERTEX_SHADER, "../../../project/cgj/cgj/assets/rain-particle-vs.glsl");
	program->addShader(GL_GEOMETRY_SHADER, "../../../project/cgj/cgj/assets/rain-particle-gs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "../../../project/cgj/cgj/assets/rain-particle-fs.glsl");
	program->addUniformBlock("Camera", UBO_BP);
	program->addUniform("ModelMatrix");
	program->addAttribute("inVertex", RainParticleSystem::VERTEX);
	program->addAttribute("inColor", RainParticleSystem::COLOR);
	program->addAttribute("inSize", RainParticleSystem::SIZE);

	program->create();

	ShaderManager::getInstance()->add("rainParticles", program);
}

// --------------------------------------- SCENE ------------------------------------------

SceneNode* createPoplarTree(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s) {
	SceneNode* c = node->createNode();

	SceneNode* n1 = c->createNode();
	n1->setMesh(MeshManager::getInstance()->get("poplar-trunk"));
	n1->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	n1->setColor({ 0.4f, 0.15f, 0.f }, { 0.2f, 0.1f, 0.1f });
	n1->setSteps(4);

	SceneNode* n11 = n1->createNode();
	n11->setMesh(MeshManager::getInstance()->get("poplar-treetop1"));
	n11->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	n11->setColor({ 0.353f, 0.561f, 0.365f }, { 0.219f, 0.431f, 0.231f });
	n11->setSteps(4);

	SceneNode* n12 = n1->createNode();
	n12->setMesh(MeshManager::getInstance()->get("poplar-treetop2"));
	n12->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	n12->setColor({ 0.360f, 0.57f, 0.365f }, { 0.219f, 0.431f, 0.231f });
	n12->setSteps(4);

	SceneNode* n2 = c->createNode();
	n2->setMesh(MeshManager::getInstance()->get("poplar-trunk"));
	n2->setShaderProgram(ShaderManager::getInstance()->get("silhouette"));
	n2->setCallback(new BackMode());

	SceneNode* n21 = n2->createNode();
	n21->setMesh(MeshManager::getInstance()->get("poplar-treetop1"));
	n21->setShaderProgram(ShaderManager::getInstance()->get("silhouette"));
	n21->setCallback(new BackMode());

	SceneNode* n22 = n2->createNode();
	n22->setMesh(MeshManager::getInstance()->get("poplar-treetop2"));
	n22->setShaderProgram(ShaderManager::getInstance()->get("silhouette"));
	n22->setCallback(new BackMode());

	c->setModelMatrix(t, r, s);

	return c;
}

SceneNode* createFirTree(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s) {
	SceneNode* c = node->createNode();

	SceneNode* n1 = c->createNode();
	n1->setMesh(MeshManager::getInstance()->get("fir-trunk"));
	n1->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	n1->setColor({ 0.4f, 0.15f, 0.f }, { 0.2f, 0.1f, 0.1f });
	n1->setSteps(4);

	SceneNode* n11 = n1->createNode();
	n11->setMesh(MeshManager::getInstance()->get("fir-treetop"));
	n11->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	n11->setColor({ 0.33f, 0.2f, 0.2f }, { 0.5f, 0.569f, 0.33f });
	n11->setSteps(4);

	SceneNode* n2 = c->createNode();
	n2->setMesh(MeshManager::getInstance()->get("fir-trunk"));
	n2->setShaderProgram(ShaderManager::getInstance()->get("silhouette"));
	n2->setCallback(new BackMode());

	SceneNode* n21 = n2->createNode();
	n21->setMesh(MeshManager::getInstance()->get("fir-treetop"));
	n21->setShaderProgram(ShaderManager::getInstance()->get("silhouette"));
	n21->setCallback(new BackMode());

	c->setModelMatrix(t, r, s);

	return c;
}

SceneNode* createTent(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s) {
	SceneNode* c = node->createNode();

	SceneNode* n1 = c->createNode();
	n1->setMesh(MeshManager::getInstance()->get("tent"));
	n1->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	n1->setColor({ 0.066f, 0.58f, 0.819f }, { 0.39f, 0.65f, 0.768f });
	n1->setSteps(4);

	SceneNode* n2 = c->createNode();
	n2->setMesh(MeshManager::getInstance()->get("tent"));
	n2->setShaderProgram(ShaderManager::getInstance()->get("silhouette"));
	n2->setCallback(new BackMode());

	c->setModelMatrix(t, r, s);

	return c;
}

// ----------------------------------------------------------------------------------------

ShadowMap* map;
ShadowsFramebuffers* fbos;
Water* water;
FireParticleSystem* fireParticleSystem;
RainParticleSystem* rainParticleSystem;

void createSceneGraph() {
	SceneGraph* graph = new SceneGraph();
	graph->setCamera(new Camera(UBO_BP));

	graph->getCamera()->setProjection(MatrixFactory::createPerspectiveProjectionMatrix(degreesToRadians(60), 640.0f / 480.0f, 1, 100));
	graph->getCamera()->changeTranslation(vec3(0, -2.f, 0));

	SceneManager::getInstance()->add("scenegraph", graph);

	SceneNode* root = graph->getRoot();
	root->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));

	TextureInfo* tinfo0 = new TextureInfo(GL_TEXTURE1, 1, "TexCubeMap", TextureManager::getInstance()->get("skybox"), nullptr);

	SceneNode* skybox = root->createNode();
	skybox->setMesh(MeshManager::getInstance()->get("cube"));
	skybox->setShaderProgram(ShaderManager::getInstance()->get("skybox"));
	skybox->setCallback(new BackMode());
	skybox->addTextureInfo(tinfo0);
	skybox->scale(vec3(32.0f, 32.0f, 32.0f));

	TextureInfo* tinfo1 = new TextureInfo(GL_TEXTURE2, 2, "WaterDuDv", TextureManager::getInstance()->get("waterDuDv"), nullptr);

	water = new Water();
	water->setFbos(new WaterFramebuffers(1080, 720, 1080, 720, 1080, 720));
	water->setMesh(MeshManager::getInstance()->get("water"));
	water->setShaderProgram(ShaderManager::getInstance()->get("water"));
	water->addTextureInfo(tinfo1);
	water->setModelMatrix(
		vec3(1, -0.1f, 0),
		qtrn(),
		vec3(4.0f, 1.0f, 5.0f)
	);
	root->addNode(water);

	SceneNode* ground = root->createNode();
	ground->setMesh(MeshManager::getInstance()->get("terrain"));
	ground->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	ground->setColor({ 0.f, 0.2f, 0.2f }, { 0.f, 0.2f, 0.2f });
	ground->setSteps(2);
	ground->setModelMatrix(
		AXIS_Y * -0.4f,
		qtrn(),
		vec3(1.5f, 1.5f, 1.55f)
	);

	SceneNode* lake = ground->createNode();
	lake->setMesh(MeshManager::getInstance()->get("lake"));
	lake->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	lake->setColor({ 0.48f, 0.39f, 0.29f }, { 0.48f, 0.39f, 0.29f });
	lake->setSteps(2);

	createPoplarTree(ground, vec3(-1.7f, 0.2f, 0.0f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createPoplarTree(ground, vec3(-1.2f, 0.2f, 1.2f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createPoplarTree(ground, vec3(-2.3f, 0.2f, 0.6f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createPoplarTree(ground, vec3(-1.2f, 0.2f, 0.2f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createPoplarTree(ground, vec3(-1.2f, 0.2f, -1.2f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createPoplarTree(ground, vec3(-1.7f, 0.2f, -1.2f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createFirTree(ground, vec3(-2.0f, 0.2f, 1.5f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createFirTree(ground, vec3(-2.0f, 0.2f, 0.0f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createFirTree(ground, vec3(-1.6f, 0.2f, 1.0f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createFirTree(ground, vec3(-2.f, 0.2f, 1.0f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createFirTree(ground, vec3(-2.3f, 0.2f, 2.0f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createFirTree(ground, vec3(-1.2f, 0.2f, -2.3f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createFirTree(ground, vec3(-1.2f, 0.2f, -1.7f), qtrn(), vec3(0.15f, 0.15f, 0.15f));
	createTent(ground, vec3(-1.4f, 0.2f, -0.7f), qtrn(degreesToRadians(30.f), AXIS_Y), vec3(0.15f, 0.15f, 0.15f));
	
	map = new ShadowMap(1024, 1024, 1080, 720, vec3(-1.3f, 0.5f, -0.6f), 0.f, 100.f);
	map->setFbos(new ShadowsFramebuffers(1024, 1024, 1080, 720));
	ground->setShadowMap(map);

	TextureInfo* tinfo2 = new TextureInfo(GL_TEXTURE3, 3, "ParticleTexture", TextureManager::getInstance()->get("fireParticle"), nullptr);

	fireParticleSystem = new FireParticleSystem(150, true);
	fireParticleSystem->setShaderProgram(ShaderManager::getInstance()->get("fireParticles"));
	fireParticleSystem->addTextureInfo(tinfo2);
	fireParticleSystem->move(vec3(1, 0, 0), -1.1f);
	fireParticleSystem->move(vec3(0, 1, 0), 0.28f);
	fireParticleSystem->move(vec3(0, 0, 1), -0.3f);
	ground->addNode(fireParticleSystem);

	rainParticleSystem = new RainParticleSystem(1500, false);
	rainParticleSystem->setShaderProgram(ShaderManager::getInstance()->get("rainParticles"));
	rainParticleSystem->move(vec3(0, 1, 0), 8.f);
	root->addNode(rainParticleSystem);
}

void drawSceneGraph() {
	SceneGraph* graph = SceneManager::getInstance()->get("scenegraph");

	std::vector<SceneNode*> nodes = graph->getRoot()->children;

	glCullFace(GL_FRONT);
	map->getFbos()->bindFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	graph->getRoot()->children[2]->shadowPass();
	map->getFbos()->unbindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	Camera* camera = graph->getCamera();
	camera->bind();

	WaterFramebuffers* fbos = water->getFbos();
	fbos->bindRefractionFramebuffer();
	{
		glEnable(GL_CLIP_DISTANCE0);

		ShaderProgram* program = ShaderManager::getInstance()->get("scenegraph");
		program->bind();
		glUniform4f(program->uniforms["ClipPlane"].index, 0.f, -1.f, 0.f, -0.1f);
		program->unbind();

		program = ShaderManager::getInstance()->get("silhouette");
		program->bind();
		glUniform4f(program->uniforms["ClipPlane"].index, 0.f, -1.f, 0.f, -0.1f);
		program->unbind();

		nodes[2]->draw(camera);

		glDisable(GL_CLIP_DISTANCE0);
	}
	fbos->unbindFramebuffer();

	float distance = 2.f * (camera->translation.at(1, 3) + 0.1f);

	fbos->bindReflectionFramebuffer();
	{
		camera->changeTranslation(vec3(0, -distance, 0));
		camera->changeRotation(degreesToRadians(180.f), 0.f, 0.f);

		camera->bind();

		graph->getRoot()->scale(vec3(-1, 1, -1));

		nodes[0]->draw(graph->getCamera());

		glEnable(GL_CLIP_DISTANCE0);

		ShaderProgram* program = ShaderManager::getInstance()->get("scenegraph");
		program->bind();
		glUniform4f(program->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, -0.1f);
		program->unbind();

		program = ShaderManager::getInstance()->get("silhouette");
		program->bind();
		glUniform4f(program->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, -0.1f);
		program->unbind();

		nodes[2]->draw(graph->getCamera());
		nodes[3]->draw(graph->getCamera());

		glDisable(GL_CLIP_DISTANCE0);
	}
	fbos->unbindFramebuffer();

	graph->getRoot()->scale(vec3(-1, 1, -1));
	camera->changeRotation(degreesToRadians(180.f), 0.f, 0.f);
	camera->changeTranslation(vec3(0, distance, 0));
	camera->bind();

	nodes[0]->draw(camera);
	nodes[1]->draw(camera);

	glEnable(GL_CLIP_DISTANCE0);

	ShaderProgram* program = ShaderManager::getInstance()->get("scenegraph");
	program->bind();
	glUniform4f(program->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, 0.1f);
	program->unbind();

	program = ShaderManager::getInstance()->get("silhouette");
	program->bind();
	glUniform4f(program->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, 0.1f);
	program->unbind();

	nodes[2]->draw(camera);
	nodes[3]->draw(camera);

	glDisable(GL_CLIP_DISTANCE0);
}

void setViewProjectionMatrix() {
	SceneManager::getInstance()->get("scenegraph")->camera->changeTranslation(vec3(0, 0, -yDistance));
	yDistance = 0.0f;
	SceneManager::getInstance()->get("scenegraph")->camera->changeRotation(degreesToRadians(offsetX), degreesToRadians(offsetY), 0.f);
	offsetX = offsetY = 0.0f;
}

#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl;
	std::cerr << "Press <return>.";
	std::cin.ignore();
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

#endif // ERROR_CALLBACK

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	delete(map);
	delete(fbos);
	ShaderManager::freeInstance();
	SceneManager::freeInstance();
	MeshManager::freeInstance();
	TextureManager::freeInstance();
	KeyBuffer::freeInstance();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		fireParticleSystem->toggle();
		rainParticleSystem->toggle();
	}

	switch (action) {
	case GLFW_PRESS:
		KeyBuffer::getInstance()->pressKey(key);
		break;
	case GLFW_RELEASE:
		KeyBuffer::getInstance()->releaseKey(key);
		break;
	default:
		break;
	}
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			if (state == MouseState::STOPPED)
				state = MouseState::STARTING;
		}
		else if (action == GLFW_RELEASE) {
				state = MouseState::STOPPED;
		}
	}
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	switch (state) {
	case MouseState::STOPPED:
		break;
	case MouseState::STARTING:
		lastPosition.x = (float)xpos;
		lastPosition.y = (float)ypos;
		state = MouseState::ROTATING;
		break;
	case MouseState::ROTATING:
		offsetX = (float)(lastPosition.x - xpos) * SceneManager::getInstance()->get("scenegraph")->camera->getSensitivity();
		offsetY = (float)(lastPosition.y - ypos) * SceneManager::getInstance()->get("scenegraph")->camera->getSensitivity();

		lastPosition.x = (float)xpos;
		lastPosition.y = (float)ypos;
		break;
	}
}

void mouse_scroll_callback(GLFWwindow* win, double xpos, double ypos)
{
	yDistance = (float)ypos;
}

///////////////////////////////////////////////////////////////////////// SETUP

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetScrollCallback(win, mouse_scroll_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);
#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
	createMeshes();
	createTextures();
	createToonShader();
	createSilhouetteShader();
	createShadowShader();
	createSkyboxShader();
	createWaterShader();
	createFireParticleSystemShader();
	createRainParticleSystemShader();
	createSceneGraph();
	return win;
}

////////////////////////////////////////////////////////////////////////// RUN

float updateLights = 0.055f;
float updateTime = 0.f;

void display(GLFWwindow* win, float elapsed_sec)
{
	bool offset_changed = false;
	if (KeyBuffer::getInstance()->isKeyPressed(GLFW_KEY_Q)) {
		Offset += OffsetDelta;
		offset_changed = true;
	}
	if (KeyBuffer::getInstance()->isKeyPressed(GLFW_KEY_A)) {
		Offset -= OffsetDelta;
		offset_changed = true;
	}
	if (offset_changed) {
		Offset = Offset < 0.0f ? 0.0f : Offset;

		std::cout << std::setprecision(2);
		std::cout << "Offset: " << Offset << std::endl;

		ShaderProgram* program = ShaderManager::getInstance()->get("silhouette");
		program->bind();
		glUniform1f(program->uniforms["Offset"].index, Offset);
		program->unbind();
	}

	updateTime += elapsed_sec;
	//if (updateTime >= updateLights) {
	//	updateTime = 0.f;
	//	map->lightPos = vec3(random(0, 0.1f), random(2.f, 2.2f), random(0, 0.1f));
	//	map->lightIntensity = random(0.35, 0.45);
	//}

	offsetX += elapsed_sec * 0.7f;
	water->update(elapsed_sec);
	fireParticleSystem->update(elapsed_sec);
	rainParticleSystem->update(elapsed_sec);
	setViewProjectionMatrix();
	drawSceneGraph();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		elapsed_time = (float)(time - last_time);
		last_time = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GLFWwindow* win = setup(gl_major, gl_minor,
		1080, 720, "Cel Shading", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END