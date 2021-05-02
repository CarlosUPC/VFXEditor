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


	if (graph.action.type == ActionGraph::NONE && action.type == ActionNode::NONE && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && NodeHovering(graph, this->position, this->size))
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

void ShaderNode::DrawNode(ShaderGraph& graph)
{
	auto draw_list = ImGui::GetWindowDrawList();

	//Update Position & Size
	float2 m_Position = CalcNodePosition(graph, this->position);
	float2 m_Size = CalcNodeSize(graph, this);
	size = m_Size; //temp

	DrawBody(draw_list, m_Position, m_Size);
	DrawTitle(draw_list, m_Position, m_Size);


	//float2 m_TitleSize = title_size * graph.scale;
	//float padding = 10.0f * graph.scale;

	//ImGui::SetCursorScreenPos(ImVec2(m_Position.x + padding , m_Position.y + ((m_TitleSize.y)*0.5f) ));

	

	//draw_list->AddRectFilledMultiColorEx(ImVec2(m_Position.x, m_Position.y), ImVec2(m_Position.x + m_Size.x, m_Position.y + 15.0f * 1), color_red, color_red, color_black, color_black, 5.0f, ImDrawCornerFlags_::ImDrawCornerFlags_Top);
	//draw_list->AddRectFilledMultiColor(ImVec2(m_Position.x, m_Position.y + 10.0f * 1), ImVec2(m_Position.x + m_Size.x, m_Position.y + 20.0f * 1), color_red, color_black, color_red, color_red);
	//ImGui::SetCursorScreenPos(ImVec2(m_Position.x, m_Position.y));
}

void ShaderNode::Update(ShaderGraph& graph)
{
}

float2 ShaderNode::CalcNodePosition(ShaderGraph& g, float2 pos)
{
	pos.x +=  g.scrolling.x;
	pos.y +=  g.scrolling.y;
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
		pos.x += /*ImGui::GetWindowPos().x +*/ g.scrolling.x;
		pos.y += /*ImGui::GetWindowPos().y +*/ g.scrolling.y;
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

	return float2(55.0f * node->inputs_count, 55.0f * node->inputs_count);

	//return graph.scale * float2(width, 80.0F + fmax(1.0f, inputs.size()*40.0f)); // TODO: height should be based on input length
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

bool ShaderNode::ConnectorHovering(float2 position, float2 size)
{
	ImVec2 previous = ImGui::GetCurrentWindow()->DC.CursorPos;

	ImGui::SetCursorScreenPos(ImVec2(position.x, position.y));
	ImGui::InvisibleButton("node", ImVec2(size.x, size.y));

	bool hovered = ImGui::IsItemHovered();

	ImGui::SetCursorScreenPos(previous);

	return hovered;
}

void ShaderNode::DrawTitle(ShaderGraph& g)
{

	//ImGui::Dummy(ImVec2(0, 3 * g.scale));
	//ImGui::Dummy(ImVec2(0, 40 * g.scale));
	//ImGui::SameLine();
	//ImGui::Text(name.c_str());
	//ImGui::Dummy(ImVec2(0, 10 * g.scale));
}

void ShaderNode::DrawTitle(ImDrawList* draw_list, float2 pos, float2 size)
{
	draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 15.0f /** graph.scale*/), ImColor(255, 0, 0), 5.0f);
	draw_list->AddRectFilled(ImVec2(pos.x, pos.y + 10.0f /** graph.scale*/), ImVec2(pos.x + size.x, pos.y + 20.0f /** graph.scale*/), ImColor(255, 0, 0));
	ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.0f, pos.y + 5.0f));
	ImGui::BeginGroup();
	ImGui::TextColored(ImVec4(255,255,255,255), "%s", name.c_str());
}

void ShaderNode::DrawBody(ImDrawList* draw_list, float2 pos, float2 size)
{

	auto color_red = ImGui::GetColorU32(ImVec4(0.85, 0.0, 0.0, 1.0));
	auto color_black = ImGui::GetColorU32(ImVec4(0.1, 0.1, 0.1, 0.45));

	//if (graph.hovered == this)
	//{
	//	float border = 1.0f;
	//	draw_list->AddRect(ImVec2(pos.x - border, pos.y - border), ImVec2(pos.x + size.x + border, pos.y + size.y + border), ImColor(255, 150, 0), 5.0f);
	//}


	draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(20, 20, 20, 180), 5.0f);
	
}

