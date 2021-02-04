#include "ShaderNode.h"
#include "ShaderGraph.h"
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

void ShaderNode::Draw(ShaderGraph& graph)
{
	auto draw_list = ImGui::GetWindowDrawList();

	//Calculate Pos
	float2 m_Position = CalcNodePosition(graph, this->position);
	float2 m_Size = float2(200.0f,120.0f);
	/*m_Position.x += ImGui::GetWindowPos().x + graph.scrolling.x;
	m_Position.y += ImGui::GetWindowPos().y + 50.0f + graph.scrolling.y;
	position = m_Position;*/

	//UI
	draw_list->AddRectFilled(ImVec2(m_Position.x, m_Position.y), ImVec2(m_Position.x + m_Size.x, m_Position.y + m_Size.y), ImColor(20, 20, 20, 180), 5.0f);
	draw_list->AddRectFilled(ImVec2(m_Position.x, m_Position.y), ImVec2(m_Position.x + m_Size.x, m_Position.y + 15.0f * 1), ImColor(255, 0, 0), 5.0f);
	draw_list->AddRectFilled(ImVec2(m_Position.x, m_Position.y + 10.0f *1), ImVec2(m_Position.x + m_Size.x, m_Position.y + 20.0f * 1), ImColor(255, 0, 0));

	ImGui::SetCursorScreenPos(ImVec2(m_Position.x, m_Position.y));
}

void ShaderNode::InnerDraw()
{
}

float2 ShaderNode::CalcNodePosition(ShaderGraph& g, float2 pos)
{
	pos.x += ImGui::GetWindowPos().x + g.scrolling.x;
	pos.y += ImGui::GetWindowPos().y + g.scrolling.y;
	return pos;
}
