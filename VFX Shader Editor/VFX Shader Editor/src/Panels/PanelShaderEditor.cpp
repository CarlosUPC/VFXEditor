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
	flags = ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar  | ImGuiWindowFlags_NoScrollWithMouse;
	ImGuiIO& io = ImGui::GetIO();
	mainFont = io.Fonts->AddFontDefault();
	textFont = io.Fonts->AddFontFromFileTTF("Fonts/PixelForce.TTF", 22);
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


	// Asset bar -----
	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("Asset")) {

		if (ImGui::MenuItem("New Shader"))
		{ 
			creating_shader = true;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Export")) {

		if (ImGui::MenuItem("Export to Unity"))
		{
			if (this->current_shader != nullptr)
				current_shader->graph->ExportShader(current_shader);

		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	
	

	if (creating_shader)
		CreateShaderContext();
	if(selecting_shader)
		LoadShaderContext();

		
	
	
	if (this->current_shader == nullptr)
	{
		ImVec2 textSize = ImGui::CalcTextSize("Graph empty. Create a Shader");
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetWindowSize().x - (textSize.x) - 120.0f, ImGui::GetWindowSize().y/2.0f));
		ImGui::PushFont(textFont);
		ImGui::Text("Graph empty. Generate a Shader!");
		ImGui::PopFont();
		ImGui::End();
		ImGui::PopStyleColor();
		return;
	}


	//Shader info
	ImGui::SetNextItemWidth(128);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_TitleBg]);
	
	ImGui::PushID((void*)&current_shader->id);
	ImGui::InputText("##", (char*)current_shader->name.c_str(), MAX_PATH);
	ImGui::PopID();

	ImGui::PopStyleColor();

	ImGui::SameLine();
	if (ImGui::Button("Compile")) {
		current_shader->graph->CompileShader(current_shader);
	}

	
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(45, 45, 50, 255));
	ImGui::BeginChild("Canvas", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);

	//INIT CANVAS
	canvas.Init(ImGui::GetCursorScreenPos(), ImGui::GetWindowSize(), IM_COL32(0, 200, 120, 120));

	
	//Canvas Stuff
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	canvas.Draw(draw_list, ImGui::GetCursorScreenPos(), ImGui::GetWindowSize());


	//Mouse Stuff
	Scrolling();
	NodeContext();


	//Graph Stuff
	current_shader->graph->Update(App->GetDT());
	
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	
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
			/*ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5F - ImGui::CalcTextSize("hey").x * 0.5F);*/;

			ImGui::EndChild();
			ImGui::Spacing();

			ImGui::EndPopup();
			
		}
		
	}
}

