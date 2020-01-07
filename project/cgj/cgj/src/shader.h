#pragma once
#ifndef __SHADRE_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <map>

class ShaderProgram {
public:
	GLuint program_id;

	struct ShaderInfo {
		GLuint index;
	};
	std::map<GLenum, ShaderInfo> shaders;

	struct AttributeInfo {
		GLuint index;
	};
	std::map<std::string, AttributeInfo> attributes;

	struct UniformInfo {
		GLint index;
	};
	std::map<std::string, UniformInfo> uniforms;

	struct UboInfo {
		GLuint index;
		GLuint binding_point;
	};
	std::map<std::string, UboInfo> ubos;

	ShaderProgram();
	~ShaderProgram();

	void addShader(const GLenum shader_type, const std::string& filename);
	void addAttribute(const std::string& name, const GLuint index);
	bool isAttribute(const std::string& name);
	void addUniform(const std::string& name);
	bool isUniform(const std::string& name);
	void addUniformBlock(const std::string& name, const GLuint binding_point);
	bool isUniformBlock(const std::string& name);
	void create();
	void bind();
	void unbind();

private:
	const std::string read(const std::string& filename);
};

#endif