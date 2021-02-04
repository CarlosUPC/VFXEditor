#include "PBRNode.h"

PBRNode::PBRNode()
{
}

PBRNode::PBRNode(const char* name, NodeType type, float2 position)
	: ShaderNode(name, type, position)
{
}

void PBRNode::InnerDraw()
{

	//ImGui::SetCursorScreenPos(ImVec2(position.x , position.y));

	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(0, 3 * 1));
	ImGui::Dummy(ImVec2(0, 40 * 1));
	ImGui::SameLine();
	ImGui::Text(name.c_str());
	ImGui::Dummy(ImVec2(0, 10 * 1));

	ImGui::EndGroup();
}
