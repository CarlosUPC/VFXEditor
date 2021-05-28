#include "ShaderNode.h"
#include "ShaderGraph.h"
#include "Random.h"
ShaderNode::ShaderNode()
{
}

ShaderNode::ShaderNode(const char* name, NODE_TYPE type, float2 position)
	: name(name), type(type), position(position)
{
	UID = Random::GenerateUUID();
	ImVec2 textSize = ImGui::CalcTextSize(name);
	title_size = { textSize.x, textSize.y };
}

ShaderNode::~ShaderNode()
{
}

void ShaderNode::InputNode(ShaderGraph& graph)
{
	
	//Node is being used to link another one
	if (graph.socket_state.node_selected == this)
		return;

	//Hover node ----
	if (NodeHovering(graph, this->position, this->size))
	{
		graph.node_hovered = this;
	}
	else
	{
		graph.node_hovered = nullptr;
	}


	//Set node hovered -----
	if (graph.node_hovered != nullptr)
	{
		this->isHovered = true;
	}
	else
	{
		this->isHovered = false;
	}

	//Select node -----
	isItemActive = ImGui::IsItemActive();
	if (ImGui::IsMouseDoubleClicked(0) &&
		isItemActive)
	{
		graph.node_selected = this;
	}

	//Set node selected -----
	if (graph.node_selected != nullptr && graph.node_selected == this)
	{
		this->isSelected = true;
		this->isHovered = false;
	}
	else
	{
		this->isSelected = false;
	}

	//Drag node -----
	if (isItemActive && ImGui::IsMouseDragging(0) )
	{
		this->position = this->position + float2(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);
		
	}

	//Remove node -----
	if (this->isHovered && ImGui::IsMouseClicked(1))
	{
		ImGui::OpenPopup("delete_node");
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 12));
	//ImGui::SetNextWindowPos(ImGui::GetMousePos());
	if (ImGui::BeginPopup("delete_node"))
	{
		if (ImGui::MenuItem("Delete"))
		{
			this->to_delete = true;
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

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

void ShaderNode::DrawLines(ShaderGraph& graph)
{
	auto draw_list = ImGui::GetWindowDrawList();

	//for (auto& link : this->links)
	//{
	//	float2 input_pos = graph.scrolling + link.input_node->inputs[link.input_socket].position;
	//	float2 output_pos = graph.scrolling + link.output_node->outputs[link.output_socket].position;

	//	if (this == link.input_node)
	//	{
	//		//input_pos = graph.scrolling + this->inputs[link.input_socket].position;
	//		link.output_node->outputs[link.output_socket].isLinked = true;
	//	}
	//	else if(this == link.output_node)
	//	{
	//		//output_pos = graph.scrolling + this->outputs[link.output_socket].position;
	//		link.input_node->inputs[link.input_socket].isLinked = true;
	//	}

	//	if (!this->inputs[link.input_socket].isLinked || !this->outputs[link.output_socket].isLinked)
	//	{

	//		draw_list->AddBezierCurve
	//		(
	//			ImVec2(output_pos.x, output_pos.y), 
	//			ImVec2(output_pos.x + 80.0f, output_pos.y), 
	//			ImVec2(input_pos.x - 80.0f, input_pos.y), 
	//			ImVec2(input_pos.x, input_pos.y), 
	//			IM_COL32(0, 150, 250, 250),3,12
	//		);


	//	}
	//}

	if (graph.socket_state.node_selected == this)
	{
		draw_list->AddBezierCurve
		(
			ImVec2(graph.socket_state.socket_pos.x, graph.socket_state.socket_pos.y),
			ImVec2(graph.socket_state.socket_pos.x + 80.0f, graph.socket_state.socket_pos.y),
			ImGui::GetIO().MousePos,
			ImGui::GetIO().MousePos,
			IM_COL32(0, 150, 250, 250), 4
		);

		if (ImGui::IsMouseReleased(0))
		{
			graph.socket_state.input_socket_actived = false;
			graph.socket_state.output_socket_actived = false;
			graph.socket_state.node_selected = nullptr;
		}
	}


}




void ShaderNode::Update(ShaderGraph& graph)
{
}

void ShaderNode::InspectorUpdate()
{
}

float2 ShaderNode::CalcNodePosition(ShaderGraph& g, float2 pos)
{
	pos.x += g.scrolling.x;
	pos.y += g.scrolling.y;
	pos *= g.scale;


	


	
	return pos;
}

float2 ShaderNode::CalcNodeSize(ShaderGraph& graph, ShaderNode* node)
{
	float width = 200.0f;

	return float2(55.0f * node->inputs_size, 55.0f * node->inputs_size);

	//return graph.scale * float2(width, 80.0F + fmax(1.0f, inputs.size()*40.0f)); // TODO: height should be based on input length
}

bool ShaderNode::NodeHovering(ShaderGraph& graph, float2 position, float2 size)
{
	//ImVec2 prev = ImGui::GetCurrentWindow()->DC.CursorPos;

	float2 m_Pos = CalcNodePosition(graph, position);
	float2 m_Size = CalcNodeSize(graph, this);

	ImGui::SetCursorScreenPos(ImVec2(m_Pos.x, m_Pos.y));
	ImGui::InvisibleButton("node", ImVec2(m_Size.x, m_Size.y));

	bool m_Hovered = ImGui::IsItemHovered();

	//ImGui::SetCursorScreenPos(prev);

	return m_Hovered;
}

bool ShaderNode::ConnectorHovering(float2 position, float2 size)
{
	//ImVec2 previous = ImGui::GetCurrentWindow()->DC.CursorPos;

	ImGui::SetCursorScreenPos(ImVec2(position.x, position.y));
	ImGui::InvisibleButton("node", ImVec2(size.x, size.y));

	bool hovered = ImGui::IsItemHovered();

	//ImGui::SetCursorScreenPos(previous);

	return hovered;
}

bool ShaderNode::SocketHovering(float2& point1, float2 point2, const float r1, const float r2)
{
	float2 distance = float2(point2.x - point1.x, point2.y - point1.y);
	float length = distance.x * distance.x + distance.y * distance.y;
	float radius = (r1 + r2) * (r1 + r2);

	if (length < radius)
	{
		return true;
	}

	return false;
}

void ShaderNode::DrawTitle(ShaderGraph& g)
{

	
}

void ShaderNode::DrawTitle(ImDrawList* draw_list, float2 pos, float2 size)
{
	draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 15.0f /** graph.scale*/), ImColor(255, 0, 0), 5.0f);
	draw_list->AddRectFilled(ImVec2(pos.x, pos.y + 10.0f /** graph.scale*/), ImVec2(pos.x + size.x, pos.y + 20.0f /** graph.scale*/), ImColor(255, 0, 0));
	ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.0f, pos.y + 5.0f));
	//ImGui::BeginGroup();
	ImGui::TextColored(ImVec4(255,255,255,255), "%s", name.c_str());
}

