#include "PBRNode.h"
#include "ShaderGraph.h"
PBRNode::PBRNode()
{
}



PBRNode::PBRNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("Albedo", VALUE_TYPE::FLOAT3));
	inputs.push_back(InputSocket("Normal", VALUE_TYPE::FLOAT3, CONTEXT_TYPE::READ_ONLY));
	inputs.push_back(InputSocket("Emission", VALUE_TYPE::FLOAT3, CONTEXT_TYPE::READ_ONLY));
	inputs.push_back(InputSocket("Metallic", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::READ_ONLY));
	inputs.push_back(InputSocket("Specular", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::READ_ONLY));
	inputs.push_back(InputSocket("Roughness", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::READ_ONLY));

	//outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	//temp hardcoded
	inputs_size = 6;
	outputs_size = 1;
}

void PBRNode::Update(ShaderGraph& g)
{
}
