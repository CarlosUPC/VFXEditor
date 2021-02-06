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

void ShaderNode::Input(ShaderGraph& graph)
{
	if (NodeHovering(graph, this->position, this->size))
	{
		isHovered = true;
		if (ImGui::IsMouseClicked(0))
		{
			graph.selected = this;
		}
	}
	else
	{
		isHovered = false;
	}


	if (isHovered)
	{
		//static bool dragging = false;
		//float2 clickOffset;
		//
		////float2 mouseDelta = float2(ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y);
		//clickOffset = { ImGui::GetMousePos().x - position.x * graph.scale, ImGui::GetMousePos().y +  - position.y * graph.scale };
		//if (ImGui::IsMouseClicked(0))
		//{
		//	dragging = true;
		//}
		//if (dragging && ImGui::IsMouseReleased(0))
		//{
		//	dragging = false;
		//}
		//if (graph.selected == this && dragging && ImGui::IsMouseDragging(0))
		//{
		//	//this->position += mouseDelta;
		//	
		//	this->position += { ImGui::GetMousePos().x,  ImGui::GetMousePos().y };
		//	this->position /= graph.scale;
		//}
	}
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
	if (isHovered)
	{
		float border = 1.0f;
		draw_list->AddRect(ImVec2(m_Position.x - border, m_Position.y - border), ImVec2(m_Position.x + m_Size.x + border, m_Position.y + m_Size.y + border), ImColor(255, 150, 0), 5.0f);
	}


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

	if (g.selected == this && isHovered)
	{
		bool is_done = !ImGui::IsMouseDragging(ImGuiMouseButton_Left);
		float2 delta = float2(ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y);

		if (!is_done) {
			pos += delta;
		}
		else
		{
			this->position += delta / g.scale;

			/*pos = this->position;
			pos.x += ImGui::GetWindowPos().x + g.scrolling.x;
			pos.y += ImGui::GetWindowPos().y + g.scrolling.y;
			pos *= g.scale;*/


		}
	}
	return pos;
}

float2 ShaderNode::CalcNodeSize(ShaderGraph& graph, ShaderNode* node)
{
	float width = 200.0f;

	return graph.scale * float2(width, 120.0f); // TODO: height should be based on input length
}

bool ShaderNode::NodeHovering(ShaderGraph& graph, float2 position, float2 size)
{
	ImVec2 prev = ImGui::GetCurrentWindow()->DC.CursorPos;

	float2 m_Pos = CalcNodePosition(graph, position);
	float2 m_Size = CalcNodeSize(graph, this);

	ImGui::SetCursorScreenPos(ImVec2(m_Pos.x, m_Pos.y));
	ImGui::InvisibleButton("node", ImVec2(m_Size.x, m_Size.y));

	bool m_Hovered = ImGui::IsItemHovered();

	ImGui::SetCursorScreenPos(prev);

	return m_Hovered;
}
