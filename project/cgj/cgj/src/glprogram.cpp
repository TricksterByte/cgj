#pragma once
#include "glprogram.h"

#include "mesh_manager.h"
#include "texture_manager.h"
#include "scene_manager.h"
#include "shader_manager.h"

GLProgram::GLProgram() {
	elapsed_time = 0;
	map = nullptr;
	water = nullptr;
	fireParticleSystem = nullptr;
	rainParticleSystem = nullptr;
}

GLProgram::~GLProgram() {
	if (map)
		delete(map);
}

void GLProgram::createMeshes() {
	std::string s("assets/meshes/cube.obj");
	Mesh* cube = new Mesh(s);
	MeshManager::getInstance()->add("cube", cube);

	s = "assets/meshes/poplar-treetop1.obj";
	Mesh* top1 = new Mesh(s);
	MeshManager::getInstance()->add("poplar-treetop1", top1);

	s = "assets/meshes/poplar-treetop2.obj";
	Mesh* top2 = new Mesh(s);
	MeshManager::getInstance()->add("poplar-treetop2", top2);

	s = "assets/meshes/poplar-trunk.obj";
	Mesh* trunk1 = new Mesh(s);
	MeshManager::getInstance()->add("poplar-trunk", trunk1);

	s = "assets/meshes/fir-treetop.obj";
	Mesh* top3 = new Mesh(s);
	MeshManager::getInstance()->add("fir-treetop", top3);

	s = "assets/meshes/fir-trunk.obj";
	Mesh* trunk2 = new Mesh(s);
	MeshManager::getInstance()->add("fir-trunk", trunk2);

	s = "assets/meshes/water.obj";
	Mesh* water = new Mesh(s);
	MeshManager::getInstance()->add("water", water);

	s = "assets/meshes/terrain.obj";
	Mesh* terrain = new Mesh(s);
	MeshManager::getInstance()->add("terrain", terrain);

	s = "assets/meshes/tent.obj";
	Mesh* tent = new Mesh(s);
	MeshManager::getInstance()->add("tent", tent);

	s = "assets/meshes/campfire.obj";
	Mesh* campfire = new Mesh(s);
	MeshManager::getInstance()->add("campfire", campfire);
}

