#pragma once

#include "ShaderNode.h"

class UVNode : public ShaderNode
{
public:
	UVNode();
	UVNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value);

private:
	bool mirrorU = false;
	bool mirrorV = false;
	
	std::string uv_str;
	

};