void ShaderNode::DrawBody(ImDrawList* draw_list, float2 pos, float2 size)
{

	auto color_red = ImGui::GetColorU32(ImVec4(0.85, 0.0, 0.0, 1.0));
	auto color_black = ImGui::GetColorU32(ImVec4(0.1, 0.1, 0.1, 0.45));

	if (this->isHovered && !this->isSelected)
	{
		float border = 1.0f;
		draw_list->AddRect(ImVec2(pos.x - border, pos.y - border), ImVec2(pos.x + size.x + border, pos.y + size.y + border), ImColor(255, 150, 0), 5.0f);
	}
	else if (this->isSelected)
	{
		float border = 1.0f;
		draw_list->AddRect(ImVec2(pos.x - border, pos.y - border), ImVec2(pos.x + size.x + border, pos.y + size.y + border), ImColor(255, 0, 0), 5.0f);
	}


	draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(20, 20, 20, 180), 5.0f);
	
}

void ShaderNode::DrawInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset)
{
	for (unsigned int i = 0; i < numInputs; i++)
	{
		InputSocket& input = this->inputs[i];

		if(input.context_type != CONTEXT_TYPE::PARAMETER)
			DrawInputConnector(graph, input, i);


		//ImGui::PushID(this->UID);
		
		//ImGui::SetNextItemWidth(2 * graph.scale);
		//ImGui::Dummy(ImVec2(0, 40.0f * graph.scale));

		//ImGui::SameLine(30.0f);

		//if (input.type == VALUE_TYPE::FLOAT1)
		//{
		//	//ImGui::SetNextItemWidth(100 * graph.scale);
		//	//float num = 2.0f;
		//	//ImGui::InputFloat(input.name.c_str(), &num);
		//	//ImGui::SetNextItemWidth(400 * graph.scale);
		//	//ImGui::SameLine(50.0f);
		//	//ImGui::Text(input.name.c_str());
		//}
		//else if (input.type == VALUE_TYPE::FLOAT2)
		//{
		//	//ImGui::SetNextItemWidth(100 * graph.scale);
		//	////ImGui::SameLine();
		//	//ImGui::InputFloat2(input.name.c_str(), &input.value2.x, "%.2f");
		//	//ImGui::SetNextItemWidth(400 * graph.scale);
		//	//ImGui::SameLine(50.0f);
		//	//ImGui::Text(input.name.c_str());
		//}
		//else if (input.type == VALUE_TYPE::FLOAT3)
		//{
		//	/*ImGui::SameLine();
		//	ImGui::InputFloat(input.name.c_str(), &input.value1);*/
		//	//ImGui::Text(input.name.c_str());
		//}
		//else if (input.type == VALUE_TYPE::FLOAT4)
		//{
		//	/*ImGui::InputFloat(input.name.c_str(), &input.value1);*/
		//	//ImGui::Text(input.name.c_str());
		//}
		//else
		//{
		//	//ImGui::Text(input.name.c_str());
		//}

		//ImGui::PopID();
	}
}