void GLProgram::createTextures() {
	TextureCubeMap* cubemap = new TextureCubeMap();
	cubemap->loadCubeMap("assets/textures/nightsky_", ".jpg");
	TextureManager::getInstance()->add("skybox", (Texture*)cubemap);

	Texture2D* dudv = new Texture2D();
	dudv->load("assets/textures/waterDUDV.png");
	TextureManager::getInstance()->add("waterDuDv", (Texture*)dudv);

	ParticleTexture* fireParticle = new ParticleTexture(4, true);
	fireParticle->load("assets/textures/fire.png");
	TextureManager::getInstance()->add("fireParticle", (Texture*)fireParticle);
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

void GLProgram::createToonShader() {
	ShaderProgram* shader = new ShaderProgram();

	shader->addShader(GL_VERTEX_SHADER, "assets/shaders/celshading2-vs.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "assets/shaders/celshadingcolor-fs.glsl");

	shader->addAttribute("inVertex", Mesh::VERTEX);
	shader->addAttribute("inNormal", Mesh::NORMAL);
	shader->addUniform("ModelMatrix");
	shader->addUniformBlock("Camera", UBO_BP);
	shader->addUniform("Color1");
	shader->addUniform("Color2");
	shader->addUniform("Steps");
	shader->addUniform("FarPlane");
	shader->addUniform("DepthCube");
	shader->addUniform("LightPosition");
	shader->addUniform("LightIntensity");
	shader->addUniform("ClipPlane");
	shader->create();

	ShaderManager::getInstance()->add("scenegraph", shader);
}

void GLProgram::createSilhouetteShader() {
	ShaderProgram* shader = new ShaderProgram();

	shader->addShader(GL_VERTEX_SHADER, "assets/shaders/silhouette1-vs.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "assets/shaders/silhouette1-fs.glsl");
	shader->addAttribute("inVertex", Mesh::VERTEX);
	shader->addAttribute("inNormal", Mesh::NORMAL);
	shader->addUniform("ModelMatrix");
	shader->addUniformBlock("Camera", UBO_BP);
	shader->addUniform("Offset");
	shader->addUniform("ClipPlane");
	shader->create();

	shader->bind();
	glUniform1f(shader->uniforms["Offset"].index, Offset);
	shader->unbind();

	ShaderManager::getInstance()->add("silhouette", shader);
}

void GLProgram::createSkyboxShader() {
	ShaderProgram* shader = new ShaderProgram();

	shader->addShader(GL_VERTEX_SHADER, "assets/shaders/skybox-vs.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "assets/shaders/skybox-fs.glsl");
	shader->addAttribute("inVertex", Mesh::VERTEX);
	shader->addUniform("ModelMatrix");
	shader->addUniformBlock("Camera", UBO_BP);
	shader->addUniform("TexCubeMap");
	shader->create();

	ShaderManager::getInstance()->add("skybox", shader);
}

void GLProgram::createWaterShader() {
	ShaderProgram* shader = new ShaderProgram();

	shader->addShader(GL_VERTEX_SHADER, "assets/shaders/water-vs.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "assets/shaders/water-fs.glsl");
	shader->addAttribute("inVertex", Mesh::VERTEX);
	shader->addAttribute("inTexcoord", Mesh::TEXCOORD);
	shader->addUniform("ModelMatrix");
	shader->addUniformBlock("Camera", UBO_BP);
	shader->addUniform("Reflection");
	shader->addUniform("WaterDuDv");
	shader->addUniform("MoveFactor");
	shader->create();

	ShaderManager::getInstance()->add("water", shader);
}

void GLProgram::createShadowShader() {
	ShaderProgram* shader = new ShaderProgram();

	shader->addShader(GL_VERTEX_SHADER, "assets/shaders/shadowmap-vs.glsl");
	shader->addShader(GL_GEOMETRY_SHADER, "assets/shaders/shadowmap-gs.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "assets/shaders/shadowmap-fs.glsl");

	shader->addAttribute("inVertex", Mesh::VERTEX);
	shader->addUniform("ModelMatrix");
	shader->addUniform("LightPosition");
	shader->addUniform("FarPlane");
	shader->addUniform("ShadowMatrices[0]");
	shader->addUniform("ShadowMatrices[1]");
	shader->addUniform("ShadowMatrices[2]");
	shader->addUniform("ShadowMatrices[3]");
	shader->addUniform("ShadowMatrices[4]");
	shader->addUniform("ShadowMatrices[5]");
	shader->create();

	ShaderManager::getInstance()->add("shadowmap", shader);
}

void GLProgram::createFireParticleSystemShader() {
	ShaderProgram* shader = new ShaderProgram();

	shader->addShader(GL_VERTEX_SHADER, "assets/shaders/fire-particle-vs.glsl");
	shader->addShader(GL_GEOMETRY_SHADER, "assets/shaders/fire-particle-gs.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "assets/shaders/fire-particle-fs.glsl");
	shader->addUniformBlock("Camera", UBO_BP);
	shader->addUniform("ModelMatrix");
	shader->addUniform("NumberOfRows");
	shader->addUniform("ParticleTexture");
	shader->addAttribute("inVertex", FireParticleSystem::VERTEX);
	shader->addAttribute("inColor", FireParticleSystem::COLOR);
	shader->addAttribute("inSize", FireParticleSystem::SIZE);
	shader->addAttribute("inTexOffsets", FireParticleSystem::TEXOFFSET);
	shader->addAttribute("inBlend", FireParticleSystem::BLEND);

	shader->create();

	ShaderManager::getInstance()->add("fireParticles", shader);
}

void GLProgram::createRainParticleSystemShader() {
	ShaderProgram* shader = new ShaderProgram();

	shader->addShader(GL_VERTEX_SHADER, "assets/shaders/rain-particle-vs.glsl");
	shader->addShader(GL_GEOMETRY_SHADER, "assets/shaders/rain-particle-gs.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "assets/shaders/rain-particle-fs.glsl");
	shader->addUniformBlock("Camera", UBO_BP);
	shader->addUniform("ModelMatrix");
	shader->addAttribute("inVertex", RainParticleSystem::VERTEX);
	shader->addAttribute("inColor", RainParticleSystem::COLOR);
	shader->addAttribute("inSize", RainParticleSystem::SIZE);

	shader->create();

	ShaderManager::getInstance()->add("rainParticles", shader);
}

void GLProgram::createShaders() {
	createToonShader();
	createSilhouetteShader();
	createShadowShader();
	createWaterShader();
	createFireParticleSystemShader();
	createRainParticleSystemShader();
	createSkyboxShader();
}

SceneNode* GLProgram::createPoplarTree(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s) {
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

SceneNode* GLProgram::createFirTree(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s) {
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

SceneNode* GLProgram::createTent(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s) {
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

SceneNode* GLProgram::createCampfire(SceneNode* node, const vec3& t, const qtrn& r, const vec3& s) {
	SceneNode* c = node->createNode();

	SceneNode* n1 = c->createNode();
	n1->setMesh(MeshManager::getInstance()->get("campfire"));
	n1->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));
	n1->setColor({ 0.31f, 0.21f, 0.12f }, { 0.14f, 0.12f, 0.12f });
	n1->setSteps(4);

	SceneNode* n2 = c->createNode();
	n2->setMesh(MeshManager::getInstance()->get("campfire"));
	n2->setShaderProgram(ShaderManager::getInstance()->get("silhouette"));
	n2->setCallback(new BackMode());

	c->setModelMatrix(t, r, s);

	return c;
}

void GLProgram::createSceneGraph() {
	SceneGraph* graph = new SceneGraph();
	graph->setCamera(new Camera(UBO_BP));

	graph->getCamera()->setProjection(MatrixFactory::createPerspectiveProjectionMatrix(degreesToRadians(60), 640.0f / 480.0f, 1, 100));
	graph->getCamera()->changeTranslation(vec3(0, -2.f, 0));

	SceneManager::getInstance()->add("scenegraph", graph);

	SceneNode* root = graph->getRoot();
	root->setShaderProgram(ShaderManager::getInstance()->get("scenegraph"));

	TextureInfo* tinfo0 = new TextureInfo(GL_TEXTURE1, 1, "TexCubeMap", TextureManager::getInstance()->get("skybox"));

	SceneNode* skybox = root->createNode();
	skybox->setMesh(MeshManager::getInstance()->get("cube"));
	skybox->setShaderProgram(ShaderManager::getInstance()->get("skybox"));
	skybox->setCallback(new BackMode());
	skybox->addTextureInfo(tinfo0);
	skybox->scale(vec3(8.0f, 8.0f, 8.0f));

	TextureInfo* tinfo1 = new TextureInfo(GL_TEXTURE2, 2, "WaterDuDv", TextureManager::getInstance()->get("waterDuDv"));

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
	createTent(ground, vec3(-1.4f, 0.25f, -0.7f), qtrn(degreesToRadians(30.f), AXIS_Y), vec3(0.03f, 0.03f, 0.02f));
	createCampfire(ground, vec3(-1.23f, 0.25f, -0.3f), qtrn(), vec3(0.01f, 0.01f, 0.01f));

	map = new ShadowMap(1024, 1024, 1080, 720, vec3(-1.3f, 0.5f, -0.6f), 0.f, 100.f);
	map->setFbos(new ShadowsFramebuffers(1024, 1024, 1080, 720));
	ground->setShadowMap(map);

	TextureInfo* tinfo2 = new TextureInfo(GL_TEXTURE3, 3, "ParticleTexture", TextureManager::getInstance()->get("fireParticle"));

	fireParticleSystem = new FireParticleSystem(150, true);
	fireParticleSystem->setShaderProgram(ShaderManager::getInstance()->get("fireParticles"));
	fireParticleSystem->addTextureInfo(tinfo2);
	fireParticleSystem->move(vec3(1, 0, 0), -1.1f);
	fireParticleSystem->move(vec3(0, 1, 0), 0.28f);
	fireParticleSystem->move(vec3(0, 0, 1), -0.3f);
	ground->addNode(fireParticleSystem);

	rainParticleSystem = new RainParticleSystem(4000, false);
	rainParticleSystem->setShaderProgram(ShaderManager::getInstance()->get("rainParticles"));
	rainParticleSystem->move(vec3(0, 1, 0), 8.f);
	root->addNode(rainParticleSystem);

	updatables.push_back((IUpdatable*)fireParticleSystem);
	updatables.push_back((IUpdatable*)rainParticleSystem);
	updatables.push_back((IUpdatable*)water);
	updatables.push_back((IUpdatable*)map);
}

void GLProgram::drawSceneGraph() {
	SceneGraph* graph = SceneManager::getInstance()->get("scenegraph");

	std::vector<SceneNode*> nodes = graph->getRoot()->children;

	// Shadow pass

	glCullFace(GL_FRONT);
	map->getFbos()->bindFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	graph->getRoot()->children[2]->shadowPass();
	map->getFbos()->unbindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	Camera* camera = graph->getCamera();
	camera->bind();

	// Reflection pass

	WaterFramebuffers* fbos = water->getFbos();

	float distance = 2.f * (camera->translation.at(1, 3) + 0.1f);

	fbos->bindReflectionFramebuffer();
	{
		camera->changeTranslation(vec3(0, -distance, 0));
		camera->changeRotation(degreesToRadians(180.f), 0.f, 0.f);

		camera->bind();

		graph->getRoot()->scale(vec3(-1, 1, -1));

		nodes[0]->draw(graph->getCamera());

		glEnable(GL_CLIP_DISTANCE0);

		ShaderProgram* shader = ShaderManager::getInstance()->get("scenegraph");
		shader->bind();
		glUniform4f(shader->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, -0.1f);
		shader->unbind();

		shader = ShaderManager::getInstance()->get("silhouette");
		shader->bind();
		glUniform4f(shader->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, -0.1f);
		shader->unbind();

		nodes[2]->draw(graph->getCamera());
		nodes[3]->draw(graph->getCamera());

		glDisable(GL_CLIP_DISTANCE0);
	}
	fbos->unbindFramebuffer();

	graph->getRoot()->scale(vec3(-1, 1, -1));
	camera->changeRotation(degreesToRadians(180.f), 0.f, 0.f);
	camera->changeTranslation(vec3(0, distance, 0));
	camera->bind();

	// Draw scene

	nodes[0]->draw(camera);
	nodes[1]->draw(camera);

	glEnable(GL_CLIP_DISTANCE0);

	ShaderProgram* shader = ShaderManager::getInstance()->get("scenegraph");
	shader->bind();
	glUniform4f(shader->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, 0.1f);
	shader->unbind();

	shader = ShaderManager::getInstance()->get("silhouette");
	shader->bind();
	glUniform4f(shader->uniforms["ClipPlane"].index, 0.f, 1.f, 0.f, 0.1f);
	shader->unbind();

	nodes[2]->draw(camera);
	nodes[3]->draw(camera);

	glDisable(GL_CLIP_DISTANCE0);
}

void GLProgram::setViewProjectionMatrix() {
	SceneManager::getInstance()->get("scenegraph")->camera->changeTranslation(vec3(0, 0, -yDistance));
	yDistance = 0.0f;
	SceneManager::getInstance()->get("scenegraph")->camera->changeRotation(degreesToRadians(offsetX), degreesToRadians(offsetY), 0.f);
	offsetX = offsetY = 0.0f;
}

void GLProgram::update(float elapsed) {
	for (IUpdatable* updatable : updatables) {
		updatable->update(elapsed);
	}
}