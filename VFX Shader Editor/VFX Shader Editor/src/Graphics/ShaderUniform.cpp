#include "ShaderUniform.h"

ShaderUniform::ShaderUniform()
{
}

ShaderUniform::~ShaderUniform()
{
}

UniformTexture::UniformTexture(const std::string& name, uint texIdx, u32 index)
{
	this->uIndex = index;
	this->uName = name;
	this->uType = UNIFORM_TYPE::TEXTURE2D;
	this->texID = texIdx;
	//TEX ??
}

void UniformTexture::Bind(ResourceShader* shader)
{
	shader->BindTexture(uName.c_str(), texID, uIndex);
}