void ShaderNode::DrawOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset)
{
	for (unsigned int i = 0; i < numOutputs; i++)
	{
		OutputSocket& output = this->outputs[i];

		//if(output.type != ValueType::NONE)
		DrawOutputConnector(graph, output);
	}
}



void ShaderNode::DrawInputConnector(ShaderGraph& graph, InputSocket& input, unsigned int index )
{

	
	input.position = graph.scrolling + float2(this->position.x +  10.0f,
		this->position.y + this->size.y * (static_cast<float>(index) + 1) / (static_cast<float>(inputs_size) + 1));

	input.position.y += 10.0f;


	ImGui::SetCursorScreenPos(ImVec2(input.position.x + 15.0f, input.position.y - 8.0f));
	ImGui::TextColored(ImVec4(255, 255, 255, 255), "%s", input.name.c_str());


	auto draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImU32 fillColor = IM_COL32(100, 100, 105, 255);
	ImU32 outlineColor = IM_COL32(0, 200, 0, 255);
	draw_list->AddCircleFilled(ImVec2(input.position.x, input.position.y), 10.0f, fillColor, 16);
	draw_list->AddCircle(ImVec2(input.position.x, input.position.y), 10.0f, outlineColor);



	if (SocketHovering(input.position, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f))
	{
		//draw_list->AddCircleFilled(ImVec2(input.position.x, input.position.y), 5.0f - 2.0f, outlineColor);
		draw_list->AddCircle(ImVec2(input.position.x, input.position.y), 10.0f, ImColor(255, 150, 0));

	}
	if (input.isLinked)
	{
		draw_list->AddCircleFilled(ImVec2(input.position.x, input.position.y), 10.0f - 3.0f, IM_COL32(0, 200, 0, 255));
	}

	
	
	
	


}

void ShaderNode::DrawOutputConnector(ShaderGraph& graph, OutputSocket& output, unsigned int index)
{

	
	output.position = graph.scrolling + float2(this->position.x + this->size.x - 10.0f,
								               this->position.y + this->size.y * (static_cast<float>(index) + 1) / (static_cast<float>(outputs_size) + 1));

	output.position.y += 10.0f;


	ImGui::SetCursorScreenPos(ImVec2(output.position.x - 60.0f, output.position.y - 8.0f));
	ImGui::TextColored(ImVec4(255,255,255,255), "%s", output.name.c_str());
	
	
	auto draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImU32 fillColor = IM_COL32(100, 100, 105, 255);
	ImU32 outlineColor = IM_COL32(0, 200, 0, 255);
	draw_list->AddCircleFilled(ImVec2(output.position.x, output.position.y), 10.0f, fillColor, 16);
	draw_list->AddCircle(ImVec2(output.position.x, output.position.y), 10.0f, outlineColor);



	if (SocketHovering(output.position, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f))
	{
		draw_list->AddCircle(ImVec2(output.position.x, output.position.y), 10.0f, ImColor(255, 150, 0));

	}
	if (output.isLinked)
	{
		draw_list->AddCircleFilled(ImVec2(output.position.x, output.position.y), 10.0f - 3.0f, IM_COL32(0, 200, 0, 255));
	}





	//float2 hitbox_pos = float2(connector_pos.x - graph.scale * 20.0f, connector_pos.y - graph.scale * 20.0f);
	//float2 hitbox_size = float2(graph.scale * 40.0f);




	

}

