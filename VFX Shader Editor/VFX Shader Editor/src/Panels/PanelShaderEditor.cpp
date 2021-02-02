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
	flags = ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar;
}

PanelShaderEditor::~PanelShaderEditor()
{
}

void PanelShaderEditor::Draw()
{
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

		
	
	/*if(active)
		ImGui::OpenPopup("CreateShader");

	bool open = true;
	if (ImGui::BeginPopupModal("CreateShader",&open))
	{
		ImGui::Text("Choose a name: ");
		if (ImGui::Button("Ready!")) ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}*/

	if (this->current_shader == nullptr)
	{
		ImGui::Text("Select or create a shader");
		//if (ImGui::Button("Create")) ImGui::OpenPopup("CreateShader"); 

		////TEMPORAL
		//ResourceShader* shader = new ResourceShader("Shaders/default.vs", "Shaders/default.fs");
		//current_shader = shader;
		//App->renderer->current_shader = shader;

		ImGui::End();
		return;
	}

	//SELECT CURRENT_SHADER -> RESOURCE_SHADER -> GRAPH
	//GRAPH POINTER
	//RENDER GRAPH
	
	ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrollCoords.x, scrollCoords.y);
	ImVec2 win_pos2 = ImGui::GetCursorPos();
	ImGui::Text("CursorPos (%.2f,%.2f)", win_pos2.x, win_pos2.y);
	ImGui::SameLine(ImGui::GetWindowWidth() - 100);
	ImGui::Checkbox("Show grid", &grid);


	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 55, 200));

	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);
	//ImGui::PushItemWidth(120.0f);

	ImVec2 offset = { ImGui::GetCursorScreenPos().x + scrollCoords.x, ImGui::GetCursorScreenPos().y + scrollCoords.y };
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	if (grid)
	{
		ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
		float GRID_SZ = 16.0f;
		ImVec2 win_pos = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetWindowSize();
		for (float x = fmodf(scrollCoords.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
			draw_list->AddLine(ImVec2(x + win_pos.x, win_pos.y), ImVec2(x + win_pos.x, canvas_sz.y + win_pos.y), GRID_COLOR);
		for (float y = fmodf(scrollCoords.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
			draw_list->AddLine(ImVec2(win_pos.x, y + win_pos.y), ImVec2(canvas_sz.x + win_pos.x, y + win_pos.y), GRID_COLOR);
	}
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
		scrollCoords = { scrollCoords.x + ImGui::GetIO().MouseDelta.x, scrollCoords.y + ImGui::GetIO().MouseDelta.y };

	ImGui::MenuItem("hello");

	current_shader->graph->Draw();
	//ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);


	//Add new node
	if (ImGui::IsMouseClicked(1))
	{
		ImGui::OpenPopup("NewNode");
	}

	AddNewNodePopUp();

	ImGui::End();
	
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
	if (ImGui::BeginPopup("NewNode"))
	{
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

		
		NodeOption("PBR", NodeType::PBR, current_shader, current_shader->graph, &ShaderGraph::CreateNode);
		NodeOption("UV", NodeType::PBR, current_shader, current_shader->graph, &ShaderGraph::CreateNode);
		NodeOption("ColorRGB", NodeType::PBR, current_shader, current_shader->graph, &ShaderGraph::CreateNode);

		

		ImGui::EndPopup();
	}

}

void PanelShaderEditor::NodeOption(const char* name, NodeType type, ResourceShader* shader, ShaderGraph* graph, ShaderNode* (ShaderGraph::* p)(const char* n, int t))
{
	std::string node_name = name + std::string(" Node");

	if (node_name.find(node_filter) == std::string::npos)
		return;
	
	
	bool click_to_create_node = ImGui::Selectable(node_name.c_str());

	
	if (click_to_create_node)
	{
		//AddNode(*shader->graph, (graph->*p)(name, (int)type));
		AddNode(*shader->graph, std::invoke(p, graph, name, type));
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
		std::function<ShaderNode * (const char*, int)> create_node = std::bind(&ShaderGraph::CreateNode, current_shader->graph, name, type);
		AddNode(*shader->graph, create_node(name, type));
		ImGui::CloseCurrentPopup();
	}
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



