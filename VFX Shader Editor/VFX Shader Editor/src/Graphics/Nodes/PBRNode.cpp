#include "PBRNode.h"
#include "ShaderGraph.h"
PBRNode::PBRNode()
{
}

PBRNode::PBRNode(const char* name, NodeType type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("diffuse", ValueType::FLOAT2));
	inputs.push_back(InputSocket("metallic", ValueType::FLOAT1));
	inputs.push_back(InputSocket("roughness", ValueType::FLOAT1));
	
	outputs.push_back(OutputSocket(ValueType::FLOAT1));

	//temp hardcoded
	inputs_count = 3;
	outputs_count = 1;
}

void PBRNode::Update(ShaderGraph& g)
{
}
