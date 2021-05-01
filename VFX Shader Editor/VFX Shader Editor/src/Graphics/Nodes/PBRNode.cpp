#include "PBRNode.h"
#include "ShaderGraph.h"
PBRNode::PBRNode()
{
}

PBRNode::PBRNode(const char* name, NodeType type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputNode("diffuse", ValueType::FLOAT2));
	inputs.push_back(InputNode("metallic", ValueType::FLOAT1));
	inputs.push_back(InputNode("roughness", ValueType::FLOAT1));
	
	outputs.push_back(OutputNode(ValueType::FLOAT1));
}

void PBRNode::InnerDraw(ShaderGraph& g)
{

	ImGui::BeginGroup();
	
	//DrawTitle(g);
	//DrawOutputs(g, 1);
	//DrawInputs(g, 3);

	ImGui::EndGroup();
}
