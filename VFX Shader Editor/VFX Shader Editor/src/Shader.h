#pragma once

#include "Globals.h"
#include "GL/glew.h"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Use();

private:
	const char* getShaderCode(const char* shaderPath);
	uint compileShader(const char* shaderCode, GLenum shaderType);

public:
	uint programID;
};
