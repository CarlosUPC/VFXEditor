#pragma once

#include "ShaderNode.h"

//=========================================================================================================================
// [NODE] PARALLAX OCLUSION NODE
//=========================================================================================================================
class ParallaxOclusionNode : public ShaderNode
{
public:
	ParallaxOclusionNode();
	ParallaxOclusionNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& uv, const std::string& amplitude, const std::string& min_layers, const std::string& max_layers);

private:


};