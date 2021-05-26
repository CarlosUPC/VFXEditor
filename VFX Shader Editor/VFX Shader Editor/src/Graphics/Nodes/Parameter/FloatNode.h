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
};