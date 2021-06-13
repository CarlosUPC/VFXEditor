#pragma once

#include "ResourceShader.h"

enum class UNIFORM_TYPE
{
	TEXTURE2D = 0,

	UNKNOWN
};


class ShaderUniform
{
public:
	ShaderUniform();
	~ShaderUniform();

	virtual void Bind(ResourceShader* shader) {};
	const std::string& GetName() const { return uName; }
	const uint& GetTextureID() const { return texID; }
protected:
	UNIFORM_TYPE uType = UNIFORM_TYPE::UNKNOWN;
	u32 uIndex;
	std::string uName;
	uint texID;

};

class UniformTexture : public ShaderUniform
{
public:
	UniformTexture(const std::string& name, uint texIdx, u32 index);
	void Bind(ResourceShader* shader) override;
private:
	//Texture tex;
};