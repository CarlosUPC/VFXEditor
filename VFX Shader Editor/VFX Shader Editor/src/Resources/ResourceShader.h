#ifndef __SHADER_H__
#define __SHADER_H__

#include "Globals.h"
#include "GL/glew.h"
#include "Resource.h"

class ResourceShader : public Resource
{
public:
	ResourceShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~ResourceShader();

	void Bind();
	void Unbind();

	//Uniforms
	void SetUniformMat4f(const char* name, float* value) const;

private:


	std::string ReadShaderFromFile(const char* filename);
	uint CompileShader(const char* shaderCode, GLenum shaderType);
	uint CreateShader(const char* vertexSource, const char* fragmentSource);

public:

	uint programID;
};
#endif // !__SHADER_H__