void ShaderNode::DrawInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset)
{
	for (unsigned int i = 0; i < numInputs; i++)
	{
		InputNode& input = this->inputs[i];

		
		DrawInputConnector(graph, input, i);


		//ImGui::PushID(this->UID);
		
		//ImGui::SetNextItemWidth(2 * graph.scale);
		//ImGui::Dummy(ImVec2(0, 40.0f * graph.scale));

		//ImGui::SameLine(30.0f);

		if (input.type == ValueType::FLOAT1)
		{
			//ImGui::SetNextItemWidth(100 * graph.scale);
			//float num = 2.0f;
			//ImGui::InputFloat(input.name.c_str(), &num);
			//ImGui::SetNextItemWidth(400 * graph.scale);
			//ImGui::SameLine(50.0f);
			//ImGui::Text(input.name.c_str());
		}
		else if (input.type == ValueType::FLOAT2)
		{
			//ImGui::SetNextItemWidth(100 * graph.scale);
			////ImGui::SameLine();
			//ImGui::InputFloat2(input.name.c_str(), &input.value2.x, "%.2f");
			//ImGui::SetNextItemWidth(400 * graph.scale);
			//ImGui::SameLine(50.0f);
			//ImGui::Text(input.name.c_str());
		}
		else if (input.type == ValueType::FLOAT3)
		{
			/*ImGui::SameLine();
			ImGui::InputFloat(input.name.c_str(), &input.value1);*/
			//ImGui::Text(input.name.c_str());
		}
		else if (input.type == ValueType::FLOAT4)
		{
			/*ImGui::InputFloat(input.name.c_str(), &input.value1);*/
			//ImGui::Text(input.name.c_str());
		}
		else
		{
			//ImGui::Text(input.name.c_str());
		}

		//ImGui::PopID();
	}
}

void ShaderNode::DrawOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset)
{
	for (unsigned int i = 0; i < numOutputs; i++)
	{
		OutputNode& output = this->outputs[i];

		//if(output.type != ValueType::NONE)
		DrawOutputConnector(graph, output);
	}
}

void ShaderNode::DrawInputConnector(ShaderGraph& graph, InputNode& input, unsigned int index )
{

	
	input.position = graph.scrolling + float2(this->position.x +  10.0f,
		this->position.y + this->size.y * (static_cast<float>(index) + 1) / (static_cast<float>(inputs_count) + 1));

	input.position.y += 10.0f;


	ImGui::SetCursorScreenPos(ImVec2(input.position.x + 15.0f, input.position.y - 8.0f));
	ImGui::TextColored(ImVec4(255, 255, 255, 255), "%s", input.name.c_str());


	auto draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImU32 fillColor = IM_COL32(100, 100, 105, 255);
	ImU32 outlineColor = IM_COL32(0, 200, 0, 255);
	draw_list->AddCircleFilled(ImVec2(input.position.x, input.position.y), 10.0f, fillColor, 16);
	draw_list->AddCircle(ImVec2(input.position.x, input.position.y), 10.0f, outlineColor);



	if (ConnectorHovering(input.position - float2(5.0f), float2(10.0f, 10.0f)))
	{
		draw_list->AddCircleFilled(ImVec2(input.position.x, input.position.y), 5.0f - 2.0f, outlineColor);

	}

	
	
	
	//
	//auto window = ImGui::GetCurrentWindow();
	//float2 connector_pos = float2(window->DC.CursorPos.x + (10.0f * graph.scale), window->DC.CursorPos.y + (7.5f * graph.scale));


	//
	//input.position = connector_pos;

	//


	//float2 hitbox_pos = float2(connector_pos.x - graph.scale * 20.0f , connector_pos.y - graph.scale * 20.0f);
	//float2 hitbox_size = float2(graph.scale * 40.0f);

	//

	//if (graph.action.type == ActionGraph::ActionType::NONE )
	//{
	//	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ConnectorHovering(hitbox_pos, hitbox_size))
	//	{
	//		
	//		graph.action.type = ActionGraph::ActionType::DRAG_CONNECTOR;
	//		//start linking
	//		input.connector.to = this;
	//		//input.connector.from = nullptr;

	//		graph.action.connector = &input.connector;
	//		graph.action.connector->index_in = index;

	//	}
	//}

	// if (graph.action.type == ActionGraph::ActionType::DRAG_CONNECTOR )
	//{
	//	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ConnectorHovering(hitbox_pos, hitbox_size))
	//	{
	//		
	//		//connect linking
	//		Connector* current_connector = graph.action.connector;

	//		if (current_connector->to != nullptr) return;
	//		if (this == graph.action.connector->from) return;

	//		current_connector->to = this;
	//		current_connector->index_in = index;

	//		//graph.action.type = ActionGraph::ActionType::RELEASE_CONNECTOR;
	//	}
	//}

	// if (graph.action.type == ActionGraph::ActionType::RELEASE_CONNECTOR )
	//{
	//	if (ConnectorHovering( hitbox_pos, hitbox_size))
	//	{
	//		
	//		//finish linking
	//		Connector* current_connector = graph.action.connector;

	//		//if (link->to == nullptr || link->from == nullptr) return;

	//		current_connector->to = this;
	//		current_connector->index_in = index;


	//		input.connector = *current_connector;

	//		for (uint i = 0; i < current_connector->from->outputs.size(); i++)
	//		{
	//			if (&current_connector->from->outputs[i].connector == current_connector)
	//			{
	//				current_connector->from->outputs[i].connector.to = nullptr;
	//				current_connector->from->outputs[i].connector.from = nullptr;
	//				break;
	//			}
	//		}
	//		

	//		graph.action.type = ActionGraph::ActionType::NONE;
	//	}
	//}


}

