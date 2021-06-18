#pragma once

#include "ResourceShader.h"

enum class UNIFORM_TYPE
{
	TEXTURE2D = 0,
	FLOAT,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	UNKNOWN
};


class ShaderUniform
{
public:
	ShaderUniform();
	~ShaderUniform();

	virtual void Bind(ResourceShader* shader) {};
	virtual void Unbind(ResourceShader* shader) {};
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
	virtual void Bind(ResourceShader* shader) override;
	virtual void Unbind(ResourceShader* shader) override;

	const uint& GetTextureID() const { return texID; }
	void SetTextureID(uint id) { texID = id; }
private:
	uint texID;
	//Texture tex;
};

class UniformFloat : public ShaderUniform
{
public:
	UniformFloat(const std::string& name, float val, u32 index);
	virtual void Bind(ResourceShader* shader) override;

	const float& GetValue() const { return value; }
	void SetValue(float val) { value = val; }
private:
	float value;
	//Texture tex;
};