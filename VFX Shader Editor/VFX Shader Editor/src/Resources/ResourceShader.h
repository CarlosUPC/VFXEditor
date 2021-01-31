#ifndef __SHADER_H__
#define __SHADER_H__

#include "Globals.h"
#include "GL/glew.h"
#include "Resource.h"

class ShaderGraph;
class ResourceShader : public Resource
{
	friend class ShaderGraph;
public:
	ResourceShader();
	ResourceShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~ResourceShader();

	void Bind();
	void Unbind();

	bool LoadMemory() override;
	bool FreeMemory() override;

	void LoadShaderResource(const char* vertexSource, const char* fragmentSource);
	//Uniforms
	void SetUniformMat4f(const char* name, float* value) const;

private:


	std::string ReadShaderFromFile(const char* filename);
	uint CompileShader(const char* shaderCode, GLenum shaderType);
	uint CreateShader(const char* vertexSource, const char* fragmentSource);


	uint Compile();
	uint Recompile();
	
	void DestroyProgram();

public:

	uint programID;
	const char* vertex_path;
	const char* fragment_path;
	ShaderGraph* graph;
};
#endif // !__SHADER_H__