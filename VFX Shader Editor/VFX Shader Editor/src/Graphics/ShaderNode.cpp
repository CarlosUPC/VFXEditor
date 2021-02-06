#include "ShaderNode.h"
#include "ShaderGraph.h"
#include "Random.h"
ShaderNode::ShaderNode()
{
}

ShaderNode::ShaderNode(const char* name, NodeType type, float2 position)
	: name(name), type(type), position(position)
{
	UID = Random::GenerateUUID();
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
		graph.hovered = this;
		if (ImGui::IsMouseClicked(0))
		{
			graph.selected = this;
		}
	}
	else
	{
		isHovered = false;
		//graph.hovered = nullptr;
	}


	if (action.type == ActionNode::NONE && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && NodeHovering(graph, this->position, this->size))
		action.type = ActionNode::DRAG_NODE;



	//if (isHovered)
	//{
	//	static bool dragging = false;
	//	float2 clickOffset;
	//	
	//	//float2 mouseDelta = float2(ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y);
	//	clickOffset = { ImGui::GetMousePos().x - position.x * graph.scale, ImGui::GetMousePos().y +  - position.y * graph.scale };
	//	if (ImGui::IsMouseClicked(0))
	//	{
	//		dragging = true;
	//	}
	//	if (dragging && ImGui::IsMouseReleased(0))
	//	{
	//		dragging = false;
	//	}
	//	if (graph.selected == this && dragging && ImGui::IsMouseDragging(0))
	//	{
	//		//this->position += mouseDelta;
	//		
	//		this->position = { ImGui::GetWindowPos().x - ImGui::GetMousePos().x , ImGui::GetWindowPos().y - ImGui::GetMousePos().y  };
	//		this->position /= graph.scale;
	//	}
	//}
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
	if (graph.hovered == this)
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


	if (action.type == ActionNode::DRAG_NODE)
	{
		bool is_done = !ImGui::IsMouseDragging(ImGuiMouseButton_Left);
		float2 delta = float2(ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y);

		if (!is_done) {
			pos += delta;
		}
		else
		{
			action.mouse_pos = delta;
			action.type = ActionNode::APPLY_DRAG;
		}
	}

	if (action.type == ActionNode::APPLY_DRAG)
	{
		this->position += action.mouse_pos / g.scale;

		pos = this->position;
		pos.x += ImGui::GetWindowPos().x + g.scrolling.x;
		pos.y += ImGui::GetWindowPos().y + g.scrolling.y;
		pos *= g.scale;

		action.type = ActionNode::NONE;
	}


	//if (g.selected == this && g.hovered)
	//{
	//	bool is_done = !ImGui::IsMouseDragging(ImGuiMouseButton_Left);
	//	float2 delta = float2(ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y);
	//	float2 mouse_pos;

	//	if (!is_done) {
	//		pos += delta;
	//	}
	//	else
	//	{
	//		 mouse_pos = delta;
	//		//this->position += mouse_pos;
	//		//this->position /= g.scale;

	//		//pos = this->position;
	//		//pos.x += /*ImGui::GetWindowPos().x +*/ g.scrolling.x;
	//		//pos.y += /*ImGui::GetWindowPos().y +*/ g.scrolling.y;
	//		//pos *= g.scale;


	//	}
	//	this->position += mouse_pos;
	//	this->position /= g.scale;
	//	pos = this->position;
	//	pos.x += ImGui::GetWindowPos().x + g.scrolling.x;
	//	pos.y += ImGui::GetWindowPos().y + g.scrolling.y;
	//	pos *= g.scale;

	//}
	return pos;
}

float2 ShaderNode::CalcNodeSize(ShaderGraph& graph, ShaderNode* node)
{
	float width = 200.0f;

	return graph.scale * float2(width, 80.0F + fmax(1.0f, inputs.size()*40.0f)); // TODO: height should be based on input length
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

void ShaderNode::DrawTitle(ShaderGraph& g)
{
	//ImGui::Dummy(ImVec2(0, 3 * g.scale));
	ImGui::Dummy(ImVec2(0, 40 * g.scale));
	ImGui::SameLine();
	ImGui::Text(name.c_str());
	//ImGui::Dummy(ImVec2(0, 10 * g.scale));
}

void ShaderNode::DrawInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset)
{
	for (unsigned int i = 0; i < numInputs; i++)
	{
		InputNode& input = this->inputs[i];

		ImGui::PushID(this->UID);
		//ImGui::SetNextItemWidth(100 * graph.scale);
		//ImGui::Dummy(ImVec2(0, 10.0f * graph.scale));

		//ImGui::SameLine();

		if (input.type == ValueType::FLOAT1)
		{
			ImGui::SetNextItemWidth(100 * graph.scale);
			float num = 2.0f;
			ImGui::InputFloat(input.name.c_str(), &num);
		}
		else if (input.type == ValueType::FLOAT2)
		{
			ImGui::SetNextItemWidth(100 * graph.scale);
			//ImGui::SameLine();
			ImGui::InputFloat2(input.name.c_str(), &input.value2.x, "%.2f");
		}
		else if (input.type == ValueType::FLOAT3)
		{
			ImGui::SameLine();
			ImGui::InputFloat(input.name.c_str(), &input.value1);
		}
		else if (input.type == ValueType::FLOAT4)
		{
			ImGui::InputFloat(input.name.c_str(), &input.value1);
		}
		else
		{
			ImGui::Text(name.c_str());
		}

		ImGui::PopID();
	}
}
