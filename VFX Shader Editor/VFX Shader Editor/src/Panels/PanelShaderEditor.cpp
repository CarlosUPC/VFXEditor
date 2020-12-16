#include "Application.h"
#include "PanelShaderEditor.h"
#include "ResourceShader.h"
#include "ModuleWindow.h"

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

		if (ImGui::MenuItem("New Shader")) menu_action = "New";
		if (ImGui::MenuItem("Load Shader"))
		{
			menu_action = "Load";
			
		}
		if (ImGui::MenuItem("Save Shader")) menu_action = "Save";

		
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	
	
	OnShaderAction(menu_action);
	if (p_open)
	{

		
	}
		
	
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

	
		ImGui::End();
		return;
	}
	
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
	//ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);


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
