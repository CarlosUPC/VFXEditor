#include "ShaderNode.h"
#include "ShaderGraph.h"
ShaderNode::ShaderNode()
{
}

ShaderNode::ShaderNode(const char* name, NodeType type, float2 position)
	: name(name), type(type), position(position)
{
	ImVec2 textSize = ImGui::CalcTextSize(name);
	title_size = { textSize.x, textSize.y };
}

ShaderNode::~ShaderNode()
{
}

void ShaderNode::Draw(ShaderGraph& graph)
{
	auto draw_list = ImGui::GetWindowDrawList();

	//Calculate Pos
	float2 m_Position = CalcNodePosition(graph, this->position);
	float2 m_Size = CalcNodeSize(graph, this);

	float2 m_TitleSize = title_size * graph.scale;
	//float2 m_Position = this->position;
	/*m_Position.x += ImGui::GetWindowPos().x + graph.scrolling.x;
	m_Position.y += ImGui::GetWindowPos().y + 50.0f + graph.scrolling.y;
	position = m_Position;*/
	auto color_red = ImGui::GetColorU32(ImVec4(0.85, 0.0, 0.0, 1.0));
	auto color_black = ImGui::GetColorU32(ImVec4(0.1, 0.1, 0.1, 0.45));

	
	//UI


	draw_list->AddRectFilled(ImVec2(m_Position.x, m_Position.y), ImVec2(m_Position.x + m_Size.x, m_Position.y + m_Size.y), ImColor(20, 20, 20, 180), 5.0f);
	draw_list->AddRectFilled(ImVec2(m_Position.x, m_Position.y), ImVec2(m_Position.x + m_Size.x, m_Position.y + 15.0f * graph.scale), ImColor(255, 0, 0), 5.0f);
	draw_list->AddRectFilled(ImVec2(m_Position.x, m_Position.y + 10.0f * graph.scale), ImVec2(m_Position.x + m_Size.x, m_Position.y + 20.0f * graph.scale), ImColor(255, 0, 0));


	float padding = 10.0f * graph.scale;

	ImGui::SetCursorScreenPos(ImVec2(m_Position.x + padding , m_Position.y + ((m_TitleSize.y)*0.5f) ));

	

	//draw_list->AddRectFilledMultiColorEx(ImVec2(m_Position.x, m_Position.y), ImVec2(m_Position.x + m_Size.x, m_Position.y + 15.0f * 1), color_red, color_red, color_black, color_black, 5.0f, ImDrawCornerFlags_::ImDrawCornerFlags_Top);
	//draw_list->AddRectFilledMultiColor(ImVec2(m_Position.x, m_Position.y + 10.0f * 1), ImVec2(m_Position.x + m_Size.x, m_Position.y + 20.0f * 1), color_red, color_black, color_red, color_red);
	//ImGui::SetCursorScreenPos(ImVec2(m_Position.x, m_Position.y));
}

void ShaderNode::InnerDraw(ShaderGraph& graph)
{
}

float2 ShaderNode::CalcNodePosition(ShaderGraph& g, float2 pos)
{
	pos.x += ImGui::GetWindowPos().x + g.scrolling.x;
	pos.y += ImGui::GetWindowPos().y + g.scrolling.y;
	pos *= g.scale;

	return pos;
}

float2 ShaderNode::CalcNodeSize(ShaderGraph& graph, ShaderNode* node)
{
	float width = 200.0f;

	return graph.scale * float2(width, 120.0f); // TODO: height should be based on input length
}
