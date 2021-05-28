#pragma once


#include "ShaderNode.h"

class FloatNode : public ShaderNode
{
public:
	FloatNode();
	FloatNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate() override;

private:
	float output;
	std::vector<std::string> input_code;
	std::vector<std::string> output_code;
};


class Vector2Node : public ShaderNode
{
public:
	Vector2Node();
	Vector2Node(const char* name, NODE_TYPE type, float2 position);
	
	void Update(ShaderGraph& graph) override;
	void InspectorUpdate() override;
	

	
private:
	
};