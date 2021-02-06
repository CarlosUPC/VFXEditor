#pragma once

#include "ShaderNode.h"

class PBRNode : public ShaderNode
{
public:
	PBRNode();
	PBRNode(const char* name, NodeType type, float2 position);

	void InnerDraw(ShaderGraph& graph) override;
};