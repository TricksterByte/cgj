#include "shader.h"

#include <iostream>
#include <fstream>

ShaderProgram::ShaderProgram() {
	program_id = glCreateProgram();

	shaders = std::map<GLenum, ShaderInfo>();
	attributes = std::map<std::string, AttributeInfo>();
	uniforms = std::map<std::string, UniformInfo>();
	ubos = std::map<std::string, UboInfo>();
}

ShaderProgram::~ShaderProgram() {
	glUseProgram(0);
	glDeleteProgram(program_id);
}

void ShaderProgram::addShader(const GLenum shader_type, const std::string& filename) {
	const GLuint shader_id = glCreateShader(shader_type);
	const std::string s = read(filename);
	const GLchar* code = s.c_str();

	glShaderSource(shader_id, 1, &code, 0);
	glCompileShader(shader_id);
	glAttachShader(program_id, shader_id);

	shaders[shader_type] = { shader_id };
}

const std::string ShaderProgram::read(const std::string& filename) {
	std::ifstream file;
	std::string source;

	std::string line;

	file.open(filename);
	if (file.is_open())
		while (std::getline(file, line))
			source += line + "\n";
	else
		std::cerr << "Couldn't open file at " << filename << std::endl;
	file.close();

	return source;
}

void ShaderProgram::addAttribute(const std::string& name, const GLuint index) {
	glBindAttribLocation(program_id, index, name.c_str());
	attributes[name] = { index };
}

bool ShaderProgram::isAttribute(const std::string& name) {
	return attributes.find(name) != attributes.end();
}

void ShaderProgram::addUniform(const std::string& name) {
	uniforms[name] = { -1 };
}

bool ShaderProgram::isUniform(const std::string& name) {
	return uniforms.find(name) != uniforms.end();
}

void ShaderProgram::addUniformBlock(const std::string& name, const GLuint binding_point) {
	ubos[name] = { 0, binding_point };
}

bool ShaderProgram::isUniformBlock(const std::string& name) {
	return ubos.find(name) != ubos.end();
}

void ShaderProgram::create() {
	glLinkProgram(program_id);

	for (auto& i : shaders) {
		glDetachShader(program_id, i.second.index);
		glDeleteShader(i.second.index);
	}

	for (auto& i : uniforms) {
		i.second.index = glGetUniformLocation(program_id, i.first.c_str());
		if (i.second.index < 0) std::cerr << "WARNING: Uniform " << i.first << " not found" << std::endl;
	}

	for (auto& i : ubos) {
		i.second.index = glGetUniformBlockIndex(program_id, i.first.c_str());
		if (i.second.index < 0) std::cerr << "Warning: UBO " << i.first << " not found" << std::endl;
		glUniformBlockBinding(program_id, i.second.index, i.second.binding_point);
	}
}

void ShaderProgram::bind() {
	glUseProgram(program_id);
}

void ShaderProgram::unbind() {
	glUseProgram(0);
}