void ShaderNode::DrawOutputConnector(ShaderGraph& graph, OutputNode& output, unsigned int index)
{

	
	output.position = graph.scrolling + float2(this->position.x + this->size.x - 10.0f,
								               this->position.y + this->size.y * (static_cast<float>(index) + 1) / (static_cast<float>(outputs_count) + 1));

	output.position.y += 10.0f;


	ImGui::SetCursorScreenPos(ImVec2(output.position.x - 60.0f, output.position.y - 8.0f));
	ImGui::TextColored(ImVec4(255,255,255,255), "%s", output.name.c_str());
	
	
	auto draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImU32 fillColor = IM_COL32(100, 100, 105, 255);
	ImU32 outlineColor = IM_COL32(0, 200, 0, 255);
	draw_list->AddCircleFilled(ImVec2(output.position.x, output.position.y), 10.0f, fillColor, 16);
	draw_list->AddCircle(ImVec2(output.position.x, output.position.y), 10.0f, outlineColor);



	if (ConnectorHovering(output.position - float2(5.0f), float2(10.0f, 10.0f)))
	{
		draw_list->AddCircleFilled(ImVec2(output.position.x, output.position.y), 5.0f - 2.0f, outlineColor);

	}





	//float2 hitbox_pos = float2(connector_pos.x - graph.scale * 20.0f, connector_pos.y - graph.scale * 20.0f);
	//float2 hitbox_size = float2(graph.scale * 40.0f);




	//

	//if (graph.action.type == ActionGraph::ActionType::NONE )
	//{
	//	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ConnectorHovering( hitbox_pos, hitbox_size))
	//	{
	//		
	//		//start linking
	//		//output.connector.to = nullptr;
	//		output.connector.from = this;

	//		graph.action.connector = &output.connector;
	//		graph.action.connector->index_out = index;

	//		graph.action.type = ActionGraph::ActionType::DRAG_CONNECTOR;
	//	}
	//}
	// if (graph.action.type == ActionGraph::ActionType::DRAG_CONNECTOR)
	//{
	//	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ConnectorHovering( hitbox_pos, hitbox_size))
	//	{
	//		
	//		//connect linking
	//		Connector* current_connector = graph.action.connector;

	//		if (current_connector->from != nullptr) return;
	//		if (this == graph.action.connector->to) return;

	//		current_connector->from = this;
	//		current_connector->index_out = index;

	//		//graph.action.type = ActionGraph::ActionType::RELEASE_CONNECTOR;
	//	}
	//}
	// if (graph.action.type == ActionGraph::ActionType::RELEASE_CONNECTOR )
	//{
	//	if (ConnectorHovering( hitbox_pos, hitbox_size))
	//	{
	//		
	//		//finish linking

	//		//Nothing to do, an input connector can not be bound to a output connector

	//		graph.action.type = ActionGraph::ActionType::NONE;
	//	}
	//}

}

void Connector::DrawConnector(ShaderGraph& g, bool isInput)
{
	

	if (g.action.type == ActionGraph::ActionType::DRAG_CONNECTOR && g.action.connector == this)
	{
		if(ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			ShaderNode* node_output = this->from;
			ShaderNode* node_input = this->to;

			if (node_input && node_output)
			{
				//render complete
				float2 end = float2(this->to->inputs[index_in].position);
				float2 start = float2(this->from->outputs[index_out].position);

				this->AddBezierLine(g, start, end, true);

				if (isInput) this->from = nullptr;
				else this->to = nullptr;
			}
			else
			{
				//render incomplete
				float2 start_pos;

				if (node_output != nullptr) start_pos = float2(this->from->outputs[index_out].position);
				else if (node_input != nullptr) start_pos = float2(this->to->inputs[index_in].position);

				this->AddBezierLine(g, start_pos, float2(ImGui::GetMousePos().x, ImGui::GetMousePos().y), false);
			}
		}
		else
		{
			g.action.type = ActionGraph::ActionType::RELEASE_CONNECTOR;
		}
	}
	else if(g.action.type == ActionGraph::ActionType::RELEASE_CONNECTOR && g.action.connector == this)
	{
		g.action.type = ActionGraph::ActionType::NONE;
	}
	else
	{
		//drawing connection between nodes
		ShaderNode* node_output = this->from;
		ShaderNode* node_input = this->to;

		if (node_input && node_output)
		{
			//render complete
			float2 end = float2(this->to->inputs[index_in].position);
			float2 start = float2(this->from->outputs[index_out].position);

			this->AddBezierLine(g, start, end, true);

			//if (node_output != nullptr) start_pos = float2(this->from->outputs[index].position);
			//else if (node_input != nullptr) start_pos = float2(this->to->inputs[index].position);
		}

	}


}

