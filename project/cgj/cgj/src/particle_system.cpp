#include "particle_system.h"

ParticleSystem::ParticleSystem(int max, bool hasTexture) : SceneNode(), maxParticles(max), hasTexture(hasTexture) {
	createBufferObjects();
}

ParticleSystem::~ParticleSystem() {
	while (particles.size() != 0) {
		delete(particles.back());
		particles.pop_back();
	};

	for (auto& t : textures) {
		delete(t);
	}

	textures.clear();
	destroyBufferObjects();
}

void ParticleSystem::createBufferObjects() {
	Vertices.resize(maxParticles);
	Colors.resize(maxParticles);
	Sizes.resize(maxParticles);

	if (hasTexture) {
		TexOffsets.resize(maxParticles);
		Blends.resize(maxParticles);
	}

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
		glEnableVertexAttribArray(VERTEX);
		glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(VERTEX, 1);

		glGenBuffers(1, &VboColors);
		glBindBuffer(GL_ARRAY_BUFFER, VboColors);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Color), nullptr, GL_STREAM_DRAW);
		glEnableVertexAttribArray(COLOR);
		glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(COLOR, 1);

		glGenBuffers(1, &VboSizes);
		glBindBuffer(GL_ARRAY_BUFFER, VboSizes);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Size), nullptr, GL_STREAM_DRAW);
		glEnableVertexAttribArray(SIZE);
		glVertexAttribPointer(SIZE, 1, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(SIZE, 1);

		if (hasTexture) {
			glGenBuffers(1, &VboTexOffsets);
			glBindBuffer(GL_ARRAY_BUFFER, VboTexOffsets);
			glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(TexOffset), nullptr, GL_STREAM_DRAW);
			glEnableVertexAttribArray(TEXOFFSET);
			glVertexAttribPointer(TEXOFFSET, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribDivisor(TEXOFFSET, 1);

			glGenBuffers(1, &VboBlends);
			glBindBuffer(GL_ARRAY_BUFFER, VboBlends);
			glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Blend), nullptr, GL_STREAM_DRAW);
			glEnableVertexAttribArray(BLEND);
			glVertexAttribPointer(BLEND, 1, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribDivisor(BLEND, 1);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::destroyBufferObjects() {
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTEX);
	glDisableVertexAttribArray(COLOR);
	glDisableVertexAttribArray(SIZE);
	glDisableVertexAttribArray(TEXOFFSET);
	glDisableVertexAttribArray(BLEND);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VboColors);
	glDeleteBuffers(1, &VboSizes);
	glDeleteBuffers(1, &VboTexOffsets);
	glDeleteBuffers(1, &VboBlends);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::update(float dt) { //fix this (initial burst)
	int created = 0;
	while (particles.size() != maxParticles) {
		if (timeSinceLast > EPSILON) {
			particles.push_back(createParticle());
			timeSinceLast -= EPSILON;
		}
		else {
			timeSinceLast += dt;
			break;
		}
	}

	int rows = 0;
	if (hasTexture) {
		if (textures[0] && textures[0]->texture) {
			rows = ((ParticleTexture*)textures[0]->texture)->getRows();
		}
		else {
			std::cout << "WARNING: no texture found" << std::endl;
		}
	}



	for (size_t sz = 0; sz < particles.size(); sz++) {
		Particle* p = particles[sz];

		if (!p->update(dt, rows)) {
			delete particles[sz];
			if (sz != particles.size()) {
				std::swap(particles[sz], particles[particles.size() - 1]);
			}
			particles.pop_back();
			sz--;
		}
		else {
			float s = particles[sz]->getSize();
			memcpy(&Vertices[sz], particles[sz]->getPosition().data(), sizeof(float) * 3);
			memcpy(&Colors[sz], particles[sz]->getColor().data(), sizeof(float) * 3);
			memcpy(&Sizes[sz], &s, sizeof(float) * 1);

			if (hasTexture) {
				float b = particles[sz]->getBlend();
				vec2 t1 = particles[sz]->getTexOffset1();
				vec2 t2 = particles[sz]->getTexOffset2();
				memcpy(&TexOffsets[sz], vec4(t1.x, t1.y, t2.x, t2.y).data(), sizeof(float) * 4);
				memcpy(&Blends[sz], &b, sizeof(float) * 1);
			}
		}
	}
}

void ParticleSystem::draw(Camera* cam) {
	if (hasTexture) {
		glEnable(GL_BLEND);
		if (((ParticleTexture*)textures[0]->texture)->getAdditive()) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
		else {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glDepthMask(GL_FALSE);
	}
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	shaderProgram->bind();
	if (shaderProgram->isUniform("ModelMatrix"))
		glUniformMatrix4fv(shaderProgram->uniforms["ModelMatrix"].index, 1, GL_TRUE, DrawMatrix.data);

	if (hasTexture) {
		if (!textures.empty()) {
			for (auto& i : textures) {
				i->updateShader(shaderProgram);
			}
			if (shaderProgram->isUniform("NumberOfRows"))
				glUniform1f(shaderProgram->uniforms["NumberOfRows"].index, (GLfloat)((ParticleTexture*)textures[0]->texture)->getRows());
		}
		else {
			std::cout << "WARNING: no texture found" << std::endl;
		}
	}

	glBindVertexArray(VaoId);

	glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Vertices.size() * sizeof(Vertex), &Vertices[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VboColors);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Color), nullptr, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Colors.size() * sizeof(Color), &Colors[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VboSizes);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Size), nullptr, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Sizes.size() * sizeof(Size), &Sizes[0]);

	if (hasTexture) {
		glBindBuffer(GL_ARRAY_BUFFER, VboTexOffsets);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(TexOffset), nullptr, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, TexOffsets.size() * sizeof(TexOffset), &TexOffsets[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VboBlends);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Blend), nullptr, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, Blends.size() * sizeof(Blend), &Blends[0]);
	}

	glDrawArraysInstanced(GL_POINTS, 0, 1, (GLsizei)particles.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (hasTexture) {
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
}

Particle* ParticleSystem::createParticle() {
	float x = random(-0.5f, 0.5f);
	float y = random(-0.5f, 0.5f);
	float z = random(-0.5f, 0.5f);
	vec3 position = currTransform.t + vec3(x, y, z);

	float velx = random(-0.5f, 0.5f);
	float vely = random(-0.5f, 0.5f);
	float velz = random(-0.5f, 0.5f);
	vec3 velocity = { velx, vely, velz };

	float r = ((float)rand()) / ((float)RAND_MAX);
	float g = ((float)rand()) / ((float)RAND_MAX);
	float b = ((float)rand()) / ((float)RAND_MAX);
	vec3 color = { r, g, b };

	float ge = random(0.0f, 0.1f);
	float ttl = random(0.1f, 3.0f);
	float size = ((float)rand()) / ((float)RAND_MAX * 10);

	return new Particle(position, velocity, color, ge, ttl, size);
}
