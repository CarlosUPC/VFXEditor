#include "Application.h"
#include "PanelPreview.h"
#include "ModuleRenderer.h"


PanelPreview::PanelPreview(const char* name)
	:Panel(name)
{

}

PanelPreview::~PanelPreview()
{

}

void PanelPreview::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	
	ImGui::Begin(name.c_str(), &active, flags);
	ImGui::PopStyleVar(3);

	ImVec2 avail_size = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)App->renderer->texture, avail_size, { 0,1 }, { 1,0 });

	ImGui::End();
}
