#include "ShaderUniform.h"

ShaderUniform::ShaderUniform()
{
}

ShaderUniform::~ShaderUniform()
{
}

UniformTexture::UniformTexture(const std::string& name, u32 texIdx, u32 index)
{
	this->uIndex = index;
	this->uName = name;
	this->uType = UNIFORM_TYPE::TEXTURE2D;
	//TEX ??
}

void UniformTexture::Bind(ResourceShader* shader)
{
	//SHADER->BINDTEXTURE()
}
