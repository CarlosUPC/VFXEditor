#pragma once

#include "Globals.h"
#include "GL/glew.h"


class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Bind();
	void Unbind();

	//Set Uniform Types

private:
	const char* ParseShader(const char* shaderPath);
	uint CompileShader(const char*& source, uint type);
	uint CreateShader(const char*& vertexShader, const char*& fragmentShader);

public:
	uint m_programID;
};
