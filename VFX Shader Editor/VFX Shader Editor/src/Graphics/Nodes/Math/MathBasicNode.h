#pragma once

#include "ShaderNode.h"

class MultiplyNode : public ShaderNode
{
public:
	MultiplyNode();
	MultiplyNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	//std::string GLSLAbstactVarFloat(const std::string& value, const std::string& name);

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2);
private:
	float output;
	std::vector<std::string> input_code;
	std::vector<std::string> output_code;
};
