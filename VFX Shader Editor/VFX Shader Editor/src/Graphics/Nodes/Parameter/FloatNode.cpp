#include "FloatNode.h"

FloatNode::FloatNode()
{
}

FloatNode::FloatNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	//temp hardcoded
	inputs_count = 1.2;
	outputs_count = 1;

}

void FloatNode::Update(ShaderGraph& graph)
{
}

void FloatNode::InspectorUpdate()
{
	ImGui::DragFloat("value", &output, 0.1f, 0.0f, 1.0f, "%.2f");
	
}