void ShaderNode::InputSocketInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset)
{

	for (int i = 0; i < numInputs; ++i)
	{
		InputSocket& input = this->inputs[i];

		if (input.context_type == CONTEXT_TYPE::PARAMETER)
			break;


		float2 input_pos = input.position;

		if (SocketHovering(input_pos, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f))
		{
			this->isHovered = false;
		}
		

		//IF OUTPUT WAS NOT ACTIVATED
		if (!graph.socket_state.output_socket_actived &&
			ImGui::IsMouseDown(0) &&
			SocketHovering(input_pos, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f))
		{

			if (ImGui::GetIO().KeyAlt)
			{
				if (input.isLinked)
				{
					//swap links
					if (input.link_ref != nullptr)
					{
						input.link_ref->to_delete = true;
						//input.isLinked = false;
					}

				break;
				}

			}


			graph.socket_state.node_selected = this;
			graph.socket_state.socked_selected = i;
			graph.socket_state.input_socket_actived = true;
			graph.socket_state.socket_pos = input_pos;

			break;

		}



			//IF OUTPUT WAS ACTIVATED
		if (graph.socket_state.output_socket_actived &&
			ImGui::IsMouseReleased(0) &&
			SocketHovering(input_pos, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f)
			)
		{

				if (graph.socket_state.node_selected != this) // this checks if we are trying to put the link in the same node and we dont want that happens
				{

					if (input.isLinked)
					{
						//swap links
						if (input.link_ref != nullptr)
						{
							input.link_ref->to_delete = true;
							input.isLinked = false;
						}

					}
					
					//create link
					input.link_ref = new ShaderLink(this, i, graph.socket_state.node_selected, graph.socket_state.socked_selected);
					graph.socket_state.node_selected->outputs[graph.socket_state.socked_selected].link_ref = input.link_ref;
					graph.links.push_back(input.link_ref);
					

				}

				graph.socket_state.node_selected = nullptr;
				graph.socket_state.socked_selected = 0;
				graph.socket_state.output_socket_actived = false;
				graph.socket_state.input_socket_actived = false;
				graph.socket_state.socket_pos = float2(0.0f);
				break;
			

		}

		

	}
}

void ShaderNode::InputSocketOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset)
{

	for (int i = 0; i < numOutputs; ++i)
	{
		OutputSocket& output = this->outputs[i];

		float2 output_pos = output.position;

		if (SocketHovering(output_pos, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f))
		{
			this->isHovered = false;
		}
		


		//IF INPUT WAS NOT ACTIVATED
		if (!graph.socket_state.input_socket_actived &&
			ImGui::IsMouseDown(0) &&
			SocketHovering(output_pos, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f))
		{

			if (ImGui::GetIO().KeyAlt)
			{
				if (output.isLinked) //TODO: el output si va apoder conectarse con muchos inputs debe tener una array de conexiones y no esto...
				{
					//swap links
					if (output.link_ref != nullptr)
					{
						output.link_ref->to_delete = true;
						//input.isLinked = false;
					}

					break;
				}

			}

			graph.socket_state.node_selected = this;
			graph.socket_state.socked_selected = i;
			graph.socket_state.output_socket_actived = true;
			graph.socket_state.socket_pos = output_pos;

			break;

		}

		//IF INPUT WAS ACTIVATED
		if (graph.socket_state.input_socket_actived &&
			ImGui::IsMouseReleased(0) &&
			SocketHovering(output_pos, float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), 10.0f, 2.0f)
			)
		{

			if (graph.socket_state.node_selected != this) // this checks if we are trying to put the link in the same node and we dont want that happens
			{

				if (graph.socket_state.node_selected->inputs[graph.socket_state.socked_selected].isLinked)
				{
					//swap links
					if (graph.socket_state.node_selected->inputs[graph.socket_state.socked_selected].link_ref != nullptr)
					{
						graph.socket_state.node_selected->inputs[graph.socket_state.socked_selected].link_ref->to_delete = true;
						graph.socket_state.node_selected->inputs[graph.socket_state.socked_selected].isLinked = false;
					}

				}

				//create link
				output.link_ref = new ShaderLink(graph.socket_state.node_selected, graph.socket_state.socked_selected, this, i);
				graph.socket_state.node_selected->inputs[graph.socket_state.socked_selected].link_ref = output.link_ref;
				graph.links.push_back(output.link_ref);


			}

			graph.socket_state.node_selected = nullptr;
			graph.socket_state.socked_selected = 0;
			graph.socket_state.input_socket_actived = false;
			graph.socket_state.output_socket_actived = false;
			graph.socket_state.socket_pos = float2(0.0);
			break;

		}
	}
}

