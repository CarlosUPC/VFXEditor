#include "ShaderNode.h"

ShaderNode::ShaderNode()
{
}

ShaderNode::ShaderNode(const char* name, NodeType type, float2 position)
	: name(name), type(type), position(position)
{

}

ShaderNode::~ShaderNode()
{
}

void ShaderNode::Draw()
{
	auto draw_list = ImGui::GetWindowDrawList();

	//Calculate Pos

	//UI
	draw_list->AddRectFilled(ImVec2(position.x, position.y), ImVec2(position.x +50.0f, position.y + 50.0f), ImColor(80, 80, 80), 5.0f);
	draw_list->AddRectFilled(ImVec2(position.x, position.y), ImVec2(position.x + 50.0f, position.y + 35.0f * 1), ImColor(40, 40, 40), 5.0f);
	draw_list->AddRectFilled(ImVec2(position.x, position.y + 30.0f *2), ImVec2(position.x + 50.0f, position.y + 40.0f * 2), ImColor(40, 40, 40));

}

void ShaderNode::InnerDraw()
{
}
