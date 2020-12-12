#include "PanelNodeGraph.h"

PanelNodeGraph::PanelNodeGraph(const char* name)
	:Panel(name)
{
}

PanelNodeGraph::~PanelNodeGraph()
{
}

void PanelNodeGraph::Draw()
{
	ImGui::Begin(name.c_str(), &active, flags);

	ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrollCoords.x, scrollCoords.y);
	ImVec2 win_pos = ImGui::GetCursorPos();
	ImGui::Text("ScreenCursorPos (%.2f,%.2f)", win_pos.x, win_pos.y);
	ImGui::SameLine(ImGui::GetWindowWidth() - 100);
	ImGui::Checkbox("Show grid", &grid);


	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 55, 200));

	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PushItemWidth(120.0f);

	//ImVec2 offset = { ImGui::GetCursorScreenPos().x + scrollCoords.x, ImGui::GetCursorScreenPos().y + scrollCoords.y };
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
	
	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);


	ImGui::End();
}