InputSocket ShaderNode::GetInputSocketbyName(const std::string& inputName)
{

	for (int i = 0; i < inputs.size(); ++i)
	{
		InputSocket& input = this->inputs[i];

		if (input.name.compare(inputName) == 0)
		{
			return input;
		}

	}
	
}







//void Connector::AddBezierLine(ShaderGraph& g, float2 start, float2 end, bool isLinked)
//{
//	float2 v_start = start;
//	float2 v_end = end;
//	//float2 v_start(start.x, start.y);
//	//float2 v_end(end.x, end.y);
//
//	float bezier = 200.0f;
//
//	//float length = start.x - end.x;
//	float length = math::Length(float2(start.x, end.x));
//
//	length = math::Min(length * 0.5f, 50.0f * g.scale);
//
//	float2 cp0, cp1;
//
//	if (!isLinked && start.x > end.x) {
//		std::swap(v_start, v_end);
//		std::swap(start, end);
//	}
//
//	if (v_start.x > v_end.x && false) {
//		cp0 = v_start - float2(length, 0);
//		cp1 = v_end + float2(length, 0);
//	}
//	else {
//		cp0 = v_start + float2(length, 0);
//		cp1 = v_end - float2(length, 0);
//	}
//
//	//ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(start.x, start.y), ImVec2(cp0.x, cp0.y), ImColor(120, 120, 120), g.scale * 5.f);
//	//ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(cp0.x, cp0.y), ImVec2(cp1.x, cp1.y), ImColor(120, 120, 120), g.scale * 5.f);
//	//ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(cp1.x, cp1.y), ImVec2(end.x, end.y), ImColor(120, 120, 120), g.scale * 5.f);
//
//	ImGui::GetCurrentWindow()->DrawList->AddBezierCurve(ImVec2(start.x, start.y), ImVec2(cp0.x, cp0.y), ImVec2(cp1.x, cp1.y), ImVec2(end.x, end.y), ImColor(120, 120, 120), g.scale * 5.f);
//
//}



ShaderLink::ShaderLink(ShaderNode* input_node, unsigned int input_socket, ShaderNode* output_node, unsigned int output_socket)
	: input_node(input_node), input_socket(input_socket), output_node(output_node), output_socket(output_socket)
{
	UID = Random::GenerateUUID();
}

void ShaderLink::DrawLink(ShaderGraph& graph)
{

	auto draw_list = ImGui::GetWindowDrawList();

	if (this->input_node != nullptr && this->output_node != nullptr)
	{
	
		//get positions
		float2 input_pos = this->input_node->inputs[this->input_socket].position;
		float2 output_pos = this->output_node->outputs[this->output_socket].position;

		//Put sockets in linked state
		if (!input_node->inputs[this->input_socket].isLinked || !output_node->outputs[this->output_socket].isLinked)
		{
			this->input_node->inputs[this->input_socket].isLinked = true;
			this->output_node->outputs[this->output_socket].isLinked = true;
		}

		//draw line
		if (this->isLineHovered)
		{
			draw_list->AddBezierCurve(
				ImVec2(output_pos.x, output_pos.y),
				ImVec2(output_pos.x + 80.0f, output_pos.y),
				ImVec2(input_pos.x - 80.0f, input_pos.y),
				ImVec2(input_pos.x, input_pos.y),
				IM_COL32(255, 150, 0, 255),
				7.0f, 12.0f
			);

		}


		draw_list->AddBezierCurve
		(
			ImVec2(output_pos.x, output_pos.y),
			ImVec2(output_pos.x + 80.0f, output_pos.y),
			ImVec2(input_pos.x - 80.0f, input_pos.y),
			ImVec2(input_pos.x, input_pos.y),
			IM_COL32(0, 150, 250, 250), 3, 12
		);



	}


}

