#include "PBRNode.h"
#include "ShaderGraph.h"
PBRNode::PBRNode()
{
}

PBRNode::PBRNode(const char* name, NodeType type, float2 position)
	: ShaderNode(name, type, position)
{
}

void PBRNode::InnerDraw(ShaderGraph& g)
{

	//ImGui::SetCursorScreenPos(ImVec2(position.x , position.y));

	ImGui::BeginGroup();
	//ImGui::Dummy(ImVec2(0, 3 * g.scale));
	ImGui::Dummy(ImVec2(0, 40 * g.scale));
	ImGui::SameLine();
	ImGui::Text(name.c_str());
	ImGui::Dummy(ImVec2(0, 10 * g.scale));

	ImGui::EndGroup();
}
