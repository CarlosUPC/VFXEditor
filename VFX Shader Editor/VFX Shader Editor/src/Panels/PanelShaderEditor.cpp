#include "Application.h"
#include "PanelShaderEditor.h"
#include "ResourceShader.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleResources.h"
#include "ShaderGraph.h"
#include "ShaderNode.h"

PanelShaderEditor::PanelShaderEditor(const char* name)
	:Panel(name)
{
	flags = ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;

	
}

PanelShaderEditor::~PanelShaderEditor()
{
}

void PanelShaderEditor::Draw()
{
	
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 55, 200) );
	ImGui::Begin(name.c_str(), 0, flags);
	
	static bool p_open = false;
	static std::string menu_action = "";


	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("Asset")) {

		if (ImGui::MenuItem("New Shader"))
			creating_shader = true;
		if (ImGui::MenuItem("Load Shader"))
			selecting_shader = true;
		if (ImGui::MenuItem("Save Shader")) {}

		
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	
	
	
	

	if (creating_shader)
		CreateNewShaderPopUp();
	if(selecting_shader)
		LoadShaderPopUp();

		
	
	
	if (this->current_shader == nullptr)
	{
		ImGui::Text("Select or create a shader");
		

		////TEMPORAL
		//ResourceShader* shader = new ResourceShader("Shaders/default.vs", "Shaders/default.fs");
		//current_shader = shader;
		//App->renderer->current_shader = shader;

		ImGui::End();
		ImGui::PopStyleColor();

		return;
	}

	//INIT CANVAS
	canvas.Init(ImGui::GetCursorScreenPos(), ImGui::GetWindowSize(), IM_COL32(0, 200, 120, 120));


	//SELECT CURRENT_SHADER -> RESOURCE_SHADER -> GRAPH
	//GRAPH POINTER
	//RENDER GRAPH
	
	/*
	ImVec2 win_pos4 = ImGui::GetWindowPos();
	ImGui::Text("WindowPos (%.2f,%.2f)", win_pos4.x, win_pos4.y);
	ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrollCoords.x, scrollCoords.y);
	ImVec2 win_pos2 = ImGui::GetCursorPos();
	ImVec2 win_pos3 = ImGui::GetCursorScreenPos();
	ImGui::Text("CursorPos (%.2f,%.2f)", win_pos2.x, win_pos2.y);
	ImGui::Text("CursorScreenPos (%.2f,%.2f)", win_pos3.x, win_pos3.y);
	ImGui::SameLine(ImGui::GetWindowWidth() - 100);
	ImGui::Checkbox("Show grid", &grid);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.25f);
	ImGui::Text("Zoom x %.1f", current_shader->graph->scale);
	ImGui::PopStyleVar();
	float num = 1.0f;
	ImGui::InputFloat("test", &num);
	*///ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	//ImGui::BeginChild("scrolling_region", ImVec2(0, 0)/*, true*//*, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse*/);
	//ImGui::PushItemWidth(120.0f);

	/*float wheel = ImGui::GetIO().MouseWheel;
	
	static float zoom = current_shader->graph->scale;
	if (wheel > 0 && ImGui::IsWindowHovered())
	{
		zoom *= (1.0 + wheel * 0.2f);

		float new_scale = zoom;
		new_scale = Clamp(new_scale, 0.1f, 2.0f);
		new_scale = ceilf(new_scale * 8) / 8;

		float2 mousePos = { ImGui::GetMousePos().x , ImGui::GetMousePos().y  };

		float2 relative_mouse = mousePos;
		relative_mouse /= current_shader->graph->scale;

		relative_mouse.x -= ImGui::GetWindowPos().x + current_shader->graph->scrolling.x;
		relative_mouse.y -= ImGui::GetWindowPos().y + current_shader->graph->scrolling.y;

		

		if (new_scale != current_shader->graph->scale) {
			current_shader->graph->scale = new_scale;


			float2 relative_mouse2 = mousePos;
			relative_mouse2 /= current_shader->graph->scale;

			relative_mouse2.x -= ImGui::GetWindowPos().x + current_shader->graph->scrolling.x;
			relative_mouse2.y -= ImGui::GetWindowPos().y + current_shader->graph->scrolling.y;

			current_shader->graph->scrolling += relative_mouse2 - relative_mouse;

		}
	}
	else if (wheel < 0 && ImGui::IsWindowHovered())
	{
		
		zoom /= (1.0 - wheel * 0.2f);

		float new_scale = zoom;

		new_scale = Clamp(new_scale, 0.1f, 2.0f);
		new_scale = ceilf(new_scale * 8) / 8;

		float2 mousePos = { ImGui::GetMousePos().x , ImGui::GetMousePos().y };
		
		float2 relative_mouse = mousePos;

		relative_mouse /= current_shader->graph->scale;
		relative_mouse.x -= ImGui::GetWindowPos().x + current_shader->graph->scrolling.x;
		relative_mouse.y -= ImGui::GetWindowPos().y + current_shader->graph->scrolling.y;



		if (new_scale != current_shader->graph->scale) {
			current_shader->graph->scale = new_scale;

			float2 relative_mouse2 = mousePos;

			relative_mouse2 /= current_shader->graph->scale;
			relative_mouse2.x -= ImGui::GetWindowPos().x + current_shader->graph->scrolling.x;
			relative_mouse2.y -= ImGui::GetWindowPos().y + current_shader->graph->scrolling.y;

			current_shader->graph->scrolling += relative_mouse2 - relative_mouse;

		}

	}*/

	//float2 offset = {  canvas.m_Scroll.x,  canvas.m_Scroll.y };
	
	//Canvas Stuff
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	canvas.Draw(draw_list, ImGui::GetCursorScreenPos(), ImGui::GetWindowSize());


	//Mouse Stuff
	Scrolling();
	NodeContext();


	//Graph Stuff
	current_shader->graph->Draw();
	

	
	ImGui::End();
	ImGui::PopStyleColor();
	
}