void ShaderLink::InputLink(ShaderGraph& graph)
{
	auto draw_list = ImGui::GetWindowDrawList();

	if (this->input_node != nullptr && this->output_node != nullptr)
	{
		//get positions
		float2 input_pos = this->input_node->inputs[this->input_socket].position;
		float2 output_pos =  this->output_node->outputs[this->output_socket].position;

		ImVec2 link_pos = ImBezierClosestPoint(
			ImVec2(output_pos.x, output_pos.y), 
			ImVec2(output_pos.x + 80.0f, output_pos.y), 
			ImVec2(input_pos.x - 80.0f, input_pos.y), 
			ImVec2(input_pos.x, input_pos.y),  
			ImGui::GetIO().MousePos, 12.0f);


		if (LineHovering(float2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y), float2(link_pos.x, link_pos.y), 8.0f, 0.0f))
		{
			/*draw_list->AddBezierCurve(
				ImVec2(output_pos.x, output_pos.y),
				ImVec2(output_pos.x + 80.0f, output_pos.y),
				ImVec2(input_pos.x - 80.0f, input_pos.y),
				ImVec2(input_pos.x, input_pos.y),
				IM_COL32(255, 150, 0, 255),
				7.0f, 12.0f
			);*/

			graph.link_hovered = this;
		}
		else
		{
			graph.link_hovered = nullptr;
		}


		//Set node hovered -----
		if (graph.link_hovered == this)
		{
			this->isLineHovered = true;
		}
		else
		{
			this->isLineHovered = false;
		}

		if (this->isLineHovered && ImGui::IsMouseClicked(1))
		{
			
			ImGui::OpenPopup("delete_line");
		}

		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 12));
		//ImGui::SetNextWindowPos(ImGui::GetMousePos());
		if (ImGui::BeginPopup("delete_line"))
		{
			
			if (ImGui::MenuItem("Delete"))
			{
				this->to_delete = true;
				
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
		

		
	}
	





}

bool ShaderLink::LineHovering(float2 p1, float2 p2, const float r1, const float r2)
{
	float2 distance = float2(p2.x - p1.x, p2.y - p1.y);
	float length = distance.x * distance.x + distance.y * distance.y;
	float radius = (r1 + r2) * (r1 + r2);

	if (length < radius)
	{
		return true;
	}

	return false;
}

void InputSocket::DisplayInputSocketDetails()
{

	if (type == VALUE_TYPE::FLOAT1)
	{
		ImGui::Text("value: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value1, 0.1f, 0.0f, 9999999.0f, "%.2f");
	}
	else if (type == VALUE_TYPE::FLOAT2)
	{
		ImGui::PushID("##X");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value2.x, 0.1f, 0.0f, 9999999.0f, "%.2f");
		ImGui::PopID();

		ImGui::PushID("##Y");
		ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value2.y, 0.1f, 0.0f, 9999999.0f, "%.2f");
		ImGui::PopID();

	}
	else if (type == VALUE_TYPE::FLOAT3)
	{
		ImGui::PushID("##X");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value3.x, 0.1f, 0.0f, 9999999.0f, "%.2f");
		ImGui::PopID();

		ImGui::PushID("##Y");
		ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value3.y, 0.1f, 0.0f, 9999999.0f, "%.2f");
		ImGui::PopID();

		ImGui::PushID("##Z");
		ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value3.z, 0.1f, 0.0f, 9999999.0f, "%.2f");
		ImGui::PopID();
	}
	else if (type == VALUE_TYPE::FLOAT4)
	{
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value3.x, 0.1f, 0.0f, 9999999.0f, "%.2f");

		ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value3.y, 0.1f, 0.0f, 9999999.0f, "%.2f");

		ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value3.z, 0.1f, 0.0f, 9999999.0f, "%.2f");

		ImGui::Text("W: "); ImGui::SameLine();
		ImGui::DragFloat(name.c_str(), &value4.w, 0.1f, 0.0f, 9999999.0f, "%.2f");
	}

}
