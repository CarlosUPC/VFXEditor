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
	

protected:
	UNIFORM_TYPE uType = UNIFORM_TYPE::UNKNOWN;
	u32 uIndex;
	std::string uName;

};

class UniformTexture : public ShaderUniform
{
public:
	UniformTexture(const std::string& name, uint texIdx, u32 index);
	void Bind(ResourceShader* shader) override;

	const uint& GetTextureID() const { return texID; }
	void SetTextureID(uint id) { texID = id; }
private:
	uint texID;
	//Texture tex;
};