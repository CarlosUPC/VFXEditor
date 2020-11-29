#include "Application.h"
#include "PanelPreview.h"
#include "ModuleRenderer.h"
#include "ModuleCamera.h"

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

	flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin(name.c_str(), &active, flags);
	ImGui::PopStyleVar(3);


	ImVec2 avail_size = ImGui::GetWindowSize();

	if (avail_size.x != size.x || avail_size.y != size.y)
	{
		size = { avail_size.x, avail_size.y };
		App->renderer->OnResize(size.x, size.y);
		App->camera->setAspectRatio(size.x/size.y);
	}
	ImGui::SetCursorPos({ 0,0 });
	ImGui::Image((ImTextureID)App->renderer->texture, ImVec2(size.x, size.y), { 0,1 }, { 1,0 });

	ImGui::End();
}