void PanelShaderEditor::OnShaderAction(std::string& action)
{
	if (action == "New")
	{

	}
	else if (action == "Load")
	{
		/*ImVec2 size = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetCursorScreenPos();*/
		//ImVec2 view = ImVec2(pos.x + (size.x / 2.0f) - ImGui::GetWindowWidth()/2.0f ,pos.y +  (size.y / 2.0f) - ImGui::GetWindowHeight() / 2.0f);
		ImGui::OpenPopup("Load");
		//static bool act = true;
		//ImGui::Begin(name.c_str(), &act,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysVerticalScrollbar);

		ImVec2 view = ImVec2(App->window->width/2.0f - 150.0f, App->window->height /2.0f - 120.0f);
		ImGui::SetNextWindowSize({300,200});
		ImGui::SetNextWindowPos(view);

		if (ImGui::BeginPopupModal("Load", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Select the first scene for the build");
			ImGui::Spacing();
			ImGui::BeginChild("#BuildChildSceneSelector", { 0,0 }, true);
			bool exists = false;
			ImGui::Selectable(std::string("Shader1").data(), exists, ImGuiSelectableFlags_AllowItemOverlap);
			ImGui::Selectable(std::string("Shader2").data(), exists, ImGuiSelectableFlags_AllowItemOverlap);
			ImGui::Selectable(std::string("Shader3").data(), exists, ImGuiSelectableFlags_AllowItemOverlap);
			ImGui::Selectable(std::string("Shader4").data(), exists);

			if (ImGui::Selectable(std::string("None").data()))
			{
				action = "";
				ImGui::CloseCurrentPopup();
			}

			//if (ImGui::IsItemClicked()) {
			//	//action = "";
			//	act = false;
			//	//ImGui::CloseCurrentPopup();
			//}
			ImGui::SameLine();
			/*ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5F - ImGui::CalcTextSize("hey").x * 0.5F);
			ImGui::Text("tu puta madre")*/;

			ImGui::EndChild();
			ImGui::Spacing();

			ImGui::EndPopup();
			
		}
		
	}
}

void PanelShaderEditor::CreateNewShaderPopUp()
{

	static char _name[MAX_PATH] = "Data Name";
	

	ImGui::OpenPopup("Create New Shader");
	ImGui::SetNextWindowSize({ 320,155 });
	if (ImGui::BeginPopupModal("Create New Shader", &creating_shader, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::PushItemWidth(235);
		
		
		ImGui::Spacing();
		ImGui::SetCursorPosX(30);
		ImGui::InputText("Name", _name, MAX_PATH, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		
		ImGui::Spacing();
		ImGui::SetCursorPosX(42);
		ImGui::Text("Shader will be generated,");
		ImGui::SetCursorPosX(33);
		ImGui::Text("just select it to open graph.");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SetCursorPosX(90);

		if (ImGui::Button("Create Shader", { 130,25 })) {
			if (std::string(_name).find(" ") == std::string::npos) {
		
				//TODO: Create resource and storage it in the list
				ResourceShader* shader = App->resources->CreateShader(_name);
				//TODO: set it as current shader resource to display the graph
				current_shader = shader;
				App->renderer->current_shader = current_shader;
				strcpy_s(_name, "Data Name");
				creating_shader = false;
				
			}
			
		}

		ImGui::EndPopup();
	}
	else
	{
		strcpy_s(_name, "Data Name");
		creating_shader = false;
	}
}

void PanelShaderEditor::LoadShaderPopUp()
{
	ImGui::OpenPopup("Load Shader");
	ImGui::SetNextWindowSize({ 320,155 });
	if (ImGui::BeginPopupModal("Load Shader", &selecting_shader, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::PushItemWidth(235);


		ImGui::Text("Select a shader");
		ImGui::Spacing();
		ImGui::BeginChild("#BuildChildSceneSelector", { 0,0 }, true);
		bool exists = false;
		ImGui::Selectable(std::string("Shader1").data(), exists, ImGuiSelectableFlags_AllowItemOverlap);
		ImGui::Selectable(std::string("Shader2").data(), exists, ImGuiSelectableFlags_AllowItemOverlap);
		ImGui::Selectable(std::string("Shader3").data(), exists, ImGuiSelectableFlags_AllowItemOverlap);
		

		if (ImGui::Selectable(std::string("None").data()))
		{
			selecting_shader = false;
			//TODO: set it as current shader resource to display the graph
		}

		
		ImGui::EndChild();
		

		ImGui::EndPopup();
	}
	else
	{
		
		selecting_shader = false;
	}
}

void PanelShaderEditor::AddNewNodePopUp()
{
	//ImVec2 win_pos = ImGui::GetWindowPos();

	ImVec2 hit_pos = ImGui::GetIO().MousePos;
	
	if (ImGui::BeginPopup("NewNode"))
	{
		//ImVec2 pos3 = ImGui::GetWindowPos();
		//ShaderNode* new_node = nullptr;
		//current_shader->graph->nodes.push_back(new_node);
		//CreateNodeFn p = &ShaderGraph::CreateNode;

		ImGui::PushItemWidth(100.0f);
		ImGui::InputText("##filter", node_filter,64);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		
		if (ImGui::Selectable("X", false, ImGuiSelectableFlags_DontClosePopups, { 8,13 }))
		{
			memcpy(&node_filter, "\0", 1);
		}

		//ImVec2 pos = ImGui::GetWindowPos();

		//pos.x /= current_shader->graph->scale;
		//pos.y /= current_shader->graph->scale;
		//pos.x -= win_pos.x + current_shader->graph->scrolling.x;
		//pos.y -= win_pos.y + current_shader->graph->scrolling.y;


		ImVec2 node_pos;
		node_pos.x = hit_pos.x - canvas.m_Scroll.x;
		node_pos.y = hit_pos.y - canvas.m_Scroll.y;


		NodeOption("PBR", NodeType::PBR, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph, &ShaderGraph::CreateNode);
		NodeOption("UV", NodeType::PBR, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph, &ShaderGraph::CreateNode);
		NodeOption("ColorRGB", NodeType::PBR, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph, &ShaderGraph::CreateNode);

		

		ImGui::EndPopup();
	}

}

void PanelShaderEditor::NodeOption(const char* name, NodeType type,float2 position, ResourceShader* shader, ShaderGraph* graph, ShaderNode* (ShaderGraph::* p)(const char* n, int t, float2 pos))
{
	std::string node_name = name + std::string(" Node");

	if (node_name.find(node_filter) == std::string::npos)
		return;
	
	
	bool click_to_create_node = ImGui::Selectable(node_name.c_str());

	
	if (click_to_create_node)
	{
		//AddNode(*shader->graph, (graph->*p)(name, (int)type));
		AddNode(*shader->graph, std::invoke(p, graph, name, type, position));
		ImGui::CloseCurrentPopup();
	}
}

void PanelShaderEditor::NodeOption(const char* name, NodeType type, ResourceShader* shader, ShaderGraph* graph)
{
	std::string node_name = name;
	std::string format = "New " + node_name + " Node";
	bool click_to_create_node = ImGui::Button(format.c_str());

	if (click_to_create_node)
	{
		//std::function<ShaderNode * (const char*, int)> create_node = std::bind(&ShaderGraph::CreateNode, current_shader->graph, name, type);
		//AddNode(*shader->graph, create_node(name, type));
		ImGui::CloseCurrentPopup();
	}
}

void PanelShaderEditor::Scrolling()
{
	if (ImGui::IsWindowHovered() && 
		!ImGui::IsAnyItemActive() && 
		ImGui::IsMouseDragging(2, 0.0f))
	{
		
		canvas.m_Scroll =
		{
			canvas.m_Scroll.x + ImGui::GetIO().MouseDelta.x,
			canvas.m_Scroll.y + ImGui::GetIO().MouseDelta.y
		};

		//canvas.m_Scroll.x = math::Clamp(canvas.m_Scroll.x, -1000.0f, 1000.0f);
		//canvas.m_Scroll.y = math::Clamp(canvas.m_Scroll.y, -1000.0f, 1000.0f);
		
		//Set ScrollOffset to graph
		current_shader->graph->SetScrollOffset(canvas.m_Scroll);

	}
}

void PanelShaderEditor::NodeContext()
{
	if (ImGui::IsMouseClicked(1) &&
		!ImGui::IsMouseClicked(0) &&
		ImGui::IsWindowHovered())
	{
		if(!ImGui::IsAnyItemHovered())
			ImGui::OpenPopup("NewNode");
	}

	AddNewNodePopUp();
}


void PanelShaderEditor::AddNode(ShaderGraph& graph, ShaderNode* node)
{
	graph.nodes.push_back(node);
}

ShaderNode PanelShaderEditor::CreateNode()
{
	ShaderNode node;
	return node;
}

void Canvas::Init(ImVec2 m_Pos, ImVec2 m_Size, ImColor m_Color)
{
	this->m_Pos = m_Pos;
	this->m_Size = m_Size;
	this->m_Color = m_Color;
}

void Canvas::Draw(ImDrawList* draw_list, ImVec2 m_Pos, ImVec2 m_Size)
{
	this->m_Pos = m_Pos;
	this->m_Size = m_Size;
	

	if (this->show_grid)
	{
		//ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 200));

		for (float x = fmodf(m_Scroll.x, grid_size); x < m_Size.x; x += grid_size)
			draw_list->AddLine(ImVec2(x + m_Pos.x, m_Pos.y), ImVec2(x + m_Pos.x, m_Size.y + m_Pos.y), m_Color);
		for (float y = fmodf(m_Scroll.y, grid_size); y < m_Size.y; y += grid_size)
			draw_list->AddLine(ImVec2(m_Pos.x, y + m_Pos.y), ImVec2(m_Size.x + m_Pos.x, y + m_Pos.y), m_Color);

		//ImGui::PopStyleColor();

	}
}