void Connector::DrawInputChannel(ShaderGraph& g, InputNode& input)
{
	auto draw_list = ImGui::GetCurrentWindow()->DrawList;

	float thickness = 1 * g.scale;
	ImU32 fillColor = IM_COL32(100, 100, 105, 255);
	ImU32 outlineColor = IM_COL32(0, 200, 0, 255);

	draw_list->AddCircleFilled(ImVec2(input.position.x, input.position.y), 10.0f * g.scale, fillColor, 16);
	draw_list->AddCircle(ImVec2(input.position.x, input.position.y), 10.0f * g.scale, outlineColor, 16, thickness);


	float2 hitbox_pos = float2(input.position.x - g.scale * 20.0f, input.position.y - g.scale * 20.0f);
	float2 hitbox_size = float2(g.scale * 40.0f);

	if (ConnectorHovering(hitbox_pos, hitbox_size))
	{
		g.hovered = nullptr;
	}
}

void Connector::DrawOutputChannel(ShaderGraph& g, OutputNode& output)
{
	auto draw_list = ImGui::GetCurrentWindow()->DrawList;

	float thickness = 1 * g.scale;
	ImU32 fillColor = IM_COL32(100, 100, 105, 255);
	ImU32 outlineColor = IM_COL32(0, 200, 0, 255);

	draw_list->AddCircleFilled(ImVec2(output.position.x, output.position.y), 10.0f * g.scale, fillColor, 16);
	draw_list->AddCircle(ImVec2(output.position.x, output.position.y), 10.0f * g.scale, outlineColor, 16, thickness);

	float2 hitbox_pos = float2(output.position.x - g.scale * 20.0f, output.position.y - g.scale * 20.0f);
	float2 hitbox_size = float2(g.scale * 40.0f);

	if (ConnectorHovering(hitbox_pos, hitbox_size))
	{
		g.hovered = nullptr;
	}
}

void Connector::AddBezierLine(ShaderGraph& g, float2 start, float2 end, bool isLinked)
{
	float2 v_start = start;
	float2 v_end = end;
	//float2 v_start(start.x, start.y);
	//float2 v_end(end.x, end.y);

	float bezier = 200.0f;

	//float length = start.x - end.x;
	float length = math::Length(float2(start.x, end.x));

	length = math::Min(length * 0.5f, 50.0f * g.scale);

	float2 cp0, cp1;

	if (!isLinked && start.x > end.x) {
		std::swap(v_start, v_end);
		std::swap(start, end);
	}

	if (v_start.x > v_end.x && false) {
		cp0 = v_start - float2(length, 0);
		cp1 = v_end + float2(length, 0);
	}
	else {
		cp0 = v_start + float2(length, 0);
		cp1 = v_end - float2(length, 0);
	}

	//ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(start.x, start.y), ImVec2(cp0.x, cp0.y), ImColor(120, 120, 120), g.scale * 5.f);
	//ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(cp0.x, cp0.y), ImVec2(cp1.x, cp1.y), ImColor(120, 120, 120), g.scale * 5.f);
	//ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(cp1.x, cp1.y), ImVec2(end.x, end.y), ImColor(120, 120, 120), g.scale * 5.f);

	ImGui::GetCurrentWindow()->DrawList->AddBezierCurve(ImVec2(start.x, start.y), ImVec2(cp0.x, cp0.y), ImVec2(cp1.x, cp1.y), ImVec2(end.x, end.y), ImColor(120, 120, 120), g.scale * 5.f);

}

bool Connector::ConnectorHovering(float2 position, float2 size)
{
	ImVec2 previous = ImGui::GetCurrentWindow()->DC.CursorPos;

	ImGui::SetCursorScreenPos(ImVec2(position.x, position.y));
	ImGui::InvisibleButton("node", ImVec2(size.x, size.y));

	bool hovered = ImGui::IsItemHovered();

	ImGui::SetCursorScreenPos(previous);

	return hovered;
}
