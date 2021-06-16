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
	
}

void UniformTexture::Bind(ResourceShader* shader)
{
	shader->BindTexture(uName.c_str(), texID, uIndex);
}

UniformFloat::UniformFloat(const std::string& name, float val, u32 index)
{
	this->uIndex = index;
	this->uName = name;
	this->uType = UNIFORM_TYPE::FLOAT;
	this->value = val;
}

void UniformFloat::Bind(ResourceShader* shader)
{
	shader->SetUniform1f(uName.c_str(), value);
}
