#pragma once

#include "ShaderNode.h"

//=========================================================================================================================
// [NODE] UNLIT NODE
//=========================================================================================================================
class PBRNode : public ShaderNode
{
public:
	PBRNode();
	PBRNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	virtual void InspectorUpdate(ShaderGraph& graph) override;

public:
	
};