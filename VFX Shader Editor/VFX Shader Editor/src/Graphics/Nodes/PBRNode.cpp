#include "PBRNode.h"
#include "ShaderGraph.h"
PBRNode::PBRNode()
{
}



PBRNode::PBRNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("diffuse", VALUE_TYPE::FLOAT2));
	inputs.push_back(InputSocket("metallic", VALUE_TYPE::FLOAT1));
	inputs.push_back(InputSocket("roughness", VALUE_TYPE::FLOAT1));
	
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	//temp hardcoded
	inputs_size = 3;
	outputs_size = 1;
}

void PBRNode::Update(ShaderGraph& g)
{
}
