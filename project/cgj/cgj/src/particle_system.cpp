#include "particle_system.h"
#include "math_helpers.h"

ParticleSystem::ParticleSystem(int max) : SceneNode() {
	maxParticles = max;

	createBufferObjects();
}

ParticleSystem::~ParticleSystem() {
	while (particles.size() != 0) {
		delete(particles.back());
		particles.pop_back();
	};

	destroyBufferObjects();
}

void ParticleSystem::createBufferObjects() {
	Vertices.resize(maxParticles);

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STREAM_DRAW);
		glEnableVertexAttribArray(Mesh::VERTEX);
		glVertexAttribPointer(Mesh::VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ParticleSystem::destroyBufferObjects() {
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(Mesh::VERTEX);
	glDeleteBuffers(1, &VboVertices);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::update(float dt) {
	while (particles.size() != maxParticles) {
		Particle* p = createParticle();
		if (p != nullptr) {
			particles.push_back(p);
		}
		else {
			timeSinceLast += dt;
			break;
		}
	}

	for (size_t sz = 0; sz < particles.size(); sz++) {
		Particle* p = particles[sz];

		if (!p->update(dt)) {
			delete particles[sz];

			if (sz != particles.size()) {
				std::swap(particles[sz], particles[particles.size() - 1]);
			}

			particles.pop_back();
			sz--;
		}
		else {
			memcpy(&Vertices[sz], particles[sz]->position.data(), sizeof(float) * 3);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Vertices.size() * sizeof(Vertex), &Vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::draw(Camera* cam) {
	shaderProgram->bind();

	if (shaderProgram->isUniform("ModelMatrix"))
		glUniformMatrix4fv(shaderProgram->uniforms["ModelMatrix"].index, 1, GL_TRUE, DrawMatrix.data);

	glBindVertexArray(VaoId);
	if (particles.size() > 0) {

		if (shaderProgram->isUniform("Size"))
			glUniform1f(shaderProgram->uniforms["Size"].index, particles[0]->size);

		if (shaderProgram->isUniform("Color"))
			glUniform3fv(shaderProgram->uniforms["Color"].index, 1, particles[0]->color.data());

		glDrawArrays(GL_POINTS, 0, (GLsizei)particles.size());
	}
	glBindVertexArray(0);

	shaderProgram->unbind();
}

Particle* ParticleSystem::createParticle() {
	if (timeSinceLast > EPSILON) {
		timeSinceLast -= EPSILON;

		float x = random(-2.f, 2.f);
		float z = random(-2.f, 2.f);
		float ttl = random(0.f, 2.f);

		vec3 velocity = { x, 12.f, z };

		return new Particle(currTransform.t, velocity, vec3(1.0f, 0.f, 0.f), 0.1f, true, ttl);
	}
	else {
		return nullptr;
	}
}