void PanelShaderEditor::CreateShaderContext()
{

	static char _name[MAX_PATH] = "Data Name";
	

	ImGui::OpenPopup("Create New Shader");
	ImGui::SetNextWindowSize({ 320,155 });
	//ImGui::SetNextWindowPos({ float(App->window->width)/2.0f  - 200.0f, float(App->window->height) / 2.0f - 20.0f });
	if (ImGui::BeginPopupModal("Create New Shader", &creating_shader, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::PushItemWidth(235);
		
		
		ImGui::Spacing();
		ImGui::SetCursorPosX(30);
		ImGui::InputText("Name", _name, MAX_PATH, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		
		ImGui::Spacing();
		ImGui::SetCursorPosX(63);
		ImGui::Text("Shader will be generated,");
		ImGui::SetCursorPosX(43);
		ImGui::Text("Choose a proper name. Otherwise");
		ImGui::SetCursorPosX(63);
		ImGui::Text("it won't be created :D");
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

void PanelShaderEditor::LoadShaderContext()
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
	
		ImGui::SetNextWindowPos(hit_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 16));
		

		if (ImGui::BeginPopup("NewNode", ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			
			ImGui::PushItemWidth(100.0f);
			ImGui::InputText("##filter", node_filter, 64);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			if (ImGui::Selectable("X", false, ImGuiSelectableFlags_DontClosePopups, { 8,13 }))
			{
				memcpy(&node_filter, "\0", 1);
			}

			ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "CREATE NODE");
			ImGui::Separator();


			if (ImGui::TreeNodeEx("Parameter"))
			{
				ImVec2 node_pos;
				node_pos.x = hit_pos.x - canvas.m_Scroll.x;
				node_pos.y = hit_pos.y - canvas.m_Scroll.y;

				CreateNodeOption("Vector1", NODE_TYPE::VECTOR1, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Vector2", NODE_TYPE::VECTOR2, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Vector3", NODE_TYPE::VECTOR3, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Vector4", NODE_TYPE::VECTOR4, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Color", NODE_TYPE::COLOR, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Time", NODE_TYPE::TIME, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Texture"))
			{
				ImVec2 node_pos;
				node_pos.x = hit_pos.x - canvas.m_Scroll.x;
				node_pos.y = hit_pos.y - canvas.m_Scroll.y;

				CreateNodeOption("TextureSampler2D", NODE_TYPE::TEXTURE_SAMPLER, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Texture2D", NODE_TYPE::TEXTURE, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Geometry"))
			{
				ImVec2 node_pos;
				node_pos.x = hit_pos.x - canvas.m_Scroll.x;
				node_pos.y = hit_pos.y - canvas.m_Scroll.y;

				CreateNodeOption("UV", NODE_TYPE::UV, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Tiling And Offset", NODE_TYPE::TILING_OFFSET, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Panner", NODE_TYPE::PANNER, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Parallax Oclusion", NODE_TYPE::PARALLAX_OCLUSION, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Math"))
			{
				ImVec2 node_pos;
				node_pos.x = hit_pos.x - canvas.m_Scroll.x;
				node_pos.y = hit_pos.y - canvas.m_Scroll.y;

				CreateNodeOption("Add", NODE_TYPE::ADD, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Subtract", NODE_TYPE::SUBTRACT, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Multiply", NODE_TYPE::MULTIPLY, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);
				CreateNodeOption("Divide", NODE_TYPE::DIVIDE, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Master"))
			{
				ImVec2 node_pos;
				node_pos.x = hit_pos.x - canvas.m_Scroll.x;
				node_pos.y = hit_pos.y - canvas.m_Scroll.y;

				CreateNodeOption("Unlit", NODE_TYPE::PBR, float2(node_pos.x, node_pos.y), current_shader, current_shader->graph);

				ImGui::TreePop();
			}
						


			ImGui::Separator();
		
			ImGui::EndPopup();
		}
		
		ImGui::PopStyleVar();
	

}

void PanelShaderEditor::NodeOption(bool& open_menu, const char* name, NODE_TYPE type,float2 position, ResourceShader* shader, ShaderGraph* graph,  ShaderNode* (ShaderGraph::* p)(const char* n, int t, float2 pos))
{
	std::string node_name = name;

	if (node_name.find(node_filter) == std::string::npos)
		return;
	
	if (ImGui::Selectable(node_name.c_str()))
	{
		//AddNode(*shader->graph, (graph->*p)(name, (int)type));
		AddNode(*shader->graph, std::invoke(p, graph, name, type, position));
		//open_menu = false;
		
		//ImGui::CloseCurrentPopup();
	}
}

void PanelShaderEditor::NodeOption(const char* name, NODE_TYPE type, ResourceShader* shader, ShaderGraph* graph, bool& open_menu)
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

void PanelShaderEditor::CreateNodeOption(const char* name, NODE_TYPE type, float2 position, ResourceShader* shader, ShaderGraph* graph)
{
	std::string node_name = name;

	if (node_name.find(node_filter) == std::string::npos)
		return;

	if (ImGui::MenuItem(node_name.c_str()))
	{
		ShaderNode* new_node = graph->CreateNode(name, type, position);
		AddNode(*shader->graph, new_node);
		

		//ImGui::CloseCurrentPopup();
	}


}

ShaderGraph* PanelShaderEditor::GetGraph()
{
	if (current_shader != nullptr)
		return current_shader->graph;
	else
		return nullptr;
	
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
		{
			current_shader->graph->node_hovered = current_shader->graph->node_selected = nullptr;
			hit_pos = ImGui::GetIO().MousePos;
		
			ImGui::OpenPopup("NewNode");

		}
		
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
